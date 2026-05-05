/* ====================================================================== */
/*                     ASTEROID BLASTER v3.0                              */
/*                                                                        */
/*  Main file: chi chua ham main() va includes.                           */
/*  Tat ca logic, structs, drawing da duoc tach ra cac file header.       */
/* ====================================================================== */

/* --- Core headers --- */
#include "core/types.h"      /* Structs, defines, constants, utilities  */

/* --- Sound system (must be before game_logic) --- */
#define MINIAUDIO_IMPLEMENTATION
#include "core/sound.h" /* Background music (miniaudio)          */

/* --- Game logic (needs sound system) --- */
#include "core/game_logic.h" /* Init, update, collision, skills         */

/* --- UI headers (backgrounds, menus) --- */
#include "ui/background_level.h" /* drawBackground, drawStars, levels    */

/* --- Core drawing headers --- */
#include "core/draw_ship.h"     /* drawShip, drawShipBody, engine, aura   */
#include "core/draw_asteroid.h" /* drawAsteroids (fixed: fillpoly)        */
#include "core/draw_effects.h"  /* drawBullets, Items, Explosions, Skills */

/* --- UI headers (screens, HUD) --- */
#include "ui/game_over.h"     /* drawGameOverScreen                      */
#include "ui/introlduction.h" /* drawGuideScreen                         */
#include "ui/menu.h"          /* drawMainMenu, drawMenuBackground        */
#include "ui/pause.h"         /* drawPauseMenu                           */
#include "ui/hud.h"           /* drawHUD, drawLevelBanner, drawMenuButton*/

/* ====================================================================== */
/*                              MAIN                                      */
/* ====================================================================== */

int main()
{
  Ship ship;
  Bullet bullets[MAX_BULLETS];
  Asteroid asteroids[MAX_ASTEROIDS];
  Item items[MAX_ITEMS];
  Explosion explosions[MAX_EXPLOSIONS];
  SkillSystem skills;

  int score = 0, hp = INITIAL_HP, level = 1;
  unsigned long lastShot = 0, lastHit = 0;
  unsigned long lastMenuClick = 0;
  int mx = WIDTH / 2, my = HEIGHT / 2;
  int frameCount = 0, levelBanner = 0, page = 0;
  int mxClick, myClick;

  GameState gameState = STATE_MAIN_MENU;
  int running = 1;
  int gameSpeed = 2; /* 0=Dung(Pause), 1=Cham, 2=Binh thuong, 3=Nhanh */

  int gd = DETECT, gm;
  initwindow(WIDTH, HEIGHT, "Asteroid Blaster v3.0");
  setbkcolor(COL_SPACE_BG);
  cleardevice();

  (void)initAudioSystem("asset/sound/background_music.mp3");

  /* Khoi tao double buffer: xoa sach ca 2 trang */
  setactivepage(0);
  cleardevice();
  setactivepage(1);
  cleardevice();
  /* Ban dau hien thi page 0, ve vao page 1 */
  setvisualpage(0);
  page = 0;

  /* Khoi tao sao nen 1 lan */
  if (!starsReady)
    initStars();

  while (running)
  {
    unsigned long now = GetTickCount();
    handleMouse(&mx, &my);

    /* Xu ly click chuot */
    if (ismouseclick(WM_LBUTTONDOWN))
    {
      getmouseclick(WM_LBUTTONDOWN, mxClick, myClick);

      switch (gameState)
      {
      case STATE_MAIN_MENU:
      {
        if (now - lastMenuClick < 200)
          break;
        lastMenuClick = now;
        int choice = handleMainMenuClick(mxClick, myClick);
        if (choice == 1)
        {
          playSoundEffect(SFX_CLICK_BUTTON); /* Button click sound */
          initGame(&ship, asteroids, bullets, items, explosions, &skills,
                   &score, &hp, &level);
          gameState = STATE_PLAYING;
          frameCount = 0;
          levelBanner = 0;
          clearmouseclick(WM_LBUTTONDOWN);
        }
        else if (choice == 2)
        {
          playSoundEffect(SFX_CLICK_BUTTON); /* Button click sound */
          gameState = STATE_GUIDE;
        }
        else if (choice == 3)
        {
          playSoundEffect(SFX_CLICK_BUTTON); /* Button click sound */
          running = 0;
        }
        else if (choice == 4)
        {
          playSoundEffect(SFX_CLICK_BUTTON); /* Button click sound */
          toggleMusic();
        }
        break;
      }

      case STATE_GUIDE:
      {
        int back = handleGuideClick(mxClick, myClick);
        if (back == 1)
        {
          playSoundEffect(SFX_CLICK_BUTTON); /* Button click sound */
          gameState = STATE_MAIN_MENU;
        }
        break;
      }

      case STATE_PAUSED:
      {
        int choice = handlePauseMenuClick(mxClick, myClick);
        if (choice == 1)
        {
          playSoundEffect(SFX_CLICK_BUTTON); /* Button click sound */
          gameState = STATE_PLAYING;
        }
        else if (choice == 2)
        {
          playSoundEffect(SFX_CLICK_BUTTON); /* Button click sound */
          gameState = STATE_MAIN_MENU;
        }
        break;
      }

      case STATE_GAME_OVER:
      {
        int choice = handleGameOverClick(mxClick, myClick);
        if (choice == 1)
        {
          playSoundEffect(SFX_CLICK_BUTTON); /* Button click sound */
          initGame(&ship, asteroids, bullets, items, explosions, &skills,
                   &score, &hp, &level);
          gameState = STATE_PLAYING;
          frameCount = 0;
          levelBanner = 0;
        }
        else if (choice == 2)
        {
          playSoundEffect(SFX_CLICK_BUTTON); /* Button click sound */
          gameState = STATE_MAIN_MENU;
        }
        break;
      }

      case STATE_PLAYING:
        if (pointInRect(mxClick, myClick, MENU_BTN_X1, MENU_BTN_Y1, MENU_BTN_X2,
                        MENU_BTN_Y2))
        {
          gameState = STATE_PAUSED;
        }
        break;
      }
    }

    /* Xu ly phim ESC trong game de pause */
    if (gameState == STATE_PLAYING)
    {
      if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
      {
        gameState = STATE_PAUSED;
        delay(200);
      }

      /* Xu ly phim 0-3: Dieu khien toc do game */
      if (GetAsyncKeyState('0') & 0x8000)
      {
        gameSpeed = 0;
        gameState = STATE_PAUSED;
        delay(200);
      }
      else if (GetAsyncKeyState('1') & 0x8000)
      {
        gameSpeed = 1; /* Cham */
        delay(150);
      }
      else if (GetAsyncKeyState('2') & 0x8000)
      {
        gameSpeed = 2; /* Binh thuong */
        delay(150);
      }
      else if (GetAsyncKeyState('3') & 0x8000)
      {
        gameSpeed = 3; /* Nhanh */
        delay(150);
      }
    }
    else if (gameState == STATE_PAUSED)
    {
      if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
      {
        if (gameSpeed == 0) gameSpeed = 2; /* Reset ve binh thuong */
        gameState = STATE_PLAYING;
        delay(200);
      }
    }

    /* ================================================================ */
    /*   Xu ly logic va render theo trang thai                          */
    /*   FIX: Ve vao trang AN (1 - page), sau do swap hien thi         */
    /* ================================================================ */
    switch (gameState)
    {
    case STATE_MAIN_MENU:
      setactivepage(1 - page);
      cleardevice();
      drawMainMenu(mx, my, isMusicEnabled());
      setvisualpage(1 - page);
      page = 1 - page;
      delay(20);
      break;

    case STATE_GUIDE:
      setactivepage(1 - page);
      cleardevice();
      drawGuideScreen(mx, my);
      setvisualpage(1 - page);
      page = 1 - page;
      delay(20);
      break;

    case STATE_PLAYING:
    {
      frameCount++;

      /* BUOC 1: INPUT */
      handleKeyboardMove(&ship);
      handleSkillsInput(&skills, &ship, mx, my, now);

      /* BUOC 2: UPDATE */
      updateShip(&ship);
      tryAutoShoot(&ship, bullets, MAX_BULLETS, mx, my, &lastShot);
      updateBullets(bullets, MAX_BULLETS);

      float spdM = skills.slowActive ? skills.slowFactor : 1.0f;
      updateAsteroids(asteroids, MAX_ASTEROIDS, level, spdM);

      updateItems(items, MAX_ITEMS);
      updateExplosions(explosions, MAX_EXPLOSIONS);
      updateSkillEffects(&skills, now);

      /* BUOC 3: COLLISION */
      handleBulletAsteroidCollisions(bullets, MAX_BULLETS, asteroids,
                                     MAX_ASTEROIDS, &score, level, items,
                                     explosions);
      handleShipAsteroidCollision(&ship, asteroids, MAX_ASTEROIDS, &hp,
                                  &lastHit, explosions, level);
      handleItemPickup(&ship, items, MAX_ITEMS);

      /* BUOC 4: SKILL DAMAGE */
      applySkillDamage(&skills, asteroids, MAX_ASTEROIDS, &score, level, items,
                       explosions);

      /* BUOC 5: LEVEL CHECK */
      if (checkLevelUp(score, level))
      {
        level++;
        updateDifficultyByLevel(level, &gCfg);
        levelBanner = LEVEL_BANNER_FR;

        {
          int i;
          for (i = 0; i < MAX_ASTEROIDS; i++)
          {
            initAsteroid(&asteroids[i], level, &gCfg);
            asteroids[i].y = (float)(-(rand() % 300) - 200);
          }
        }
      }

      if (hp <= 0)
      {
        playSoundEffect(SFX_GAMEOVER); /* Play game over sound */
        gameState = STATE_GAME_OVER;
      }

      /* BUOC 6: RENDER (ve vao trang an) */
      setactivepage(1 - page);
      cleardevice();

      drawBackground(level);
      drawAsteroids(asteroids, MAX_ASTEROIDS);
      drawItems(items, MAX_ITEMS);
      drawBullets(bullets, MAX_BULLETS);
      drawExplosions(explosions, MAX_EXPLOSIONS);
      drawSkillEffects(&skills);
      drawShip(ship, mx, my, frameCount);
      drawHUD(score, hp, level, ship, &skills, gameSpeed);

      if (levelBanner > 0)
      {
        drawLevelBanner(level);
        levelBanner--;
      }

      {
        int btnHov = pointInRect(mx, my, MENU_BTN_X1, MENU_BTN_Y1, MENU_BTN_X2,
                                 MENU_BTN_Y2);
        drawMenuButton(btnHov);
      }

      /* Swap: hien thi trang vua ve xong */
      setvisualpage(1 - page);
      page = 1 - page;

      /* BUOC 7: DELAY (thay doi theo toc do game) */
      {
        int frameDelay = 20; /* Mac dinh: binh thuong */
        if (gameSpeed == 1) frameDelay = 40;      /* Cham */
        else if (gameSpeed == 3) frameDelay = 10;  /* Nhanh */
        delay(frameDelay);
      }
      break;
    }

    case STATE_PAUSED:
    {
      setactivepage(1 - page);
      cleardevice();

      /* Ve lai game phia sau */
      drawBackground(level);
      drawAsteroids(asteroids, MAX_ASTEROIDS);
      drawItems(items, MAX_ITEMS);
      drawBullets(bullets, MAX_BULLETS);
      drawExplosions(explosions, MAX_EXPLOSIONS);
      drawSkillEffects(&skills);
      drawShip(ship, mx, my, frameCount);
      drawHUD(score, hp, level, ship, &skills, gameSpeed);

      {
        int btnHov = pointInRect(mx, my, MENU_BTN_X1, MENU_BTN_Y1, MENU_BTN_X2,
                                 MENU_BTN_Y2);
        drawMenuButton(btnHov);
      }

      /* Ve menu pause len tren */
      drawPauseMenu(mx, my);

      setvisualpage(1 - page);
      page = 1 - page;
      delay(20);
      break;
    }

    case STATE_GAME_OVER:
      setactivepage(1 - page);
      cleardevice();
      drawGameOverScreen(score, level, mx, my);
      setvisualpage(1 - page);
      page = 1 - page;
      delay(20);
      break;
    }
  }

  closegraph();
  shutdownAudioSystem();
  return 0;
}
