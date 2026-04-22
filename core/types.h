#ifndef TYPES_H
#define TYPES_H

#include <conio.h>
#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "color.h"        /* He thong mau RGB thong nhat */

/*
 * MinGW libbgi co the export textheight voi C++ mangling,
 * trong khi mot so ban graphics.h khai bao kieu C.
 * Goi truc tiep symbol mangled de tranh loi undefined reference.
 */
extern int bgi_textheight_cpp(const char *text) asm("_Z10textheightPKc");
static int textHeightCompat(const char *text) { return bgi_textheight_cpp(text); }

/* ====================================================================== */
/*                         HANG SO (CONSTANTS)                            */
/* ====================================================================== */

#define WIDTH 1200
#define HEIGHT 700
#define MAX_ASTEROIDS 30  /* So thien thach toi da trong mang         */
#define MAX_BULLETS 50    /* So vien dan toi da tren man hinh         */
#define MAX_ITEMS 12      /* So vat pham (tinh the) toi da            */
#define MAX_EXPLOSIONS 35 /* So hieu ung no dong thoi                 */
#define MAX_STARS 100     /* So ngoi sao nen (tao hieu ung khong gian)*/

#define SHIP_SPEED 10   /* Toc do di chuyen phi thuyen (pixel/frame) */
#define SHIP_RADIUS 20  /* Ban kinh va cham cua phi thuyen (pixel)   */
#define BULLET_SPEED 10 /* Toc do bay cua vien dan (pixel/frame)     */
#define BULLET_RADIUS 3 /* Ban kinh vien dan (pixel)                 */

#define DEFAULT_SHOOT_CD 120 /* Cooldown ban dan mac dinh (ms)            */
#define MIN_SHOOT_CD 35      /* Cooldown toi thieu khi nang cap toi da    */
#define HIT_COOLDOWN 500     /* Thoi gian bat tu sau khi bi dam (ms)      */
#define INITIAL_HP 100       /* Mau HP ban dau cua phi thuyen             */
#define HIT_DAMAGE 12        /* Sat thuong moi lan bi thien thach dam     */

#define MAX_PELLETS 7  // So vien dan toi da moi phat ban
#define MAX_B_DAMAGE 5 // Sat thuong toi da moi vien dan

#define ITEM_DROP_CHANCE 5  /* Xac suat roi tinh the khi asteroid no (%) */
#define LEVEL_UP_SCORE 1000 /* Diem can de len level: score >= level*nay */
#define LEVEL_BANNER_FR 90  /* So frame hien banner "LEVEL UP" (~1.5s)   */

#define AST_MIN_R 15      /* Ban kinh thien thach nho nhat (pixel)     */
#define AST_MAX_R 45      /* Ban kinh thien thach lon nhat (pixel)     */
#define AST_SPD_MIN 3.0f  /* Toc do roi nho nhat (pixel/frame)         */
#define AST_SPD_MAX 10.5f /* Toc do roi lon nhat (pixel/frame)         */

/* Toa do nut MENU (goc trai tren) */
#define MENU_BTN_X1 10
#define MENU_BTN_Y1 10
#define MENU_BTN_X2 55
#define MENU_BTN_Y2 38

/* Toa do overlay panel */
#define PANEL_X1 (WIDTH / 2 - 180)
#define PANEL_Y1 (HEIGHT / 2 - 130)
#define PANEL_X2 (WIDTH / 2 + 180)
#define PANEL_Y2 (HEIGHT / 2 + 100)

/* Toa do nut CONTINUE */
#define CONT_X1 (WIDTH / 2 - 120)
#define CONT_Y1 (HEIGHT / 2 - 30)
#define CONT_X2 (WIDTH / 2 + 120)
#define CONT_Y2 (HEIGHT / 2 + 10)

/* Toa do nut EXIT */
#define EXIT_X1 (WIDTH / 2 - 120)
#define EXIT_Y1 (HEIGHT / 2 + 30)
#define EXIT_X2 (WIDTH / 2 + 120)
#define EXIT_Y2 (HEIGHT / 2 + 70)

/* Toa do cac nut trong Main Menu */
#define MENU_START_X1 (WIDTH / 2 - 150)
#define MENU_START_Y1 (HEIGHT / 2 - 20)
#define MENU_START_X2 (WIDTH / 2 + 150)
#define MENU_START_Y2 (HEIGHT / 2 + 30)

#define MENU_GUIDE_X1 (WIDTH / 2 - 150)
#define MENU_GUIDE_Y1 (HEIGHT / 2 + 50)
#define MENU_GUIDE_X2 (WIDTH / 2 + 150)
#define MENU_GUIDE_Y2 (HEIGHT / 2 + 100)

#define MENU_EXIT_X1 (WIDTH / 2 - 150)
#define MENU_EXIT_Y1 (HEIGHT / 2 + 120)
#define MENU_EXIT_X2 (WIDTH / 2 + 150)
#define MENU_EXIT_Y2 (HEIGHT / 2 + 170)

/* Toa do nut BACK trong Guide */
#define GUIDE_BACK_X1 (WIDTH / 2 - 100)
#define GUIDE_BACK_Y1 (HEIGHT - 80)
#define GUIDE_BACK_X2 (WIDTH / 2 + 100)
#define GUIDE_BACK_Y2 (HEIGHT - 40)

/* Toa do cac nut trong Game Over */
#define GAMEOVER_REPLAY_X1 (WIDTH / 2 - 150)
#define GAMEOVER_REPLAY_Y1 (HEIGHT / 2 + 50)
#define GAMEOVER_REPLAY_X2 (WIDTH / 2 + 150)
#define GAMEOVER_REPLAY_Y2 (HEIGHT / 2 + 100)

#define GAMEOVER_MENU_X1 (WIDTH / 2 - 150)
#define GAMEOVER_MENU_Y1 (HEIGHT / 2 + 120)
#define GAMEOVER_MENU_X2 (WIDTH / 2 + 150)
#define GAMEOVER_MENU_Y2 (HEIGHT / 2 + 170)

#ifndef M_PI
#define M_PI 3.14159265358979f /* Hang so Pi (dung tinh goc, ve hinh tron) */
#endif

/* ====================================================================== */
/*                         GAME STATES                                    */
/* ====================================================================== */

typedef enum
{
  STATE_MAIN_MENU,
  STATE_GUIDE,
  STATE_PLAYING,
  STATE_PAUSED,
  STATE_GAME_OVER
} GameState;

/* ====================================================================== */
/*                         STRUCT DEFINITIONS                             */
/* ====================================================================== */

/*
 * Ship  Phi thuyen cua nguoi choi
 */
typedef struct
{
  float x, y;
  int radius, speed;
  int bulletDamage;
  int pelletsPerShot;
  int shootCooldown;
} Ship;

/*
 * Bullet – Vien dan
 */
typedef struct
{
  float x, y, dx, dy;
  int active, radius, damage;
} Bullet;

/*
 * Asteroid – Thien thach
 */
typedef struct
{
  float x, y;
  float vx, vy;
  int radius, active;
  int seed;
  float rot, rotSpeed;
  int hp, maxHp;
} Asteroid;

/*
 * Item – Tinh the nang cap (Power Crystal)
 */
typedef struct
{
  float x, y, vy;
  int active, radius;
  int sparkle;
} Item;

typedef struct
{
  float x, y, maxRadius;
  int frame, maxFrames, active;
} Explosion;

/*
 * Skill 1 – BOMB BURST: no vong tron tai vi tri ship
 * Skill 2 – TIME SLOW: lam cham asteroid trong 4 giay
 * Skill 3 – PIERCING BEAM: tia xuyen thang theo huong chuot
 */
typedef struct
{
  unsigned long bombCD, bombLast;
  int bombActive, bombFrame;
  float bombX, bombY, bombRadius;
  int bombDmg, bombDmgDone;

  unsigned long slowCD, slowLast, slowDur;
  int slowActive;
  float slowFactor;

  unsigned long beamCD, beamLast, beamDur;
  int beamActive, beamDmg, beamDmgDone;
  float beamDX, beamDY, beamSX, beamSY;
} SkillSystem;

/*
 * GameConfig
 */
typedef struct
{
  int activeAsteroids;
  float astSpeedMin, astSpeedMax;
  float angleVariety;
  int baseAstHp;
} GameConfig;

/* ====================================================================== */
/*                         GLOBAL VARIABLES                               */
/* ====================================================================== */

static GameConfig gCfg;

/* Nang cap xoay vong: pelletsPerShot -> bulletDamage -> shootCooldown -> lap lai */
static int gItemCycle = 0;

/* Stars data */
static int sX[MAX_STARS], sY[MAX_STARS], sB[MAX_STARS];
static int sPhase[MAX_STARS]; /* Pha nhap nhay cua tung ngoi sao */
static int starsReady = 0;
static int gStarFrame = 0;      /* Dem frame toan cuc cho hieu ung nhap nhay sao */
static int gCurrentBGImage = 4; /* Chi so anh BMP hien tai (4, 5, 6...) */

/* ====================================================================== */
/*                         UTILITY FUNCTIONS                              */
/* ====================================================================== */

static unsigned int seedRand(unsigned int s, int i)
{
  s = s * 1103515245u + 12345u + (unsigned int)i * 2654435761u;
  return s;
}

/*
 * beamHitsCircle(): Kiem tra tia (beam) co cat qua hinh tron khong.
 */
static int beamHitsCircle(float sx, float sy, float dx, float dy, float cx,
                          float cy, float cr)
{
  float vx = cx - sx, vy = cy - sy;
  float proj = vx * dx + vy * dy;
  if (proj < 0)
    return 0;
  float px = vx - proj * dx, py = vy - proj * dy;
  return (sqrtf(px * px + py * py) < cr) ? 1 : 0;
}

void initStars(void)
{
  int i;
  for (i = 0; i < MAX_STARS; i++)
  {
    sX[i] = rand() % WIDTH;
    sY[i] = rand() % HEIGHT;
    sB[i] = rand() % 3;
    sPhase[i] = rand() % 60; /* Pha ngau nhien de sao nhap nhay lech nhau */
  }
  starsReady = 1;
}

/* pointInRect(): Kiem tra xem diem (x, y) co nam trong hinh chu nhat hay khong. */
int pointInRect(int x, int y, int x1, int y1, int x2, int y2)
{
  return (x >= x1 && x <= x2 && y >= y1 && y <= y2) ? 1 : 0;
}

/* Forward declarations */
void drawStars(void);
void drawBackground(int level);

#endif
