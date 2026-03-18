#include <conio.h>
#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

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

/* Forward declarations */
void drawStars(void);
void drawLevel2Background(void);
void drawLevel3Background(void);
void drawLevel4Background(void);
void drawLevel5Background(void);
void drawLevel6Background(void);
void drawBackground(int level);
int pointInRect(int x, int y, int x1, int y1, int x2, int y2);
void drawMainMenu(int mx, int my);
int handleMainMenuClick(int mx, int my);
void drawGuideScreen(int mx, int my);
int handleGuideClick(int mx, int my);
void drawPauseMenu(int hmx, int hmy);
int handlePauseMenuClick(int mx, int my);
void drawGameOverScreen(int score, int level, int mx, int my);
int handleGameOverClick(int mx, int my);

/* Game States */
typedef enum {
  STATE_MAIN_MENU,
  STATE_GUIDE,
  STATE_PLAYING,
  STATE_PAUSED,
  STATE_GAME_OVER
} GameState;

/*
 * Nang cap xoay vong: pelletsPerShot -> bulletDamage -> shootCooldown -> lap
 * lai.
 */
static int gItemCycle = 0;

#ifndef M_PI
#define M_PI 3.14159265358979f /* Hang so Pi (dung tinh goc, ve hinh tron) */
#endif

/*
 * Ship  Phi thuyen cua nguoi choi
 * radius         : ban kinh va cham â€“ dung de kiem tra va cham voi
 * asteroid/item speed          : so pixel di chuyen moi frame khi nhan phim
 * bulletDamage   : sat thuong moi vien dan (tang khi nhat tinh the)
 * pelletsPerShot : so vien dan phong ra moi lan ban (tang khi nhat tinh the)
 * shootCooldown  : thoi gian giua 2 lan ban (ms), giam = ban nhanh hon
 */
typedef struct {
  float x, y;
  int radius, speed;
  int bulletDamage;
  int pelletsPerShot;
  int shootCooldown;
} Ship;

/*
 * Bullet â€“ Vien dan
 */
typedef struct {
  float x, y, dx, dy;
  int active, radius, damage;
} Bullet;

/*
 * Asteroid â€“ Thien thach
 */
typedef struct {
  float x, y;
  float vx, vy;
  int radius, active;
  int seed;
  float rot, rotSpeed;
  int hp, maxHp;
} Asteroid;

/*
 * Item â€“ Tinh the nang cap (Power Crystal)
 */
typedef struct {
  float x, y, vy;
  int active, radius;
  int sparkle;
} Item;

typedef struct {
  float x, y, maxRadius;
  int frame, maxFrames, active;
} Explosion;

/*

 * Skill 1 â€“ BOMB BURST: no vong tron tai vi tri ship
 * Skill 2 â€“ TIME SLOW: lam cham asteroid trong 4 giay
 * Skill 3 â€“ PIERCING BEAM: tia xuyen thang theo huong chuot
 */
typedef struct {
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
 * activeAsteroids : so asteroid hoat dong cung luc (tang theo level)
 * astSpeedMin/Max : khoang toc do roi (tang theo level)
 * angleVariety    : do da dang goc roi (0 = thang, lon = roi cheo nhieu)
 * baseAstHp       : mau co ban cua asteroid tai level hien tai
 */
typedef struct {
  int activeAsteroids;
  float astSpeedMin, astSpeedMax;
  float angleVariety;
  int baseAstHp;
} GameConfig;
static GameConfig gCfg;

static int sX[MAX_STARS], sY[MAX_STARS], sB[MAX_STARS];
static int sPhase[MAX_STARS]; /* Pha nhap nhay cua tung ngoi sao */
static int starsReady = 0;
static int gStarFrame = 0; /* Dem frame toan cuc cho hieu ung nhap nhay sao */
static int gCurrentBGImage = 4; /* Chi so anh BMP hien tai (4, 5, 6...) */

static unsigned int seedRand(unsigned int s, int i) {
  s = s * 1103515245u + 12345u + (unsigned int)i * 2654435761u;
  return s;
}

/*
 * beamHitsCircle(): Kiem tra tia (beam) co cat qua hinh tron khong.
 * Dung cho Skill 3 (Piercing Beam) â€“ xac dinh asteroid nao bi tia ban trung.
 * Tham so: (sx,sy) diem bat dau, (dx,dy) huong tia (da normalize),
 *          (cx,cy) tam hinh tron, cr ban kinh.
 * Tra ve: 1 = trung, 0 = truot.
 */
static int beamHitsCircle(float sx, float sy, float dx, float dy, float cx,
                          float cy, float cr) {
  float vx = cx - sx, vy = cy - sy;
  float proj = vx * dx + vy * dy;
  if (proj < 0)
    return 0;
  float px = vx - proj * dx, py = vy - proj * dy;
  return (sqrtf(px * px + py * py) < cr) ? 1 : 0;
}

void initStars(void) {
  int i;
  for (i = 0; i < MAX_STARS; i++) {
    sX[i] = rand() % WIDTH;
    sY[i] = rand() % HEIGHT;
    sB[i] = rand() % 3;
    sPhase[i] = rand() % 60; /* Pha ngau nhien de sao nhap nhay lech nhau */
  }
  starsReady = 1;
}

// updateDifficultyByLevel(): Cap nhat cau hinh do kho dua tren level hien tai.

void updateDifficultyByLevel(int level, GameConfig *cfg) {
  /* So asteroid toi da tang 2 moi level, nhung khong qua MAX_ASTEROIDS */
  cfg->activeAsteroids = 8 + level * 2;
  if (cfg->activeAsteroids > MAX_ASTEROIDS)
    cfg->activeAsteroids = MAX_ASTEROIDS; /* Gioi han tren */
  /* Toc do roi tang nhe moi level (asteroid nhanh dan) */
  cfg->astSpeedMin = AST_SPD_MIN + level * 0.12f;
  cfg->astSpeedMax = AST_SPD_MAX + level * 0.22f;
  /* Goc roi chenh lech nhieu hon -> asteroid bay cheo hon */
  cfg->angleVariety = 0.3f + level * 0.08f;
  if (cfg->angleVariety > 1.2f)
    cfg->angleVariety = 1.2f; /* Gioi han tren */
  /* Mau asteroid tang: level 1 = 1HP, level 2-3 = 2HP, v.v. */
  cfg->baseAstHp = 1 + level / 2;
}

void initShip(Ship *s) {
  s->x = WIDTH / 2.0f;
  s->y = HEIGHT - 80.0f; /* Giua chieu ngang, cach day 80px */
  s->radius = SHIP_RADIUS;
  s->speed = SHIP_SPEED;
  s->bulletDamage = 1;   /* Sat thuong 1 diem/vien ban dau */
  s->pelletsPerShot = 1; /* Ban 1 vien moi phat (tang khi nhat item) */
  s->shootCooldown = DEFAULT_SHOOT_CD; /* Cooldown 120ms = khoang 8 phat/giay */
}

void initBullet(Bullet *b) {
  b->x = b->y = b->dx = b->dy = 0; /* Reset toa do va van toc ve 0 */
  b->active = 0;
  b->radius = BULLET_RADIUS;
  b->damage = 1;
}

/*
 * initAsteroid(): Tao 1 thien thach moi o vi tri ngau nhien PHIA TREN man hinh.
 */
void initAsteroid(Asteroid *a, int level, GameConfig *cfg) {
  float spd, mxA;

  a->radius = AST_MIN_R + rand() % (AST_MAX_R - AST_MIN_R + 1);
  /* Vi tri X ngau nhien, dam bao asteroid nam trong man hinh */
  a->x = (float)(a->radius + rand() % (WIDTH - 2 * a->radius));
  /* Bat dau tren man hinh (y am), khoang cach ngau nhien de asteroid xuat hien
   * dan */
  a->y = (float)(-(rand() % 400) - a->radius);

  spd = cfg->astSpeedMin +
        (rand() % 100) / 100.0f * (cfg->astSpeedMax - cfg->astSpeedMin);
  mxA = cfg->angleVariety;
  a->vy = spd;

  a->vx = ((rand() % 200 - 100) / 100.0f) * mxA * spd * 0.4f;

  a->seed = rand();
  a->rot = 0;

  a->rotSpeed = ((rand() % 100) - 50) / 1500.0f;
  a->maxHp = cfg->baseAstHp;
  a->hp = a->maxHp;
  a->active = 1;
}

// initAllAsteroids(): Khoi tao toan bo mang asteroid khi bat dau game.

void initAllAsteroids(Asteroid ast[], int n, int level, GameConfig *cfg) {
  int i;
  for (i = 0; i < n; i++) {
    initAsteroid(&ast[i], level, cfg);
    /* Neu vuot so luong cho phep: tat bo, cho toi khi level tang */
    if (i >= cfg->activeAsteroids)
      ast[i].active = 0;
  }
}

void initSkillSystem(SkillSystem *sk) {
  /* Skill 1 â€“ Bomb Burst: CD 8 giay, ban kinh 160px, dame 8 */
  sk->bombCD = 8000;
  sk->bombLast = 0;
  sk->bombActive = 0;
  sk->bombFrame = 0;
  sk->bombRadius = 160.0f;
  sk->bombDmg = 8;
  sk->bombDmgDone = 0; /* Co ngan dame bi ap dung nhieu lan */

  /* Skill 2 â€“ Time Slow: CD 12 giay, ham cham 4 giay, he so 0.35 */
  sk->slowCD = 12000;
  sk->slowLast = 0;
  sk->slowDur = 4000;
  sk->slowActive = 0;
  sk->slowFactor = 0.35f; /* Asteroid chi di chuyen 35% toc do binh thuong */

  /* Skill 3 â€“ Piercing Beam: CD 10 giay, hien tia 450ms, dame 12 */
  sk->beamCD = 10000;
  sk->beamLast = 0;
  sk->beamDur = 450;
  sk->beamActive = 0;
  sk->beamDmg = 12;
  sk->beamDmgDone = 0;
}

// initGame(): Khoi dong toan bo trang thai game ve gia tri ban dau.
void initGame(Ship *ship, Asteroid ast[], Bullet bul[], Item items[],
              Explosion exps[], SkillSystem *sk, int *score, int *hp,
              int *level) {
  int i;

  srand((unsigned int)time(NULL));
  *level = 1;
  *score = 0;
  *hp = INITIAL_HP;
  updateDifficultyByLevel(*level, &gCfg);
  initShip(ship);
  initAllAsteroids(ast, MAX_ASTEROIDS, *level, &gCfg);
  for (i = 0; i < MAX_BULLETS; i++)
    initBullet(&bul[i]); /* Tat toan bo dan */
  for (i = 0; i < MAX_ITEMS; i++) {
    items[i].active = 0;
    items[i].radius = 12;
  } /* Tat tinh the */
  for (i = 0; i < MAX_EXPLOSIONS; i++)
    exps[i].active = 0; /* Tat hieu ung no */
  initSkillSystem(sk);
  if (!starsReady)
    initStars(); /* Chi tao sao 1 lan duy nhat */
}

void handleKeyboardMove(Ship *s) {
  if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('A') & 0x8000)
    s->x -= s->speed;
  if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000)
    s->x += s->speed;
  if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState('W') & 0x8000)
    s->y -= s->speed;
  if (GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState('S') & 0x8000)
    s->y += s->speed;
}

// handleMouse(): Cap nhat toa do con tro chuot moi frame.
void handleMouse(int *mx, int *my) {
  *mx = mousex(); /* Lay toa do X chuot (pixel) trong cua so WinBGIm */
  *my = mousey(); /* Lay toa do Y chuot (pixel) trong cua so WinBGIm */
}

void handleSkillsInput(SkillSystem *sk, Ship *ship, int mx, int my,
                       unsigned long now) {
  /* Skill 1 â€“ Bomb Burst: phim '1', CD 8000ms */
  if (GetAsyncKeyState('1') & 0x8000) {
    /* now - bombLast >= bombCD: kiem tra da het cooldown chua */
    if (now - sk->bombLast >= sk->bombCD) {
      sk->bombLast = now;  /* Ghi lai thoi diem vua dung */
      sk->bombActive = 1;  /* Bat hieu ung no */
      sk->bombFrame = 0;   /* Reset frame animation */
      sk->bombX = ship->x; /* Vi tri no = vi tri ship luc ban */
      sk->bombY = ship->y;
      sk->bombDmgDone = 0; /* Cho phep ap dung dame lan nay */
    }
  }

  if (GetAsyncKeyState('2') & 0x8000) {
    if (now - sk->slowLast >= sk->slowCD) {
      sk->slowLast = now;
      sk->slowActive = 1; /* Bat trang thai lam cham */
    }
  }

  if (GetAsyncKeyState('3') & 0x8000) {
    if (now - sk->beamLast >= sk->beamCD) {
      /* Tinh huong tia tu ship den vi tri chuot */
      float dx = mx - ship->x, dy = my - ship->y;
      float ln = sqrtf(dx * dx + dy * dy); /* Do dai vector huong */
      if (ln < 1.0f)
        return; /* Chuot trung ship, bo qua */
      sk->beamLast = now;
      sk->beamActive = 1;
      sk->beamDmgDone = 0;
      sk->beamSX = ship->x;
      sk->beamSY = ship->y; /* Diem bat dau tia */
      /* Normalize: chia cho do dai de co vector don vi (dai = 1) */
      sk->beamDX = dx / ln;
      sk->beamDY = dy / ln;
    }
  }
}

void updateShip(Ship *s) {
  if (s->x - s->radius < 0)
    s->x = (float)s->radius; /* Can bien trai */
  if (s->x + s->radius > WIDTH)
    s->x = (float)(WIDTH - s->radius); /* Can bien phai */
  if (s->y - s->radius < 0)
    s->y = (float)s->radius; /* Can bien tren */
  if (s->y + s->radius > HEIGHT)
    s->y = (float)(HEIGHT - s->radius); /* Can bien duoi */
}

static int freeBullet(Bullet b[], int n) {
  int i;
  for (i = 0; i < n; i++)
    if (!b[i].active)
      return i;
  return -1;
}

// shootPellets(): Tao N vien dan (N = pelletsPerShot) theo huong (dirX, dirY).

void shootPellets(Ship *ship, Bullet bul[], int n, float dirX, float dirY) {
  int p, idx;
  /* atan2f(y, x): tinh goc (radian) cua vector (dirX, dirY) so voi truc X.
   * Day la ham math.h, bien vector huong thanh goc de tinh spread sau do. */
  float base = atan2f(dirY, dirX);
  int cnt = ship->pelletsPerShot;
  /* Neu ban nhieu vien: trai cac goc xung quanh goc chinh trong khoang
   * totalSpread */
  float totalSpread = (cnt > 1) ? (cnt - 1) * 0.08f : 0;
  if (totalSpread > 0.5f)
    totalSpread = 0.5f; /* Gioi han spread toi da ~28 do */

  for (p = 0; p < cnt; p++) {
    /* off: goc lech cua vien thu p so voi trung tam, phan bo deu */
    float off = 0;
    if (cnt > 1)
      off = -totalSpread / 2.0f + totalSpread * p / (cnt - 1);
    float a = base + off; /* Goc thuc cua vien dan nay */

    idx = freeBullet(bul, n); /* Tim slot rong trong mang dan */
    if (idx < 0)
      return; /* Mang dan day, bo qua */
    bul[idx].x = ship->x;
    bul[idx].y = ship->y;
    /* cosf(a) va sinf(a): chuyen goc thanh thanh phan x, y cua vector van toc
     */
    bul[idx].dx = cosf(a) * BULLET_SPEED;
    bul[idx].dy = sinf(a) * BULLET_SPEED;
    bul[idx].active = 1;
    bul[idx].radius = BULLET_RADIUS;
    bul[idx].damage = ship->bulletDamage;
  }
}

// tryAutoShoot(): Tu dong ban dan moi khi het cooldown.

void tryAutoShoot(Ship *ship, Bullet bul[], int n, int mx, int my,
                  unsigned long *lastShot) {
  /* GetTickCount(): lay so ms ke tu khi may khoi dong (Windows API) */
  unsigned long now = GetTickCount();
  /* Neu chua het cooldown thi chua ban */
  if (now - *lastShot < (unsigned long)ship->shootCooldown)
    return;
  /* Tinh vector huong tu ship den chuot */
  float dx = mx - ship->x, dy = my - ship->y;
  float ln = sqrtf(dx * dx + dy * dy); /* Do dai vector (khoang cach) */
  if (ln < 5.0f)
    return; /* Chuot qua gan ship: bo qua de tranh chia cho 0 */
  /* Normalize: chia dx,dy cho do dai -> vector don vi (do dai = 1) */
  shootPellets(ship, bul, n, dx / ln, dy / ln);
  *lastShot = now; /* Ghi lai thoi diem vua ban */
}
void updateBullets(Bullet b[], int n) {
  int i;
  for (i = 0; i < n; i++) {
    if (!b[i].active)
      continue; /* Bo qua dan chua hoat dong */
    b[i].x += b[i].dx;
    b[i].y += b[i].dy; /* Di chuyen theo van toc */
    /* Tat dan khi ra ngoai vung nhin (20px lem) */
    if (b[i].x < -20 || b[i].x > WIDTH + 20 || b[i].y < -20 ||
        b[i].y > HEIGHT + 20)
      b[i].active = 0;
  }
}

// updateAsteroids(): Di chuyen va xoay cac asteroid moi frame.
void updateAsteroids(Asteroid a[], int n, int level, float spdMul) {
  int i;
  for (i = 0; i < n; i++) {
    if (!a[i].active)
      continue;

    a[i].x += a[i].vx * spdMul;
    a[i].y += a[i].vy * spdMul;
    a[i].rot += a[i].rotSpeed; /* Xoay them goc moi frame */

    if (a[i].y - a[i].radius > HEIGHT || a[i].x + a[i].radius < -60 ||
        a[i].x - a[i].radius > WIDTH + 60) {
      initAsteroid(&a[i], level, &gCfg); /* Tao lai asteroid moi */
    }
  }

  for (i = 0; i < n && i < gCfg.activeAsteroids; i++)
    if (!a[i].active)
      initAsteroid(&a[i], level, &gCfg);
}

void updateItems(Item it[], int n) {
  int i;
  for (i = 0; i < n; i++) {
    if (!it[i].active)
      continue;
    it[i].y += it[i].vy; /* Roi xuong moi frame */
    if (it[i].y > HEIGHT + 20)
      it[i].active = 0; /* Ra khoi man hinh: mat */
  }
}

void updateExplosions(Explosion e[], int n) {
  int i;
  for (i = 0; i < n; i++) {
    if (!e[i].active)
      continue;
    e[i].frame++; /* Tien sang frame tiep theo cua animation no */
    if (e[i].frame >= e[i].maxFrames)
      e[i].active = 0; /* Xong animation: tat */
  }
}

void updateSkillEffects(SkillSystem *sk, unsigned long now) {

  if (sk->bombActive) {
    sk->bombFrame++; /* Moi frame animation no ra them */
    if (sk->bombFrame >= 22)
      sk->bombActive = 0; /* Het animation: tat */
  }

  if (sk->slowActive) {
    /* now - slowLast: thoi gian da troi ke tu kich hoat */
    if (now - sk->slowLast >= sk->slowDur)
      sk->slowActive = 0; /* Het 4 giay: tra toc do binh thuong */
  }
  /* Beam: tat khi da hien du beamDur ms (450ms) */
  if (sk->beamActive) {
    if (now - sk->beamLast >= sk->beamDur)
      sk->beamActive = 0; /* Tat tia sau 450ms */
  }
}

// VA CHAM + ITEM + SKILL DAMAGE

int checkCircleCollision(float x1, float y1, float r1, float x2, float y2,
                         float r2) {
  float dx = x2 - x1, dy = y2 - y1;
  return (sqrtf(dx * dx + dy * dy) < r1 + r2) ? 1 : 0;
}

// spawnExplosion(): Tao hieu ung no tai vi tri (x, y) voi ban kinh toi da mr.

void spawnExplosion(Explosion e[], int n, float x, float y, float mr) {
  int i;
  for (i = 0; i < n; i++) {
    if (!e[i].active) {
      e[i].x = x;
      e[i].y = y;
      e[i].maxRadius = mr;
      e[i].frame = 0;
      e[i].maxFrames = 12;
      e[i].active = 1;
      return; /* Chi can 1 slot, tim duoc la thoat */
    }
  }
}

// spawnItem(): Tao tinh the roi tai vi tri (x, y) khi asteroid bi no.

void spawnItem(Item it[], int n, float x, float y) {
  int i;
  for (i = 0; i < n; i++) {
    if (!it[i].active) {
      it[i].x = x;
      it[i].y = y;
      it[i].vy = 1.5f; /* Toc do roi xuong */
      it[i].active = 1;
      it[i].radius = 13;
      it[i].sparkle = 0; /* Reset hieu ung lap lanh */
      return;
    }
  }
}

/* applyItemToShip(): Ap dung nang cap xoay vong khi nhat tinh the.
 * Thu tu xoay vong: vien (pellets) -> sat thuong (damage) -> toc ban (cooldown)
 * -> lap lai. gItemCycle % 3 quyet dinh nang cap nao duoc ap dung lan nay. */
void applyItemToShip(Ship *s) {
  switch (gItemCycle % 3) {
  case 0: /* Nang cap so vien: +1 (toi da MAX_PELLETS) */
    if (s->pelletsPerShot < MAX_PELLETS)
      s->pelletsPerShot++;
    break;
  case 1: /* Nang cap sat thuong: +1 (toi da MAX_B_DAMAGE) */
    if (s->bulletDamage < MAX_B_DAMAGE)
      s->bulletDamage++;
    break;
  case 2: /* Nang cap toc ban: giam cooldown 15ms (nhanh hon) */
    s->shootCooldown -= 15;
    if (s->shootCooldown < MIN_SHOOT_CD)
      s->shootCooldown = MIN_SHOOT_CD; /* Khong giam duoi muc toi thieu */
    break;
  }
  gItemCycle++; /* Tang bien toan cuc de lan sau nang cap thu tiep theo */
}

/*
 * handleBulletAsteroidCollisions(): Xu ly va cham DAN vs THIEN THACH.
 *        - Cong diem (nho=30, vua=20, lon=10, + bonus maxHp*5)
 *        - Tao hieu ung no (spawnExplosion)
 *        - 15% co hoi roi tinh the (spawnItem)
 *        - Respawn asteroid moi (initAsteroid)
 */
void handleBulletAsteroidCollisions(Bullet bul[], int nb, Asteroid ast[],
                                    int na, int *score, int level, Item items[],
                                    Explosion exps[]) {
  int i, j;
  for (i = 0; i < nb; i++) {
    if (!bul[i].active)
      continue;
    for (j = 0; j < na; j++) {
      if (!ast[j].active)
        continue;
      /* Kiem tra dan [i] va asteroid [j] co cham nhau khong */
      if (checkCircleCollision(bul[i].x, bul[i].y, (float)bul[i].radius,
                               ast[j].x, ast[j].y, (float)ast[j].radius)) {
        ast[j].hp -= bul[i].damage; /* Tru mau asteroid */
        bul[i].active = 0;          /* Tat dan (1 dan chi trung 1 asteroid) */

        if (ast[j].hp <= 0) { /* Asteroid het mau: no */
          /* Diem thuong: asteroid nho hon duoc nhieu diem hon */
          int pts = 10;
          if (ast[j].radius <= 22)
            pts = 30;
          else if (ast[j].radius <= 35)
            pts = 20;
          *score += pts + ast[j].maxHp * 5; /* Bonus diem theo HP toi da */

          spawnExplosion(exps, MAX_EXPLOSIONS, ast[j].x, ast[j].y,
                         (float)ast[j].radius);
          /* ITEM_DROP_CHANCE% xac suat roi tinh the */
          if (rand() % 100 < ITEM_DROP_CHANCE)
            spawnItem(items, MAX_ITEMS, ast[j].x, ast[j].y);
          initAsteroid(&ast[j], level, &gCfg); /* Tao asteroid moi thay the */
        }
        break; /* 1 vien dan chi trung 1 asteroid, ngat vong lap asteroid */
      }
    }
  }
}

/*
 * handleShipAsteroidCollision(): Xu ly va cham PHI THUYEN vs THIEN THACH.
 */
void handleShipAsteroidCollision(Ship *s, Asteroid ast[], int na, int *hp,
                                 unsigned long *lastHit, Explosion exps[],
                                 int level) {
  int j;
  /* GetTickCount(): lay thoi gian hien tai de so sanh voi lastHit */
  unsigned long now = GetTickCount();
  for (j = 0; j < na; j++) {
    if (!ast[j].active)
      continue;
    /* Kiem tra ship cham asteroid j */
    if (checkCircleCollision(s->x, s->y, (float)s->radius, ast[j].x, ast[j].y,
                             (float)ast[j].radius)) {
      /* HIT_COOLDOWN (500ms): tranh bi tru mau LIEN TUC khi chong len asteroid
       */
      if (now - *lastHit >= HIT_COOLDOWN) {
        *hp -= HIT_DAMAGE; /* Tru 12 HP moi lan bi dam */
        if (*hp < 0)
          *hp = 0;      /* HP khong xuong duoi 0 */
        *lastHit = now; /* Ghi lai thoi diem vua bi dam */
      }
      /* No nho tai vi tri asteroid, sau do reset asteroid len tren */
      spawnExplosion(exps, MAX_EXPLOSIONS, ast[j].x, ast[j].y,
                     (float)ast[j].radius * 0.5f);
      initAsteroid(&ast[j], level, &gCfg);
    }
  }
}

/* handleItemPickup(): Kiem tra ship cham tinh the va nhat len.
 */
void handleItemPickup(Ship *s, Item it[], int n) {
  int i;
  for (i = 0; i < n; i++) {
    if (!it[i].active)
      continue;
    /* Va cham hinh tron: ship cham tinh the -> nhat */
    if (checkCircleCollision(s->x, s->y, (float)s->radius, it[i].x, it[i].y,
                             (float)it[i].radius)) {
      applyItemToShip(s); /* Nang cap ship theo vong xoay */
      it[i].active = 0;   /* Tat item: bien mat khoi man hinh */
    }
  }
}

/*
 * applySkillDamage(): Ap dung sat thuong cua skill len asteroid.

 */
void applySkillDamage(SkillSystem *sk, Asteroid ast[], int na, int *score,
                      int level, Item items[], Explosion exps[]) {
  int j;
  /* Bomb Burst: gay damage vung tron.
   * Co bombDmgDone dam bao dame chi xay ra 1 lan khi skill bat dau
   * (bombFrame=0), du bomb hieu ung van con hien 22 frame. */
  if (sk->bombActive && !sk->bombDmgDone) {
    sk->bombDmgDone =
        1; /* Danh dau da ap dung, khong damage them lan nao nua */
    for (j = 0; j < na; j++) {
      if (!ast[j].active)
        continue;
      /* Kiem tra asteroid co nam trong vung no (bombRadius) khong */
      if (checkCircleCollision(sk->bombX, sk->bombY, sk->bombRadius, ast[j].x,
                               ast[j].y, (float)ast[j].radius)) {
        ast[j].hp -= sk->bombDmg; /* Tru 8 diem mau */
        if (ast[j].hp <= 0) {
          int pts = 10;
          if (ast[j].radius <= 22)
            pts = 30;
          else if (ast[j].radius <= 35)
            pts = 20;
          *score += pts + ast[j].maxHp * 5;
          spawnExplosion(exps, MAX_EXPLOSIONS, ast[j].x, ast[j].y,
                         (float)ast[j].radius);
          if (rand() % 100 < ITEM_DROP_CHANCE)
            spawnItem(items, MAX_ITEMS, ast[j].x, ast[j].y);
          initAsteroid(&ast[j], level, &gCfg);
        }
      }
    }
  }
  /* Piercing Beam: sat thuong moi asteroid tren duong tia.
   * beamHitsCircle(): kiem tra tia (tu beamSX/SY theo huong beamDX/DY) co cat
   * qua asteroid. Tuong tu Bomb, co beamDmgDone de damage chi 1 lan khi beam
   * bat dau. */
  if (sk->beamActive && !sk->beamDmgDone) {
    sk->beamDmgDone = 1;
    for (j = 0; j < na; j++) {
      if (!ast[j].active)
        continue;
      /* Kiem tra asteroid co nam tren duong tia beam khong */
      if (beamHitsCircle(sk->beamSX, sk->beamSY, sk->beamDX, sk->beamDY,
                         ast[j].x, ast[j].y, (float)ast[j].radius)) {
        ast[j].hp -= sk->beamDmg; /* Tru 12 diem mau moi asteroid trung tia */
        if (ast[j].hp <= 0) {
          int pts = 10;
          if (ast[j].radius <= 22)
            pts = 30;
          else if (ast[j].radius <= 35)
            pts = 20;
          *score += pts + ast[j].maxHp * 5;
          spawnExplosion(exps, MAX_EXPLOSIONS, ast[j].x, ast[j].y,
                         (float)ast[j].radius);
          if (rand() % 100 < ITEM_DROP_CHANCE)
            spawnItem(items, MAX_ITEMS, ast[j].x, ast[j].y);
          initAsteroid(&ast[j], level, &gCfg);
        }
      }
    }
  }
}

/* checkLevelUp(): Kim tra dieu kien len level.
 */
int checkLevelUp(int score, int level) {
  return (score >= level * LEVEL_UP_SCORE) ? 1 : 0;
}

#include "background_level.h"

/* --- Phi thuyen --- */

/* drawShipBody(): Ve than phi thuyen gom 3 phan: than chinh, canh trai, canh
 * phai va kinh lai.
 */
void drawShipBody(Ship s, float angle) {
  /* Tinh toa do dinh mui: cach tam mot khoang (radius + 14) theo huong angle */
  int nx = (int)(s.x + cosf(angle) * (s.radius + 14));
  int ny = (int)(s.y + sinf(angle) * (s.radius + 14));
  /* 2 dinh phia sau: lech goc +/-2.5 rad so voi angle */
  int lx = (int)(s.x + cosf(angle + 2.5f) * s.radius);
  int ly = (int)(s.y + sinf(angle + 2.5f) * s.radius);
  int rx = (int)(s.x + cosf(angle - 2.5f) * s.radius);
  int ry = (int)(s.y + sinf(angle - 2.5f) * s.radius);

  /* Than chinh: tam giac CYAN to dau */
  setfillstyle(SOLID_FILL, CYAN); /* Kieu to dac, mau CYAN */
  int body[] = {nx, ny, lx, ly, rx, ry, nx, ny};
  fillpoly(4, body); /* fillpoly(so_dinh, mang_toa_do): ve da giac da to mau */
  setcolor(WHITE);   /* Dat mau ve vien la WHITE */
  line(nx, ny, lx, ly);
  line(nx, ny, rx, ry);
  line(lx, ly, rx, ry); /* Ve vien */

  /* Canh trai: tam giac BLUE nho ben canh */
  {
    int w1x = (int)(s.x + cosf(angle + 2.0f) * s.radius * 0.7f);
    int w1y = (int)(s.y + sinf(angle + 2.0f) * s.radius * 0.7f);
    int w2x = (int)(s.x + cosf(angle + 1.8f) * (s.radius + 12));
    int w2y = (int)(s.y + sinf(angle + 1.8f) * (s.radius + 12));
    int w3x = (int)(s.x + cosf(angle + 2.8f) * s.radius * 1.1f);
    int w3y = (int)(s.y + sinf(angle + 2.8f) * s.radius * 1.1f);
    setfillstyle(SOLID_FILL, BLUE);
    int wL[] = {w1x, w1y, w2x, w2y, w3x, w3y, w1x, w1y};
    fillpoly(4, wL);
    setcolor(LIGHTCYAN);
    line(w1x, w1y, w2x, w2y);
    line(w2x, w2y, w3x, w3y);
  }
  /* Canh phai: doi xung voi canh trai (goc tru thay vi cong) */
  {
    int w1x = (int)(s.x + cosf(angle - 2.0f) * s.radius * 0.7f);
    int w1y = (int)(s.y + sinf(angle - 2.0f) * s.radius * 0.7f);
    int w2x = (int)(s.x + cosf(angle - 1.8f) * (s.radius + 12));
    int w2y = (int)(s.y + sinf(angle - 1.8f) * (s.radius + 12));
    int w3x = (int)(s.x + cosf(angle - 2.8f) * s.radius * 1.1f);
    int w3y = (int)(s.y + sinf(angle - 2.8f) * s.radius * 1.1f);
    setfillstyle(SOLID_FILL, BLUE);
    int wR[] = {w1x, w1y, w2x, w2y, w3x, w3y, w1x, w1y};
    fillpoly(4, wR);
    setcolor(LIGHTCYAN);
    line(w1x, w1y, w2x, w2y);
    line(w2x, w2y, w3x, w3y);
  }
  /* Kinh lai: hinh ellipse nho phia truoc */
  {
    int cx = (int)(s.x + cosf(angle) * s.radius * 0.35f);
    int cy = (int)(s.y + sinf(angle) * s.radius * 0.35f);
    setcolor(LIGHTCYAN);
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    fillellipse(cx, cy, 5, 4); /* fillellipse(cx,cy,rx,ry): ve ellipse to mau */
  }
}

/* drawShipEngineFlame(): Ve luoi lua dong co phia sau phi thuyen.
 */
void drawShipEngineFlame(Ship s, float angle, int frame) {
  float fLen = 10.0f + (frame % 6) * 2.5f;
  float hW = 5.0f + (frame % 4);
  float bx = s.x - cosf(angle) * s.radius * 0.5f;
  float by = s.y - sinf(angle) * s.radius * 0.5f;
  float perp = angle + (float)M_PI / 2.0f;

  int tx = (int)(s.x - cosf(angle) * (s.radius * 0.5f + fLen));
  int ty = (int)(s.y - sinf(angle) * (s.radius * 0.5f + fLen));
  int e1x = (int)(bx + cosf(perp) * hW);
  int e1y = (int)(by + sinf(perp) * hW);
  int e2x = (int)(bx - cosf(perp) * hW);
  int e2y = (int)(by - sinf(perp) * hW);

  setfillstyle(SOLID_FILL, (frame % 3 == 0) ? LIGHTRED : RED);
  int fl[] = {e1x, e1y, tx, ty, e2x, e2y, e1x, e1y};
  fillpoly(4, fl);

  /* Lop trong vang */
  float iL = fLen * 0.55f, iW = hW * 0.45f;
  int itx = (int)(s.x - cosf(angle) * (s.radius * 0.5f + iL));
  int ity = (int)(s.y - sinf(angle) * (s.radius * 0.5f + iL));
  int i1x = (int)(bx + cosf(perp) * iW);
  int i1y = (int)(by + sinf(perp) * iW);
  int i2x = (int)(bx - cosf(perp) * iW);
  int i2y = (int)(by - sinf(perp) * iW);
  setfillstyle(SOLID_FILL, YELLOW);
  int fi[] = {i1x, i1y, itx, ity, i2x, i2y, i1x, i1y};
  fillpoly(4, fi);
}

/* drawShip(): Ve toan bo phi thuyen (lua, than, duong ngam, crosshair).
 */
void drawShip(Ship s, int mx, int my, int frame) {
  /* atan2f(y, x): tinh goc cua vector (mx-sx, my-sy), biet huong chuot so voi
   * ship */
  float angle = atan2f((float)(my - s.y), (float)(mx - s.x));
  drawShipEngineFlame(s, angle, frame); /* Ve lua truoc (nam phia duoi than) */
  drawShipBody(s, angle);               /* Ve than tau len tren lua */

  /* Duong ngam: duong net dut tu ship den 80px ve huong chuot */
  setcolor(DARKGRAY);
  setlinestyle(DASHED_LINE, 0, 1); /* Kieu duong net dut */
  float dx = (float)(mx - s.x), dy = (float)(my - s.y);
  float ln = sqrtf(dx * dx + dy * dy);
  if (ln > 1.0f) {
    int ax = (int)(s.x + dx / ln * 80); /* Diem cuoi duong ngam (80px tu tam) */
    int ay = (int)(s.y + dy / ln * 80);
    line((int)s.x, (int)s.y, ax, ay); /* line(x1,y1,x2,y2): ve doan thang */
  }
  setlinestyle(SOLID_LINE, 0, 1); /* Dat lai kieu duong ke la lien tuc */

  /* Crosshair tai vi tri chuot: vong tron + duong cheo */
  setcolor(LIGHTGREEN);
  circle(mx, my, 8); /* circle(x,y,r): ve duong tron tam (mx,my) ban kinh 8 */
  line(mx - 12, my, mx + 12, my); /* Duong ngang */
  line(mx, my - 12, mx, my + 12); /* Duong doc */
}

// --- Dan ---
void drawBullets(Bullet b[], int n) {
  int i;
  for (i = 0; i < n; i++) {
    if (!b[i].active)
      continue;
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse((int)b[i].x, (int)b[i].y, b[i].radius,
                b[i].radius); /* Ve hinh tron dan */
  }
}

/* --- Thien thach --- */

void drawAsteroid(Asteroid a) {
  int nV = 10, i; /* 10 dinh polygon */
  int poly[20];   /* Mang toa do: [x0,y0, x1,y1, ..., x9,y9] */
  unsigned int s;
  int cFill, cLine;

  if (!a.active)
    return;

  s = (unsigned int)a.seed;
  for (i = 0; i < nV; i++) {
    /* va: goc cua dinh thu i, gom goc xoay (rot) va goc phan bo deu quanh tron
     */
    float va = a.rot + (float)i / nV * 2.0f * (float)M_PI;
    s = seedRand(s, i); /* Tao so ngau nhien on dinh tu seed va chi so dinh */
    /* rv: he so lech ban kinh (0.75..1.25) tao hinh khong tron deu */
    float rv = 0.75f + (float)(s % 50) / 100.0f;
    poly[i * 2] = (int)(a.x + cosf(va) * a.radius * rv); /* Toa do X dinh i */
    poly[i * 2 + 1] =
        (int)(a.y + sinf(va) * a.radius * rv); /* Toa do Y dinh i */
  }

  /* Chon mau theo kich thuoc asteroid */
  if (a.radius <= 22) {
    cFill = DARKGRAY;
    cLine = LIGHTGRAY;
  } /* Nho: xam */
  else if (a.radius <= 35) {
    cFill = RED;
    cLine = LIGHTRED;
  } /* Vua: do */
  else {
    cFill = BROWN;
    cLine = YELLOW;
  } /* Lon: nau */

  setfillstyle(SOLID_FILL, cFill);
  fillpoly(nV, poly); /* fillpoly(n, arr): ve va to mau da giac n dinh */
  setcolor(cLine);
  /* Ve vien polygon bang cach noi cac dinh lien tiep */
  for (i = 0; i < nV; i++) {
    int nx = (i + 1) % nV;
    line(poly[i * 2], poly[i * 2 + 1], poly[nx * 2], poly[nx * 2 + 1]);
  }
  /* Crater */
  {
    int cx = (int)(a.x + cosf(a.rot + 1.0f) * a.radius * 0.3f);
    int cy = (int)(a.y + sinf(a.rot + 1.0f) * a.radius * 0.3f);
    setcolor(cLine);
    circle(cx, cy, a.radius / 5);
  }
}

/* Thanh mau tren dau asteroid */
void drawAsteroidHPBar(Asteroid a) {
  if (!a.active || a.maxHp <= 1)
    return; /* Khong ve neu asteroid 1HP (khong can thanh) */
  int bW = a.radius * 2, bH = 4; /* Chieu rong = duong kinh, cao = 4px */
  int x1 = (int)(a.x - bW / 2);  /* Goc trai tren cua thanh */
  int y1 = (int)(a.y - a.radius - 10);
  int fillW = (int)(bW * (float)a.hp / a.maxHp); /* Chieu rong phan HP con */
  if (fillW < 0)
    fillW = 0;

  setcolor(DARKGRAY);
  rectangle(x1, y1, x1 + bW, y1 + bH); /* Khung ngoai thanh HP */
  /* Chon mau theo ti le HP con lai */
  if (a.hp * 2 > a.maxHp)
    setfillstyle(SOLID_FILL, GREEN); /* >50%: xanh */
  else if (a.hp * 4 > a.maxHp)
    setfillstyle(SOLID_FILL, YELLOW); /* >25%: vang */
  else
    setfillstyle(SOLID_FILL, RED);              /* <=25%: do  */
  bar(x1 + 1, y1 + 1, x1 + fillW, y1 + bH - 1); /* To phan HP con lai */
}

/* drawAsteroids(): Ve tat ca asteroid dang active cung voi thanh HP cua chung.
 */
void drawAsteroids(Asteroid a[], int n) {
  int i;
  for (i = 0; i < n; i++) {
    drawAsteroid(a[i]);      /* Ve hinh dang asteroid */
    drawAsteroidHPBar(a[i]); /* Ve thanh HP phia tren (neu can) */
  }
}

/* --- Vat pham: Tinh the suc manh (lap lanh + bieu tuong set) --- */
void drawItems(Item it[], int n) {
  int i;
  for (i = 0; i < n; i++) {
    if (!it[i].active)
      continue;
    it[i].sparkle++; /* Tang dem frame moi frame de tao hieu ung dong */

    int cx = (int)it[i].x, cy = (int)it[i].y;
    int r = it[i].radius;

    /* --- Hinh kim cuong (diamond) --- */
    int dia[] = {cx,     cy - r,  /* dinh tren */
                 cx + r, cy,      /* phai */
                 cx,     cy + r,  /* duoi */
                 cx - r, cy,      /* trai */
                 cx,     cy - r}; /* dong */

    /* Mau lap lanh xen ke LIGHTCYAN / LIGHTMAGENTA */
    int glow = (it[i].sparkle / 5) % 2;
    setfillstyle(SOLID_FILL, glow ? LIGHTCYAN : LIGHTMAGENTA);
    fillpoly(5, dia);

    /* Vien sang */
    setcolor(WHITE);
    line(cx, cy - r, cx + r, cy);
    line(cx + r, cy, cx, cy + r);
    line(cx, cy + r, cx - r, cy);
    line(cx - r, cy, cx, cy - r);

    /* --- Bieu tuong tia set (lightning bolt) ben trong --- */
    setcolor(YELLOW);
    /* Net set don gian: 4 doan thang tao hinh zig-zag */
    line(cx - 2, cy - r + 4, cx + 2, cy - 2);
    line(cx + 2, cy - 2, cx - 2, cy + 1);
    line(cx - 2, cy + 1, cx + 2, cy + r - 4);

    /* --- Tia sparkle xoay quanh --- */
    {
      float sa = it[i].sparkle * 0.15f; /* Goc xoay tang dan moi frame */
      int k;
      for (k = 0; k < 4; k++) {
        /* Tinh diem dau/cuoi tia: cach tam (r+3) va (r+7) pixel */
        float a = sa + k * (float)M_PI / 2.0f; /* 4 tia phÃ¢n bo vai 90 do */
        int sx1 = cx + (int)(cosf(a) * (r + 3));
        int sy1 = cy + (int)(sinf(a) * (r + 3));
        int sx2 = cx + (int)(cosf(a) * (r + 7));
        int sy2 = cy + (int)(sinf(a) * (r + 7));
        setcolor((k + it[i].sparkle / 3) % 2 ? WHITE : YELLOW);
        line(sx1, sy1, sx2, sy2); /* Ve tia sang nho xung quanh */
      }
    }
  }
}

/* --- Hieu ung no --- */

void drawExplosions(Explosion e[], int n) {
  int i;
  for (i = 0; i < n; i++) {
    if (!e[i].active)
      continue;
    float p = (float)e[i].frame / e[i].maxFrames; /* Ti le hoan thanh (0..1) */
    float r = e[i].maxRadius * p; /* Ban kinh tang dan theo tien trinh */
    int c;
    /* Mau: vang -> do nhat -> do -> xam (no dan, nguoi lan) */
    if (p < 0.25f)
      c = YELLOW;
    else if (p < 0.5f)
      c = LIGHTRED;
    else if (p < 0.75f)
      c = RED;
    else
      c = DARKGRAY;
    setcolor(c);
    circle((int)e[i].x, (int)e[i].y, (int)r); /* Vong ngoai */
    if (r > 5)
      circle((int)e[i].x, (int)e[i].y, (int)(r * 0.6f)); /* Vong giua */
    if (r > 10)
      circle((int)e[i].x, (int)e[i].y, (int)(r * 0.3f)); /* Vong trong */
  }
}

/* --- Hieu ung skill --- */
void drawSkillEffects(SkillSystem *sk) {
  /* Bomb Burst: vong tron no */
  if (sk->bombActive) {
    float p = (float)sk->bombFrame / 22.0f; /* Ti le phi (0..1) */
    float r = sk->bombRadius * p;           /* Ban kinh hien tai */
    int c;
    if (p < 0.3f)
      c = YELLOW;
    else if (p < 0.6f)
      c = LIGHTRED;
    else
      c = RED;
    setcolor(c);
    circle((int)sk->bombX, (int)sk->bombY, (int)r);          /* Vong ngoai */
    circle((int)sk->bombX, (int)sk->bombY, (int)(r * 0.7f)); /* Vong giua */
    if (r > 20)
      circle((int)sk->bombX, (int)sk->bombY, (int)(r * 0.4f));
  }
  /* Piercing Beam: tia sang xuyen man hinh */
  if (sk->beamActive) {
    float endX = sk->beamSX + sk->beamDX * 1500; /* Keo tia dai 1500px */
    float endY = sk->beamSY + sk->beamDY * 1500;
    /* Ve tia day 3px mau CYAN, sau do tia mong 1px mau WHITE len tren */
    setcolor(LIGHTCYAN);
    setlinestyle(SOLID_LINE, 0, 3); /* setlinestyle(kieu, pattern, do_day) */
    line((int)sk->beamSX, (int)sk->beamSY, (int)endX, (int)endY);
    setcolor(WHITE);
    setlinestyle(SOLID_LINE, 0, 1); /* Dat lai duong ke mong */
    line((int)sk->beamSX, (int)sk->beamSY, (int)endX, (int)endY);
  }
  /* Time Slow: chi bao tren HUD, khong co hieu ung hinh anh */
  if (sk->slowActive) {
    setcolor(LIGHTBLUE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2); /* Co chu 2 */
    outtextxy(WIDTH / 2 - 70, HEIGHT - 40,
              (char *)"~ SLOW ACTIVE ~");     /* In chu tai toa do */
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1); /* Reset co chu ve 1 */
  }
}

/* drawHUD(): Ve giao dien nguoi choi: HP, Level, Score, chi so dan, cooldown
 * skill.
 */
void drawHUD(int score, int hp, int level, Ship ship, SkillSystem *sk) {
  char buf[80];
  unsigned long now =
      GetTickCount(); /* Lay thoi gian hien tai de tinh CD con lai */

  /* HP bar */
  setcolor(WHITE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1); /* Font mac dinh, ngang, co 1 */
  outtextxy(15, 12, (char *)"HP:");         /* In nhan "HP:" tai (15,12) */
  rectangle(50, 10, 260, 28);               /* Khung trang bar HP */
  /* Tinh chieu rong phan HP con lai: ty le voi HP toi da */
  int bw = (int)(210.0f * hp / INITIAL_HP);
  if (bw < 0)
    bw = 0;
  /* Mau HP: xanh (khoe), vang (trung binh), do (nguy hiem) */
  if (hp > 60)
    setfillstyle(SOLID_FILL, GREEN);
  else if (hp > 30)
    setfillstyle(SOLID_FILL, YELLOW);
  else
    setfillstyle(SOLID_FILL, RED);
  bar(51, 11, 51 + bw, 27); /* bar(): to mau vung HCN the hien HP */
  /* sprintf: ghi "hp/INITIAL_HP" vao buf, sau do outtextxy in ra */
  sprintf(buf, "%d/%d", hp, INITIAL_HP);
  setcolor(WHITE);
  outtextxy(268, 12, buf); /* In gia tri HP dang so */

  /* Level: hien thi giua tren man hinh */
  setcolor(LIGHTCYAN);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2); /* Co chu lon hon binh thuong */
  sprintf(buf, "LEVEL %d", level);
  outtextxy(WIDTH / 2 - 55, 8, buf); /* Giua man hinh, sat top */

  /* Score: hien thi goc tren phai */
  setcolor(YELLOW);
  sprintf(buf, "SCORE: %d", score);
  outtextxy(WIDTH - 240, 8, buf);

  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1); /* Reset co chu ve 1 */

  /* Weapon stats (duoi trai): hien thi sat thuong, so vien, cooldown hien tai
   */
  setcolor(LIGHTGREEN);
  sprintf(buf, "DMG:%d  PELLETS:%d  CD:%dms", ship.bulletDamage,
          ship.pelletsPerShot, ship.shootCooldown);
  outtextxy(15, HEIGHT - 50, buf);

  /* Score de len level */
  setcolor(DARKGRAY);
  sprintf(buf, "Next Lv: %d", level * LEVEL_UP_SCORE);
  outtextxy(15, HEIGHT - 30, buf); /* Goc duoi trai */

  /* Skill cooldown (duoi phai) */
  {
    int sx = WIDTH - 360, sy = HEIGHT - 80;
    float cd;
    int color;

    /* Skill 1 â€“ Bomb: tinh so giay CD con lai, xanh=san sang, do=dang CD */
    cd = 0;
    if (now - sk->bombLast < sk->bombCD)
      cd =
          (float)(sk->bombCD - (now - sk->bombLast)) / 1000.0f; /* ms -> giay */
    color = (cd <= 0) ? GREEN : RED;
    setcolor(color);
    if (cd <= 0)
      sprintf(buf, "[1] BOMB: Ready");
    else
      sprintf(buf, "[1] BOMB: %.1fs", cd); /* %.1f: 1 chu so thap phan */
    outtextxy(sx, sy, buf);

    /* Skill 2 â€“ Slow */
    cd = 0;
    if (now - sk->slowLast < sk->slowCD)
      cd = (float)(sk->slowCD - (now - sk->slowLast)) / 1000.0f;
    if (sk->slowActive) {
      color = YELLOW;
      sprintf(buf, "[2] SLOW: Active");
    } else if (cd <= 0) {
      color = GREEN;
      sprintf(buf, "[2] SLOW: Ready");
    } else {
      color = RED;
      sprintf(buf, "[2] SLOW: %.1fs", cd);
    }
    setcolor(color);
    outtextxy(sx, sy + 16, buf);

    /* Skill 3 â€“ Beam */
    cd = 0;
    if (now - sk->beamLast < sk->beamCD)
      cd = (float)(sk->beamCD - (now - sk->beamLast)) / 1000.0f;
    color = (cd <= 0) ? GREEN : RED;
    setcolor(color);
    if (cd <= 0)
      sprintf(buf, "[3] BEAM: Ready");
    else
      sprintf(buf, "[3] BEAM: %.1fs", cd);
    outtextxy(sx, sy + 32, buf);
  }
}

/* --- Level Banner --- */

void drawLevelBanner(int level) {
  char buf[32];
  int tw, th;
  setfillstyle(SOLID_FILL, BLUE);
  bar(WIDTH / 2 - 200, HEIGHT / 2 - 60, WIDTH / 2 + 200,
      HEIGHT / 2 + 40); /* Nen banner */
  setcolor(LIGHTCYAN);
  rectangle(WIDTH / 2 - 200, HEIGHT / 2 - 60, WIDTH / 2 + 200,
            HEIGHT / 2 + 40); /* Vien */
  rectangle(WIDTH / 2 - 202, HEIGHT / 2 - 62, WIDTH / 2 + 202,
            HEIGHT / 2 + 42); /* Vien doi */
  setcolor(YELLOW);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
  tw = textwidth((char *)"LEVEL UP!");
  th = textheight((char *)"LEVEL UP!");
  outtextxy(WIDTH / 2 - tw / 2, HEIGHT / 2 - 50 - th / 2, (char *)"LEVEL UP!");
  setcolor(WHITE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  sprintf(buf, "Level %d", level);
  tw = textwidth(buf);
  outtextxy(WIDTH / 2 - tw / 2, HEIGHT / 2 + 5, buf);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* pointInRect(): Kiem tra xem diem (x, y) co nam trong hinh chu nhat hay khong.
 * Tra ve 1 neu nam trong, 0 neu o ngoai. Dung de: kiem tra click chuot vao nut.
 */
int pointInRect(int x, int y, int x1, int y1, int x2, int y2) {
  return (x >= x1 && x <= x2 && y >= y1 && y <= y2) ? 1 : 0;
}

#include "menu.h"
#include "introlduction.h"
#include "pause.h"
#include "game_over.h"


/* drawMenuButton(): Ve nut hamburger (3 gach ngang) goc trai tren.
. */
void drawMenuButton(int hovered) {
  /* Nen nut */
  setfillstyle(SOLID_FILL, hovered ? DARKGRAY : BLACK);
  bar(MENU_BTN_X1, MENU_BTN_Y1, MENU_BTN_X2, MENU_BTN_Y2);
  /* Vien */
  setcolor(hovered ? WHITE : LIGHTGRAY);
  rectangle(MENU_BTN_X1, MENU_BTN_Y1, MENU_BTN_X2, MENU_BTN_Y2);
  /* 3 gach ngang (hamburger icon) */
  int cx = (MENU_BTN_X1 + MENU_BTN_X2) / 2;
  int cy = (MENU_BTN_Y1 + MENU_BTN_Y2) / 2;
  setcolor(hovered ? YELLOW : WHITE);
  line(cx - 12, cy - 7, cx + 12, cy - 7);
  line(cx - 12, cy, cx + 12, cy);
  line(cx - 12, cy + 7, cx + 12, cy + 7);
}

int main() {

  Ship ship;
  Bullet bullets[MAX_BULLETS];
  Asteroid asteroids[MAX_ASTEROIDS];
  Item items[MAX_ITEMS];
  Explosion explosions[MAX_EXPLOSIONS];
  SkillSystem skills;

  int score = 0, hp = INITIAL_HP, level = 1;
  unsigned long lastShot = 0, lastHit = 0;
  int mx = WIDTH / 2, my = HEIGHT / 2;
  int frameCount = 0, levelBanner = 0, page = 0;
  int mxClick, myClick;

  GameState gameState = STATE_MAIN_MENU; /* Bat dau tu Main Menu */
  int running = 1;                       /* Vong lap chinh cua chuong trinh */

  int gd = DETECT, gm;
  initwindow(WIDTH, HEIGHT, "Asteroid Blaster v3.0");
  setbkcolor(BLACK);
  cleardevice();

  /* Khoi tao sao nen 1 lan */
  if (!starsReady)
    initStars();

  while (running) {
    unsigned long now = GetTickCount();
    handleMouse(&mx, &my);

    /* Xu ly click chuot */
    if (ismouseclick(WM_LBUTTONDOWN)) {
      getmouseclick(WM_LBUTTONDOWN, mxClick, myClick);

      switch (gameState) {
      case STATE_MAIN_MENU: {
        int choice = handleMainMenuClick(mxClick, myClick);
        if (choice == 1) {
          /* START GAME: khoi tao game va chuyen sang PLAYING */
          initGame(&ship, asteroids, bullets, items, explosions, &skills,
                   &score, &hp, &level);
          gameState = STATE_PLAYING;
          frameCount = 0;
          levelBanner = 0;
        } else if (choice == 2) {
          /* GUIDE: chuyen sang man hinh huong dan */
          gameState = STATE_GUIDE;
        } else if (choice == 3) {
          /* EXIT: thoat chuong trinh */
          running = 0;
        }
        break;
      }

      case STATE_GUIDE: {
        int back = handleGuideClick(mxClick, myClick);
        if (back == 1) {
          /* BACK: quay lai Main Menu */
          gameState = STATE_MAIN_MENU;
        }
        break;
      }

      case STATE_PAUSED: {
        int choice = handlePauseMenuClick(mxClick, myClick);
        if (choice == 1) {
          /* CONTINUE: tiep tuc choi */
          gameState = STATE_PLAYING;
        } else if (choice == 2) {
          /* EXIT: quay lai Main Menu */
          gameState = STATE_MAIN_MENU;
        }
        break;
      }

      case STATE_GAME_OVER: {
        int choice = handleGameOverClick(mxClick, myClick);
        if (choice == 1) {
          /* REPLAY: khoi tao lai game va choi lai */
          initGame(&ship, asteroids, bullets, items, explosions, &skills,
                   &score, &hp, &level);
          gameState = STATE_PLAYING;
          frameCount = 0;
          levelBanner = 0;
        } else if (choice == 2) {
          /* EXIT TO MENU: quay lai Main Menu */
          gameState = STATE_MAIN_MENU;
        }
        break;
      }

      case STATE_PLAYING:
        /* Kiem tra click nut MENU trong game */
        if (pointInRect(mxClick, myClick, MENU_BTN_X1, MENU_BTN_Y1, MENU_BTN_X2,
                        MENU_BTN_Y2)) {
          gameState = STATE_PAUSED;
        }
        break;
      }
    }

    /* Xu ly phim ESC trong game de pause */
    if (gameState == STATE_PLAYING) {
      if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        gameState = STATE_PAUSED;
        delay(200);
      }
    } else if (gameState == STATE_PAUSED) {
      if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        gameState = STATE_PLAYING;
        delay(200);
      }
    }

    /* Xu ly logic va render theo trang thai */
    switch (gameState) {
    case STATE_MAIN_MENU:
      setactivepage(0);
      setvisualpage(0);
      drawMainMenu(mx, my);
      delay(16);
      break;

    case STATE_GUIDE:
      setactivepage(0);
      setvisualpage(0);
      drawGuideScreen(mx, my);
      delay(16);
      break;

    case STATE_PLAYING: {
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
      if (checkLevelUp(score, level)) {
        level++;
        updateDifficultyByLevel(level, &gCfg);
        levelBanner = LEVEL_BANNER_FR;

        {
          int i;
          for (i = 0; i < MAX_ASTEROIDS; i++) {
            initAsteroid(&asteroids[i], level, &gCfg);
            asteroids[i].y = (float)(-(rand() % 300) - 200);
          }
        }
      }

      if (hp <= 0) {
        gameState = STATE_GAME_OVER; /* Chuyen sang man hinh Game Over */
      }

      /* BUOC 6: RENDER */
      setactivepage(page);
      cleardevice();

      drawBackground(level);
      drawAsteroids(asteroids, MAX_ASTEROIDS);
      drawItems(items, MAX_ITEMS);
      drawBullets(bullets, MAX_BULLETS);
      drawExplosions(explosions, MAX_EXPLOSIONS);
      drawSkillEffects(&skills);
      drawShip(ship, mx, my, frameCount);
      drawHUD(score, hp, level, ship, &skills);

      if (levelBanner > 0) {
        drawLevelBanner(level);
        levelBanner--;
      }

      {
        int btnHov = pointInRect(mx, my, MENU_BTN_X1, MENU_BTN_Y1, MENU_BTN_X2,
                                 MENU_BTN_Y2);
        drawMenuButton(btnHov);
      }

      setvisualpage(page);
      page = 1 - page;

      /* BUOC 7: DELAY */
      delay(16);
      break;
    }

    case STATE_PAUSED: {
      setactivepage(page);
      cleardevice();

      /* Ve lai game phia sau */
      drawBackground(level);
      drawAsteroids(asteroids, MAX_ASTEROIDS);
      drawItems(items, MAX_ITEMS);
      drawBullets(bullets, MAX_BULLETS);
      drawExplosions(explosions, MAX_EXPLOSIONS);
      drawSkillEffects(&skills);
      drawShip(ship, mx, my, frameCount);
      drawHUD(score, hp, level, ship, &skills);

      {
        int btnHov = pointInRect(mx, my, MENU_BTN_X1, MENU_BTN_Y1, MENU_BTN_X2,
                                 MENU_BTN_Y2);
        drawMenuButton(btnHov);
      }

      /* Ve menu pause len tren */
      drawPauseMenu(mx, my);

      setvisualpage(page);
      page = 1 - page;
      delay(16);
      break;
    }

    case STATE_GAME_OVER:
      setactivepage(0);
      setvisualpage(0);
      drawGameOverScreen(score, level, mx, my);
      delay(16);
      break;
    }
  }

  closegraph();
  return 0;
}
