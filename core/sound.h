// Sound system wrapper for miniaudio background music.
#ifndef SOUND_H
#define SOUND_H

#include "miniaudio.h"

typedef struct
{
    ma_engine engine;
    ma_sound bgm;
    int engineReady;
    int bgmReady;
} AudioSystem;

static AudioSystem gAudio;
static int gMusicEnabled = 1;

static int initAudioSystem(const char *musicPath)
{
    ma_result result;
    ma_engine_config config = ma_engine_config_init();

    gAudio.engineReady = 0;
    gAudio.bgmReady = 0;
    gMusicEnabled = 1;

    result = ma_engine_init(&config, &gAudio.engine);
    if (result != MA_SUCCESS)
    {
        return 0;
    }
    gAudio.engineReady = 1;

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
    return 1;
}

static void shutdownAudioSystem(void)
{
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

#endif
