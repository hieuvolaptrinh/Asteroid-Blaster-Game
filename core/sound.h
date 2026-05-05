// Sound system wrapper for miniaudio background music and sound effects.
#ifndef SOUND_H
#define SOUND_H

#include "miniaudio.h"

/* Sound effect types */
typedef enum
{
    SFX_SHOT = 0,
    SFX_EXPLOSION,
    SFX_SKILL7,
    SFX_SKILL8,
    SFX_SKILL9,
    SFX_LEVEL_UP,
    SFX_LEVEL_DOWN,
    SFX_GAMEOVER,
    SFX_CLICK_BUTTON,
    SFX_GAME_START,
    SFX_COUNT /* Total number of sound effects */
} SoundEffectType;

typedef struct
{
    ma_engine engine;
    ma_sound bgm;
    ma_sound sfx[SFX_COUNT];
    int engineReady;
    int bgmReady;
    int sfxReady[SFX_COUNT];
} AudioSystem;

static AudioSystem gAudio;
static int gMusicEnabled = 1;

/* Sound effect file paths */
static const char *sfxPaths[SFX_COUNT] = {
    "asset/sound/shot.mp3",
    "asset/sound/explosion.mp3",
    "asset/sound/skill7.mp3",
    "asset/sound/skill8.mp3",
    "asset/sound/skill9.mp3",
    "asset/sound/level_up.mp3",
    "asset/sound/level_down.mp3",
    "asset/sound/gameover.mp3",
    "asset/sound/click_button.mp3",
    "asset/sound/game_start.mp3"
};

static int initAudioSystem(const char *musicPath)
{
    ma_result result;
    ma_engine_config config = ma_engine_config_init();
    int i;

    gAudio.engineReady = 0;
    gAudio.bgmReady = 0;
    gMusicEnabled = 1;

    /* Initialize all sfx as not ready */
    for (i = 0; i < SFX_COUNT; i++)
    {
        gAudio.sfxReady[i] = 0;
    }

    result = ma_engine_init(&config, &gAudio.engine);
    if (result != MA_SUCCESS)
    {
        return 0;
    }
    gAudio.engineReady = 1;

    /* Initialize background music */
    result = ma_sound_init_from_file(
        &gAudio.engine, musicPath,
        MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_NO_SPATIALIZATION, NULL, NULL,
        &gAudio.bgm);
    if (result != MA_SUCCESS)
    {
        ma_engine_uninit(&gAudio.engine);
        gAudio.engineReady = 0;
        return 0;
    }

    gAudio.bgmReady = 1;
    ma_sound_set_looping(&gAudio.bgm, MA_TRUE);
    ma_sound_set_volume(&gAudio.bgm, 0.5f); /* Set volume to 50% */
    ma_sound_start(&gAudio.bgm);

    /* Initialize all sound effects */
    for (i = 0; i < SFX_COUNT; i++)
    {
        result = ma_sound_init_from_file(
            &gAudio.engine, sfxPaths[i],
            MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_SPATIALIZATION, NULL, NULL,
            &gAudio.sfx[i]);
        if (result == MA_SUCCESS)
        {
            gAudio.sfxReady[i] = 1;
            ma_sound_set_volume(&gAudio.sfx[i], 0.7f); /* SFX at 70% volume */
        }
    }

    return 1;
}

static void shutdownAudioSystem(void)
{
    int i;

    /* Uninit all sound effects */
    for (i = 0; i < SFX_COUNT; i++)
    {
        if (gAudio.sfxReady[i])
        {
            ma_sound_uninit(&gAudio.sfx[i]);
            gAudio.sfxReady[i] = 0;
        }
    }

    if (gAudio.bgmReady)
    {
        ma_sound_uninit(&gAudio.bgm);
        gAudio.bgmReady = 0;
    }
    if (gAudio.engineReady)
    {
        ma_engine_uninit(&gAudio.engine);
        gAudio.engineReady = 0;
    }
}

static void pauseMusic(void)
{
    if (gAudio.bgmReady)
    {
        ma_sound_stop(&gAudio.bgm);
    }
}

static void resumeMusic(void)
{
    if (gAudio.bgmReady)
    {
        ma_sound_start(&gAudio.bgm);
    }
}

static void stopMusic(void)
{
    if (gAudio.bgmReady)
    {
        ma_sound_stop(&gAudio.bgm);
        ma_sound_seek_to_pcm_frame(&gAudio.bgm, 0);
    }
}

static void setMusicVolume(float volume)
{
    if (gAudio.bgmReady)
    {
        ma_sound_set_volume(&gAudio.bgm, volume);
    }
}

static int isMusicEnabled(void)
{
    return gMusicEnabled;
}

static void toggleMusic(void)
{
    gMusicEnabled = gMusicEnabled ? 0 : 1;
    if (gMusicEnabled)
    {
        resumeMusic();
    }
    else
    {
        pauseMusic();
    }
}

/* Play a sound effect (non-looping, restart if already playing) */
void playSoundEffect(SoundEffectType sfxType)
{
    if (sfxType >= 0 && sfxType < SFX_COUNT && gAudio.sfxReady[sfxType])
    {
        /* Stop and rewind to beginning, then play */
        ma_sound_seek_to_pcm_frame(&gAudio.sfx[sfxType], 0);
        ma_sound_start(&gAudio.sfx[sfxType]);
    }
}

#endif
