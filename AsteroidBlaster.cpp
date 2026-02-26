/*
 * ============================================================
 *  ASTEROID BLASTER v3.0 – Ban Thien Thach (Ky Nang + Nang Cap)
 *  C/C++ – WinBGIm (graphics.h)  –  1200 x 800
 * ============================================================
 *
 *  COMPILE (Dev-C++ / CodeBlocks + WinBGIm):
 *  ------------------------------------------
 *  g++ AsteroidBlaster.cpp -o AsteroidBlaster.exe ^
 *      -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
 *
 *  DIEU KHIEN:
 *  -----------
 *  Arrow / WASD : di chuyen phi thuyen
 *  Chuot        : huong ngam (tu dong ban lien tuc)
 *  Phim 1       : Skill BOMB BURST  (no vong tron)
 *  Phim 2       : Skill TIME SLOW   (lam cham asteroid)
 *  Phim 3       : Skill PIERCING BEAM (tia xuyen)
 *  ESC          : thoat
 *
 * ============================================================
 */

#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>

/* ============================================================
 *  SECTION 1 – HANG SO
 * ============================================================ */

#define WIDTH            1200
#define HEIGHT           800
#define MAX_ASTEROIDS    30
#define MAX_BULLETS      80
#define MAX_ITEMS        12
#define MAX_EXPLOSIONS   35
#define MAX_STARS        120

#define SHIP_SPEED       5
#define SHIP_RADIUS      20
#define BULLET_SPEED     13
#define BULLET_RADIUS    3

#define DEFAULT_SHOOT_CD 120      /* ms ban dau */
#define MIN_SHOOT_CD     35       /* cooldown toi thieu */
#define HIT_COOLDOWN     500      /* ms */
#define INITIAL_HP       100
#define HIT_DAMAGE       12

#define MAX_PELLETS      7
#define MAX_B_DAMAGE     10

#define ITEM_DROP_CHANCE 15       /* % */
#define LEVEL_UP_SCORE   2000     /* score >= level * nay */
#define LEVEL_BANNER_FR  90       /* frame hien banner */

#define AST_MIN_R        15
#define AST_MAX_R        45
#define AST_SPD_MIN      1.0f
#define AST_SPD_MAX      3.5f

/* Vat pham: chi 1 loai – Tinh the suc manh (Power Crystal) */
static int gItemCycle = 0;       /* Bien xoay vong nang cap */

#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

/* ============================================================
 *  SECTION 2 – STRUCT
 * ============================================================ */

typedef struct {
    float x, y;
    int   radius, speed;
    /* Thong so dan */
    int   bulletDamage;       /* sat thuong moi vien */
    int   pelletsPerShot;     /* so vien / phat ban   */
    int   shootCooldown;      /* ms giua cac phat     */
} Ship;

typedef struct {
    float x, y, dx, dy;
    int   active, radius, damage;
} Bullet;

typedef struct {
    float x, y;
    float vx, vy;
    int   radius, active;
    int   seed;               /* polygon on dinh */
    float rot, rotSpeed;
    int   hp, maxHp;
} Asteroid;

typedef struct {
    float x, y, vy;
    int   active, radius;
    int   sparkle;            /* frame counter cho hieu ung lap lanh */
} Item;

typedef struct {
    float x, y, maxRadius;
    int   frame, maxFrames, active;
} Explosion;

/* --- He thong 3 ky nang --- */
typedef struct {
    /* Skill 1 – Bomb Burst */
    unsigned long bombCD;           /* tong CD (ms) */
    unsigned long bombLast;         /* thoi diem dung gan nhat */
    int   bombActive;               /* hieu ung dang chay? */
    int   bombFrame;
    float bombX, bombY, bombRadius;
    int   bombDmg, bombDmgDone;

    /* Skill 2 – Time Slow */
    unsigned long slowCD, slowLast, slowDur;
    int   slowActive;
    float slowFactor;

    /* Skill 3 – Piercing Beam */
    unsigned long beamCD, beamLast, beamDur;
    int   beamActive, beamDmg, beamDmgDone;
    float beamDX, beamDY, beamSX, beamSY;
} SkillSystem;

/* Cau hinh do kho (thay doi theo level) */
typedef struct {
    int   activeAsteroids;
    float astSpeedMin, astSpeedMax;
    float angleVariety;
    int   baseAstHp;              /* maxHp cua asteroid */
} GameConfig;

/* ============================================================
 *  SECTION 3 – BIEN TOAN CUC + TIEN ICH
 * ============================================================ */

static GameConfig gCfg;

/* Sao nen (khoi tao 1 lan) */
static int sX[MAX_STARS], sY[MAX_STARS], sB[MAX_STARS];
static int starsReady = 0;

/* Pseudo-random on dinh theo seed (dung ve asteroid) */
static unsigned int seedRand(unsigned int s, int i) {
    s = s * 1103515245u + 12345u + (unsigned int)i * 2654435761u;
    return s;
}

/* Kiem tra beam cat hinh tron */
static int beamHitsCircle(float sx, float sy, float dx, float dy,
                          float cx, float cy, float cr) {
    float vx = cx - sx, vy = cy - sy;
    float proj = vx * dx + vy * dy;
    if (proj < 0) return 0;
    float px = vx - proj * dx, py = vy - proj * dy;
    return (sqrtf(px * px + py * py) < cr) ? 1 : 0;
}

/* ============================================================
 *  SECTION 4 – KHOI TAO (Init)
 * ============================================================ */

void initStars(void) {
    int i;
    for (i = 0; i < MAX_STARS; i++) {
        sX[i] = rand() % WIDTH;
        sY[i] = rand() % HEIGHT;
        sB[i] = rand() % 3;
    }
    starsReady = 1;
}

void updateDifficultyByLevel(int level, GameConfig *cfg) {
    cfg->activeAsteroids = 8 + level * 2;
    if (cfg->activeAsteroids > MAX_ASTEROIDS)
        cfg->activeAsteroids = MAX_ASTEROIDS;
    cfg->astSpeedMin  = AST_SPD_MIN + level * 0.12f;
    cfg->astSpeedMax  = AST_SPD_MAX + level * 0.22f;
    cfg->angleVariety = 0.3f + level * 0.08f;
    if (cfg->angleVariety > 1.2f) cfg->angleVariety = 1.2f;
    cfg->baseAstHp = 1 + level / 2;
}

void initShip(Ship *s) {
    s->x = WIDTH / 2.0f;  s->y = HEIGHT - 80.0f;
    s->radius = SHIP_RADIUS;  s->speed = SHIP_SPEED;
    s->bulletDamage   = 1;
    s->pelletsPerShot = 1;
    s->shootCooldown  = DEFAULT_SHOOT_CD;
}

void initBullet(Bullet *b) {
    b->x = b->y = b->dx = b->dy = 0;
    b->active = 0; b->radius = BULLET_RADIUS; b->damage = 1;
}

void initAsteroid(Asteroid *a, int level, GameConfig *cfg) {
    float spd, mxA;
    a->radius = AST_MIN_R + rand() % (AST_MAX_R - AST_MIN_R + 1);
    a->x = (float)(a->radius + rand() % (WIDTH - 2 * a->radius));
    a->y = (float)(-(rand() % 400) - a->radius);

    spd = cfg->astSpeedMin + (rand() % 100) / 100.0f * (cfg->astSpeedMax - cfg->astSpeedMin);
    mxA = cfg->angleVariety;
    a->vy = spd;
    a->vx = ((rand() % 200 - 100) / 100.0f) * mxA * spd * 0.4f;

    a->seed     = rand();
    a->rot      = 0;
    a->rotSpeed = ((rand() % 100) - 50) / 1500.0f;
    a->maxHp    = cfg->baseAstHp;
    a->hp       = a->maxHp;
    a->active   = 1;
}

void initAllAsteroids(Asteroid ast[], int n, int level, GameConfig *cfg) {
    int i;
    for (i = 0; i < n; i++) {
        initAsteroid(&ast[i], level, cfg);
        if (i >= cfg->activeAsteroids) ast[i].active = 0;
    }
}

void initSkillSystem(SkillSystem *sk) {
    sk->bombCD = 8000;  sk->bombLast = 0;
    sk->bombActive = 0; sk->bombFrame = 0;
    sk->bombRadius = 160.0f; sk->bombDmg = 8;
    sk->bombDmgDone = 0;

    sk->slowCD = 12000; sk->slowLast = 0;
    sk->slowDur = 4000; sk->slowActive = 0;
    sk->slowFactor = 0.35f;

    sk->beamCD = 10000; sk->beamLast = 0;
    sk->beamDur = 450;  sk->beamActive = 0;
    sk->beamDmg = 12;   sk->beamDmgDone = 0;
}

void initGame(Ship *ship, Asteroid ast[], Bullet bul[],
              Item items[], Explosion exps[], SkillSystem *sk,
              int *score, int *hp, int *level) {
    int i;
    srand((unsigned int)time(NULL));
    *level = 1; *score = 0; *hp = INITIAL_HP;
    updateDifficultyByLevel(*level, &gCfg);
    initShip(ship);
    initAllAsteroids(ast, MAX_ASTEROIDS, *level, &gCfg);
    for (i = 0; i < MAX_BULLETS; i++) initBullet(&bul[i]);
    for (i = 0; i < MAX_ITEMS; i++) { items[i].active = 0; items[i].radius = 12; }
    for (i = 0; i < MAX_EXPLOSIONS; i++) exps[i].active = 0;
    initSkillSystem(sk);
    if (!starsReady) initStars();
}

/* ============================================================
 *  SECTION 5 – INPUT
 * ============================================================ */

void handleKeyboardMove(Ship *s) {
    if (GetAsyncKeyState(VK_LEFT)  & 0x8000 || GetAsyncKeyState('A') & 0x8000) s->x -= s->speed;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000) s->x += s->speed;
    if (GetAsyncKeyState(VK_UP)    & 0x8000 || GetAsyncKeyState('W') & 0x8000) s->y -= s->speed;
    if (GetAsyncKeyState(VK_DOWN)  & 0x8000 || GetAsyncKeyState('S') & 0x8000) s->y += s->speed;
}

void handleMouse(int *mx, int *my) {
    *mx = mousex(); *my = mousey();
}

/*
 * handleSkillsInput: kiem tra phim 1/2/3, kich hoat skill neu het CD
 */
void handleSkillsInput(SkillSystem *sk, Ship *ship,
                       int mx, int my, unsigned long now) {
    /* Skill 1 – Bomb Burst */
    if (GetAsyncKeyState('1') & 0x8000) {
        if (now - sk->bombLast >= sk->bombCD) {
            sk->bombLast     = now;
            sk->bombActive   = 1;
            sk->bombFrame    = 0;
            sk->bombX        = ship->x;
            sk->bombY        = ship->y;
            sk->bombDmgDone  = 0;
        }
    }
    /* Skill 2 – Time Slow */
    if (GetAsyncKeyState('2') & 0x8000) {
        if (now - sk->slowLast >= sk->slowCD) {
            sk->slowLast   = now;
            sk->slowActive = 1;
        }
    }
    /* Skill 3 – Piercing Beam */
    if (GetAsyncKeyState('3') & 0x8000) {
        if (now - sk->beamLast >= sk->beamCD) {
            float dx = mx - ship->x, dy = my - ship->y;
            float ln = sqrtf(dx * dx + dy * dy);
            if (ln < 1.0f) return;
            sk->beamLast    = now;
            sk->beamActive  = 1;
            sk->beamDmgDone = 0;
            sk->beamSX = ship->x; sk->beamSY = ship->y;
            sk->beamDX = dx / ln;  sk->beamDY = dy / ln;
        }
    }
}

/* ============================================================
 *  SECTION 6 – UPDATE
 * ============================================================ */

void updateShip(Ship *s) {
    if (s->x - s->radius < 0)      s->x = (float)s->radius;
    if (s->x + s->radius > WIDTH)   s->x = (float)(WIDTH - s->radius);
    if (s->y - s->radius < 0)      s->y = (float)s->radius;
    if (s->y + s->radius > HEIGHT)  s->y = (float)(HEIGHT - s->radius);
}

/* Tim slot dan trong */
static int freeBullet(Bullet b[], int n) {
    int i; for (i = 0; i < n; i++) if (!b[i].active) return i; return -1;
}

/*
 * shootPellets: tao N vien dan (pelletsPerShot) xoe nhe
 */
void shootPellets(Ship *ship, Bullet bul[], int n, float dirX, float dirY) {
    int p, idx;
    float base = atan2f(dirY, dirX);
    int   cnt  = ship->pelletsPerShot;
    /* spread nhe khi nhieu vien: tong ~0.08 rad/vien, cap 0.5 rad */
    float totalSpread = (cnt > 1) ? (cnt - 1) * 0.08f : 0;
    if (totalSpread > 0.5f) totalSpread = 0.5f;

    for (p = 0; p < cnt; p++) {
        float off = 0;
        if (cnt > 1) off = -totalSpread / 2.0f + totalSpread * p / (cnt - 1);
        float a = base + off;

        idx = freeBullet(bul, n);
        if (idx < 0) return;
        bul[idx].x  = ship->x;  bul[idx].y  = ship->y;
        bul[idx].dx = cosf(a) * BULLET_SPEED;
        bul[idx].dy = sinf(a) * BULLET_SPEED;
        bul[idx].active = 1;
        bul[idx].radius = BULLET_RADIUS;
        bul[idx].damage = ship->bulletDamage;
    }
}

void tryAutoShoot(Ship *ship, Bullet bul[], int n,
                  int mx, int my, unsigned long *lastShot) {
    unsigned long now = GetTickCount();
    if (now - *lastShot < (unsigned long)ship->shootCooldown) return;
    float dx = mx - ship->x, dy = my - ship->y;
    float ln = sqrtf(dx * dx + dy * dy);
    if (ln < 5.0f) return;
    shootPellets(ship, bul, n, dx / ln, dy / ln);
    *lastShot = now;
}

void updateBullets(Bullet b[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!b[i].active) continue;
        b[i].x += b[i].dx;  b[i].y += b[i].dy;
        if (b[i].x < -20 || b[i].x > WIDTH + 20 ||
            b[i].y < -20 || b[i].y > HEIGHT + 20)
            b[i].active = 0;
    }
}

void updateAsteroids(Asteroid a[], int n, int level, float spdMul) {
    int i;
    for (i = 0; i < n; i++) {
        if (!a[i].active) continue;
        a[i].x   += a[i].vx * spdMul;
        a[i].y   += a[i].vy * spdMul;
        a[i].rot += a[i].rotSpeed;
        if (a[i].y - a[i].radius > HEIGHT ||
            a[i].x + a[i].radius < -60 ||
            a[i].x - a[i].radius > WIDTH + 60) {
            initAsteroid(&a[i], level, &gCfg);
        }
    }
    /* Dam bao so asteroid active = cfg */
    for (i = 0; i < n && i < gCfg.activeAsteroids; i++)
        if (!a[i].active) initAsteroid(&a[i], level, &gCfg);
}

void updateItems(Item it[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!it[i].active) continue;
        it[i].y += it[i].vy;
        if (it[i].y > HEIGHT + 20) it[i].active = 0;
    }
}

void updateExplosions(Explosion e[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!e[i].active) continue;
        e[i].frame++;
        if (e[i].frame >= e[i].maxFrames) e[i].active = 0;
    }
}

/*
 * updateSkillEffects: quan ly thoi gian hieu ung skill
 */
void updateSkillEffects(SkillSystem *sk, unsigned long now) {
    /* Bomb animation (~20 frame) */
    if (sk->bombActive) {
        sk->bombFrame++;
        if (sk->bombFrame >= 22) sk->bombActive = 0;
    }
    /* Time Slow het han */
    if (sk->slowActive) {
        if (now - sk->slowLast >= sk->slowDur)
            sk->slowActive = 0;
    }
    /* Beam het han */
    if (sk->beamActive) {
        if (now - sk->beamLast >= sk->beamDur)
            sk->beamActive = 0;
    }
}

/* ============================================================
 *  SECTION 7 – VA CHAM + ITEM + SKILL DAMAGE
 * ============================================================ */

int checkCircleCollision(float x1, float y1, float r1,
                         float x2, float y2, float r2) {
    float dx = x2 - x1, dy = y2 - y1;
    return (sqrtf(dx * dx + dy * dy) < r1 + r2) ? 1 : 0;
}

void spawnExplosion(Explosion e[], int n, float x, float y, float mr) {
    int i;
    for (i = 0; i < n; i++) {
        if (!e[i].active) {
            e[i].x = x; e[i].y = y; e[i].maxRadius = mr;
            e[i].frame = 0; e[i].maxFrames = 12; e[i].active = 1;
            return;
        }
    }
}

void spawnItem(Item it[], int n, float x, float y) {
    int i;
    for (i = 0; i < n; i++) {
        if (!it[i].active) {
            it[i].x = x; it[i].y = y; it[i].vy = 1.5f;
            it[i].active = 1; it[i].radius = 13;
            it[i].sparkle = 0;
            return;
        }
    }
}

/*
 * applyItemToShip: xoay vong nang cap (pellets -> damage -> fire rate)
 * Moi lan nhat tinh the, chi so khac nhau duoc tang.
 */
void applyItemToShip(Ship *s) {
    switch (gItemCycle % 3) {
        case 0: /* Tang so vien */
            if (s->pelletsPerShot < MAX_PELLETS) s->pelletsPerShot++;
            break;
        case 1: /* Tang sat thuong */
            if (s->bulletDamage < MAX_B_DAMAGE) s->bulletDamage++;
            break;
        case 2: /* Tang toc ban */
            s->shootCooldown -= 15;
            if (s->shootCooldown < MIN_SHOOT_CD)
                s->shootCooldown = MIN_SHOOT_CD;
            break;
    }
    gItemCycle++;
}

/* Dan vs Asteroid – tru HP asteroid, neu hp<=0 thi no */
void handleBulletAsteroidCollisions(Bullet bul[], int nb,
                                    Asteroid ast[], int na,
                                    int *score, int level,
                                    Item items[], Explosion exps[]) {
    int i, j;
    for (i = 0; i < nb; i++) {
        if (!bul[i].active) continue;
        for (j = 0; j < na; j++) {
            if (!ast[j].active) continue;
            if (checkCircleCollision(bul[i].x, bul[i].y, (float)bul[i].radius,
                                     ast[j].x, ast[j].y, (float)ast[j].radius)) {
                ast[j].hp -= bul[i].damage;
                bul[i].active = 0;

                if (ast[j].hp <= 0) {
                    /* Cong diem: nho +30, vua +20, lon +10, + maxHp*5 */
                    int pts = 10;
                    if (ast[j].radius <= 22) pts = 30;
                    else if (ast[j].radius <= 35) pts = 20;
                    *score += pts + ast[j].maxHp * 5;

                    spawnExplosion(exps, MAX_EXPLOSIONS,
                                  ast[j].x, ast[j].y, (float)ast[j].radius);
                    if (rand() % 100 < ITEM_DROP_CHANCE)
                        spawnItem(items, MAX_ITEMS, ast[j].x, ast[j].y);
                    initAsteroid(&ast[j], level, &gCfg);
                }
                break;  /* 1 vien dan chi trung 1 asteroid */
            }
        }
    }
}

/* Ship vs Asteroid */
void handleShipAsteroidCollision(Ship *s, Asteroid ast[], int na,
                                 int *hp, unsigned long *lastHit,
                                 Explosion exps[], int level) {
    int j;
    unsigned long now = GetTickCount();
    for (j = 0; j < na; j++) {
        if (!ast[j].active) continue;
        if (checkCircleCollision(s->x, s->y, (float)s->radius,
                                 ast[j].x, ast[j].y, (float)ast[j].radius)) {
            if (now - *lastHit >= HIT_COOLDOWN) {
                *hp -= HIT_DAMAGE;
                if (*hp < 0) *hp = 0;
                *lastHit = now;
            }
            spawnExplosion(exps, MAX_EXPLOSIONS,
                           ast[j].x, ast[j].y, (float)ast[j].radius * 0.5f);
            initAsteroid(&ast[j], level, &gCfg);
        }
    }
}

/* Ship nhat tinh the */
void handleItemPickup(Ship *s, Item it[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!it[i].active) continue;
        if (checkCircleCollision(s->x, s->y, (float)s->radius,
                                 it[i].x, it[i].y, (float)it[i].radius)) {
            applyItemToShip(s);
            it[i].active = 0;
        }
    }
}

/* Ap dung sat thuong cua skill (goi 1 lan khi skill vua kich hoat) */
void applySkillDamage(SkillSystem *sk, Asteroid ast[], int na,
                      int *score, int level,
                      Item items[], Explosion exps[]) {
    int j;
    /* Bomb Burst: gay damage vung tron */
    if (sk->bombActive && !sk->bombDmgDone) {
        sk->bombDmgDone = 1;
        for (j = 0; j < na; j++) {
            if (!ast[j].active) continue;
            if (checkCircleCollision(sk->bombX, sk->bombY, sk->bombRadius,
                                     ast[j].x, ast[j].y, (float)ast[j].radius)) {
                ast[j].hp -= sk->bombDmg;
                if (ast[j].hp <= 0) {
                    int pts = 10;
                    if (ast[j].radius <= 22) pts = 30;
                    else if (ast[j].radius <= 35) pts = 20;
                    *score += pts + ast[j].maxHp * 5;
                    spawnExplosion(exps, MAX_EXPLOSIONS,
                                  ast[j].x, ast[j].y, (float)ast[j].radius);
                    if (rand() % 100 < ITEM_DROP_CHANCE)
                        spawnItem(items, MAX_ITEMS, ast[j].x, ast[j].y);
                    initAsteroid(&ast[j], level, &gCfg);
                }
            }
        }
    }
    /* Piercing Beam: sat thuong moi asteroid tren duong tia */
    if (sk->beamActive && !sk->beamDmgDone) {
        sk->beamDmgDone = 1;
        for (j = 0; j < na; j++) {
            if (!ast[j].active) continue;
            if (beamHitsCircle(sk->beamSX, sk->beamSY,
                               sk->beamDX, sk->beamDY,
                               ast[j].x, ast[j].y, (float)ast[j].radius)) {
                ast[j].hp -= sk->beamDmg;
                if (ast[j].hp <= 0) {
                    int pts = 10;
                    if (ast[j].radius <= 22) pts = 30;
                    else if (ast[j].radius <= 35) pts = 20;
                    *score += pts + ast[j].maxHp * 5;
                    spawnExplosion(exps, MAX_EXPLOSIONS,
                                  ast[j].x, ast[j].y, (float)ast[j].radius);
                    if (rand() % 100 < ITEM_DROP_CHANCE)
                        spawnItem(items, MAX_ITEMS, ast[j].x, ast[j].y);
                    initAsteroid(&ast[j], level, &gCfg);
                }
            }
        }
    }
}

int checkLevelUp(int score, int level) {
    return (score >= level * LEVEL_UP_SCORE) ? 1 : 0;
}

/* ============================================================
 *  SECTION 8 – VE (Render)
 * ============================================================ */

/* --- Nen sao --- */
void drawStars(void) {
    int i, c;
    for (i = 0; i < MAX_STARS; i++) {
        c = (sB[i] == 0) ? DARKGRAY : (sB[i] == 1) ? LIGHTGRAY : WHITE;
        putpixel(sX[i], sY[i], c);
    }
}

/* --- Phi thuyen --- */

void drawShipBody(Ship s, float angle) {
    /* Dinh mui */
    int nx = (int)(s.x + cosf(angle) * (s.radius + 14));
    int ny = (int)(s.y + sinf(angle) * (s.radius + 14));
    /* 2 dinh sau */
    int lx = (int)(s.x + cosf(angle + 2.5f) * s.radius);
    int ly = (int)(s.y + sinf(angle + 2.5f) * s.radius);
    int rx = (int)(s.x + cosf(angle - 2.5f) * s.radius);
    int ry = (int)(s.y + sinf(angle - 2.5f) * s.radius);

    /* Than chinh */
    setfillstyle(SOLID_FILL, CYAN);
    int body[] = { nx, ny, lx, ly, rx, ry, nx, ny };
    fillpoly(4, body);
    setcolor(WHITE);
    line(nx, ny, lx, ly); line(nx, ny, rx, ry); line(lx, ly, rx, ry);

    /* Canh trai */
    {
        int w1x = (int)(s.x + cosf(angle + 2.0f) * s.radius * 0.7f);
        int w1y = (int)(s.y + sinf(angle + 2.0f) * s.radius * 0.7f);
        int w2x = (int)(s.x + cosf(angle + 1.8f) * (s.radius + 12));
        int w2y = (int)(s.y + sinf(angle + 1.8f) * (s.radius + 12));
        int w3x = (int)(s.x + cosf(angle + 2.8f) * s.radius * 1.1f);
        int w3y = (int)(s.y + sinf(angle + 2.8f) * s.radius * 1.1f);
        setfillstyle(SOLID_FILL, BLUE);
        int wL[] = { w1x, w1y, w2x, w2y, w3x, w3y, w1x, w1y };
        fillpoly(4, wL);
        setcolor(LIGHTCYAN);
        line(w1x, w1y, w2x, w2y); line(w2x, w2y, w3x, w3y);
    }
    /* Canh phai */
    {
        int w1x = (int)(s.x + cosf(angle - 2.0f) * s.radius * 0.7f);
        int w1y = (int)(s.y + sinf(angle - 2.0f) * s.radius * 0.7f);
        int w2x = (int)(s.x + cosf(angle - 1.8f) * (s.radius + 12));
        int w2y = (int)(s.y + sinf(angle - 1.8f) * (s.radius + 12));
        int w3x = (int)(s.x + cosf(angle - 2.8f) * s.radius * 1.1f);
        int w3y = (int)(s.y + sinf(angle - 2.8f) * s.radius * 1.1f);
        setfillstyle(SOLID_FILL, BLUE);
        int wR[] = { w1x, w1y, w2x, w2y, w3x, w3y, w1x, w1y };
        fillpoly(4, wR);
        setcolor(LIGHTCYAN);
        line(w1x, w1y, w2x, w2y); line(w2x, w2y, w3x, w3y);
    }
    /* Kinh lai */
    {
        int cx = (int)(s.x + cosf(angle) * s.radius * 0.35f);
        int cy = (int)(s.y + sinf(angle) * s.radius * 0.35f);
        setcolor(LIGHTCYAN);
        setfillstyle(SOLID_FILL, LIGHTBLUE);
        fillellipse(cx, cy, 5, 4);
    }
}

void drawShipEngineFlame(Ship s, float angle, int frame) {
    float fLen = 10.0f + (frame % 6) * 2.5f;
    float hW   = 5.0f + (frame % 4);
    float bx   = s.x - cosf(angle) * s.radius * 0.5f;
    float by   = s.y - sinf(angle) * s.radius * 0.5f;
    float perp = angle + (float)M_PI / 2.0f;

    int tx = (int)(s.x - cosf(angle) * (s.radius * 0.5f + fLen));
    int ty = (int)(s.y - sinf(angle) * (s.radius * 0.5f + fLen));
    int e1x = (int)(bx + cosf(perp) * hW);
    int e1y = (int)(by + sinf(perp) * hW);
    int e2x = (int)(bx - cosf(perp) * hW);
    int e2y = (int)(by - sinf(perp) * hW);

    setfillstyle(SOLID_FILL, (frame % 3 == 0) ? LIGHTRED : RED);
    int fl[] = { e1x, e1y, tx, ty, e2x, e2y, e1x, e1y };
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
    int fi[] = { i1x, i1y, itx, ity, i2x, i2y, i1x, i1y };
    fillpoly(4, fi);
}

void drawShip(Ship s, int mx, int my, int frame) {
    float angle = atan2f((float)(my - s.y), (float)(mx - s.x));
    drawShipEngineFlame(s, angle, frame);
    drawShipBody(s, angle);

    /* Duong ngam */
    setcolor(DARKGRAY);
    setlinestyle(DASHED_LINE, 0, 1);
    float dx = (float)(mx - s.x), dy = (float)(my - s.y);
    float ln = sqrtf(dx * dx + dy * dy);
    if (ln > 1.0f) {
        int ax = (int)(s.x + dx / ln * 80);
        int ay = (int)(s.y + dy / ln * 80);
        line((int)s.x, (int)s.y, ax, ay);
    }
    setlinestyle(SOLID_LINE, 0, 1);

    /* Crosshair */
    setcolor(LIGHTGREEN);
    circle(mx, my, 8);
    line(mx - 12, my, mx + 12, my);
    line(mx, my - 12, mx, my + 12);
}

/* --- Dan --- */
void drawBullets(Bullet b[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!b[i].active) continue;
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse((int)b[i].x, (int)b[i].y, b[i].radius, b[i].radius);
    }
}

/* --- Thien thach --- */
void drawAsteroid(Asteroid a) {
    int nV = 10, i;
    int poly[20]; /* 10 dinh x 2 */
    unsigned int s;
    int cFill, cLine;

    if (!a.active) return;

    s = (unsigned int)a.seed;
    for (i = 0; i < nV; i++) {
        float va = a.rot + (float)i / nV * 2.0f * (float)M_PI;
        s = seedRand(s, i);
        float rv = 0.75f + (float)(s % 50) / 100.0f;
        poly[i * 2]     = (int)(a.x + cosf(va) * a.radius * rv);
        poly[i * 2 + 1] = (int)(a.y + sinf(va) * a.radius * rv);
    }

    if (a.radius <= 22) { cFill = DARKGRAY; cLine = LIGHTGRAY; }
    else if (a.radius <= 35) { cFill = RED; cLine = LIGHTRED; }
    else { cFill = BROWN; cLine = YELLOW; }

    setfillstyle(SOLID_FILL, cFill);
    fillpoly(nV, poly);
    setcolor(cLine);
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
    if (!a.active || a.maxHp <= 1) return;
    int bW = a.radius * 2, bH = 4;
    int x1 = (int)(a.x - bW / 2);
    int y1 = (int)(a.y - a.radius - 10);
    int fillW = (int)(bW * (float)a.hp / a.maxHp);
    if (fillW < 0) fillW = 0;

    setcolor(DARKGRAY);
    rectangle(x1, y1, x1 + bW, y1 + bH);
    if (a.hp * 2 > a.maxHp)     setfillstyle(SOLID_FILL, GREEN);
    else if (a.hp * 4 > a.maxHp) setfillstyle(SOLID_FILL, YELLOW);
    else                          setfillstyle(SOLID_FILL, RED);
    bar(x1 + 1, y1 + 1, x1 + fillW, y1 + bH - 1);
}

void drawAsteroids(Asteroid a[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        drawAsteroid(a[i]);
        drawAsteroidHPBar(a[i]);
    }
}

/* --- Vat pham: Tinh the suc manh (lap lanh + bieu tuong set) --- */
void drawItems(Item it[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!it[i].active) continue;
        it[i].sparkle++;  /* tang frame lap lanh */

        int cx = (int)it[i].x, cy = (int)it[i].y;
        int r  = it[i].radius;

        /* --- Hinh kim cuong (diamond) --- */
        int dia[] = { cx, cy - r,          /* dinh tren */
                      cx + r, cy,          /* phai */
                      cx, cy + r,          /* duoi */
                      cx - r, cy,          /* trai */
                      cx, cy - r };        /* dong */

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
        line(cx + 2, cy - 2,     cx - 2, cy + 1);
        line(cx - 2, cy + 1,     cx + 2, cy + r - 4);

        /* --- Tia sparkle xoay quanh --- */
        {
            float sa = it[i].sparkle * 0.15f; /* goc xoay */
            int k;
            for (k = 0; k < 4; k++) {
                float a = sa + k * (float)M_PI / 2.0f;
                int sx1 = cx + (int)(cosf(a) * (r + 3));
                int sy1 = cy + (int)(sinf(a) * (r + 3));
                int sx2 = cx + (int)(cosf(a) * (r + 7));
                int sy2 = cy + (int)(sinf(a) * (r + 7));
                setcolor((k + it[i].sparkle / 3) % 2 ? WHITE : YELLOW);
                line(sx1, sy1, sx2, sy2);
            }
        }
    }
}

/* --- Hieu ung no --- */
void drawExplosions(Explosion e[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!e[i].active) continue;
        float p = (float)e[i].frame / e[i].maxFrames;
        float r = e[i].maxRadius * p;
        int c;
        if (p < 0.25f) c = YELLOW;
        else if (p < 0.5f) c = LIGHTRED;
        else if (p < 0.75f) c = RED;
        else c = DARKGRAY;
        setcolor(c);
        circle((int)e[i].x, (int)e[i].y, (int)r);
        if (r > 5) circle((int)e[i].x, (int)e[i].y, (int)(r * 0.6f));
        if (r > 10) circle((int)e[i].x, (int)e[i].y, (int)(r * 0.3f));
    }
}

/* --- Hieu ung skill --- */
void drawSkillEffects(SkillSystem *sk) {
    /* Bomb Burst: vong tron no */
    if (sk->bombActive) {
        float p = (float)sk->bombFrame / 22.0f;
        float r = sk->bombRadius * p;
        int c;
        if (p < 0.3f) c = YELLOW;
        else if (p < 0.6f) c = LIGHTRED;
        else c = RED;
        setcolor(c);
        circle((int)sk->bombX, (int)sk->bombY, (int)r);
        circle((int)sk->bombX, (int)sk->bombY, (int)(r * 0.7f));
        if (r > 20) circle((int)sk->bombX, (int)sk->bombY, (int)(r * 0.4f));
    }
    /* Piercing Beam: tia sang */
    if (sk->beamActive) {
        float endX = sk->beamSX + sk->beamDX * 1500;
        float endY = sk->beamSY + sk->beamDY * 1500;
        /* Tia day 3 pixel */
        setcolor(LIGHTCYAN);
        setlinestyle(SOLID_LINE, 0, 3);
        line((int)sk->beamSX, (int)sk->beamSY, (int)endX, (int)endY);
        setcolor(WHITE);
        setlinestyle(SOLID_LINE, 0, 1);
        line((int)sk->beamSX, (int)sk->beamSY, (int)endX, (int)endY);
    }
    /* Time Slow: chi bao tren man hinh */
    if (sk->slowActive) {
        setcolor(LIGHTBLUE);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(WIDTH / 2 - 70, HEIGHT - 40, (char*)"~ SLOW ACTIVE ~");
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    }
}

/* --- HUD --- */
void drawHUD(int score, int hp, int level, Ship ship, SkillSystem *sk) {
    char buf[80];
    unsigned long now = GetTickCount();

    /* HP bar */
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(15, 12, (char*)"HP:");
    rectangle(50, 10, 260, 28);
    int bw = (int)(210.0f * hp / INITIAL_HP);
    if (bw < 0) bw = 0;
    if (hp > 60)       setfillstyle(SOLID_FILL, GREEN);
    else if (hp > 30)  setfillstyle(SOLID_FILL, YELLOW);
    else               setfillstyle(SOLID_FILL, RED);
    bar(51, 11, 51 + bw, 27);
    sprintf(buf, "%d/%d", hp, INITIAL_HP);
    setcolor(WHITE); outtextxy(268, 12, buf);

    /* Level */
    setcolor(LIGHTCYAN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    sprintf(buf, "LEVEL %d", level);
    outtextxy(WIDTH / 2 - 55, 8, buf);

    /* Score */
    setcolor(YELLOW);
    sprintf(buf, "SCORE: %d", score);
    outtextxy(WIDTH - 240, 8, buf);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

    /* Weapon stats (duoi trai) */
    setcolor(LIGHTGREEN);
    sprintf(buf, "DMG:%d  PELLETS:%d  CD:%dms",
            ship.bulletDamage, ship.pelletsPerShot, ship.shootCooldown);
    outtextxy(15, HEIGHT - 50, buf);

    /* Score de len level */
    setcolor(DARKGRAY);
    sprintf(buf, "Next Lv: %d", level * LEVEL_UP_SCORE);
    outtextxy(15, HEIGHT - 30, buf);

    /* Skill cooldown (duoi phai) */
    {
        int sx = WIDTH - 360, sy = HEIGHT - 52;
        float cd;
        int color;

        /* Skill 1 – Bomb */
        cd = 0;
        if (now - sk->bombLast < sk->bombCD)
            cd = (float)(sk->bombCD - (now - sk->bombLast)) / 1000.0f;
        color = (cd <= 0) ? GREEN : RED;
        setcolor(color);
        if (cd <= 0) sprintf(buf, "[1] BOMB: Ready");
        else         sprintf(buf, "[1] BOMB: %.1fs", cd);
        outtextxy(sx, sy, buf);

        /* Skill 2 – Slow */
        cd = 0;
        if (now - sk->slowLast < sk->slowCD)
            cd = (float)(sk->slowCD - (now - sk->slowLast)) / 1000.0f;
        if (sk->slowActive) { color = YELLOW; sprintf(buf, "[2] SLOW: Active"); }
        else if (cd <= 0)   { color = GREEN;  sprintf(buf, "[2] SLOW: Ready"); }
        else                { color = RED;    sprintf(buf, "[2] SLOW: %.1fs", cd); }
        setcolor(color);
        outtextxy(sx, sy + 16, buf);

        /* Skill 3 – Beam */
        cd = 0;
        if (now - sk->beamLast < sk->beamCD)
            cd = (float)(sk->beamCD - (now - sk->beamLast)) / 1000.0f;
        color = (cd <= 0) ? GREEN : RED;
        setcolor(color);
        if (cd <= 0) sprintf(buf, "[3] BEAM: Ready");
        else         sprintf(buf, "[3] BEAM: %.1fs", cd);
        outtextxy(sx, sy + 32, buf);
    }
}

/* --- Level Banner --- */
void drawLevelBanner(int level) {
    char buf[32];
    setfillstyle(SOLID_FILL, BLUE);
    bar(WIDTH / 2 - 200, HEIGHT / 2 - 60, WIDTH / 2 + 200, HEIGHT / 2 + 40);
    setcolor(LIGHTCYAN);
    rectangle(WIDTH / 2 - 200, HEIGHT / 2 - 60, WIDTH / 2 + 200, HEIGHT / 2 + 40);
    rectangle(WIDTH / 2 - 202, HEIGHT / 2 - 62, WIDTH / 2 + 202, HEIGHT / 2 + 42);
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    outtextxy(WIDTH / 2 - 105, HEIGHT / 2 - 48, (char*)"LEVEL UP!");
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    sprintf(buf, "Level %d", level);
    outtextxy(WIDTH / 2 - 45, HEIGHT / 2 + 5, buf);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* --- Game Over --- */
void drawGameOver(int score, int level) {
    char buf[80];
    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 0, WIDTH, HEIGHT);
    setcolor(RED);
    rectangle(WIDTH / 2 - 280, HEIGHT / 2 - 140, WIDTH / 2 + 280, HEIGHT / 2 + 130);
    rectangle(WIDTH / 2 - 282, HEIGHT / 2 - 142, WIDTH / 2 + 282, HEIGHT / 2 + 132);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 7);
    outtextxy(WIDTH / 2 - 210, HEIGHT / 2 - 110, (char*)"GAME OVER");
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    sprintf(buf, "Final Score: %d", score);
    outtextxy(WIDTH / 2 - 130, HEIGHT / 2 - 10, buf);
    setcolor(LIGHTCYAN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    sprintf(buf, "Level Reached: %d", level);
    outtextxy(WIDTH / 2 - 100, HEIGHT / 2 + 40, buf);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(WIDTH / 2 - 110, HEIGHT / 2 + 90, (char*)"Press any key to exit...");
}

/* ============================================================
 *  SECTION 9 – MAIN / GAME LOOP
 * ============================================================ */

int main() {
    Ship       ship;
    Bullet     bullets[MAX_BULLETS];
    Asteroid   asteroids[MAX_ASTEROIDS];
    Item       items[MAX_ITEMS];
    Explosion  explosions[MAX_EXPLOSIONS];
    SkillSystem skills;

    int score = 0, hp = INITIAL_HP, level = 1, gameOver = 0;
    unsigned long lastShot = 0, lastHit = 0;
    int mx = WIDTH / 2, my = HEIGHT / 2;
    int frameCount = 0, levelBanner = 0, page = 0;

    /* Khoi tao cua so */
    int gd = DETECT, gm;
    initwindow(WIDTH, HEIGHT, "Asteroid Blaster v3.0");
    setbkcolor(BLACK);
    cleardevice();

    /* Khoi tao game */
    initGame(&ship, asteroids, bullets, items, explosions, &skills,
             &score, &hp, &level);

    /* ====== VONG LAP CHINH ======
     *  input -> update -> collision -> skill dmg
     *  -> level check -> render -> delay
     */
    while (!gameOver) {
        unsigned long now = GetTickCount();
        frameCount++;
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;

        /* 1) INPUT */
        handleKeyboardMove(&ship);
        handleMouse(&mx, &my);
        handleSkillsInput(&skills, &ship, mx, my, now);

        /* 2) UPDATE */
        updateShip(&ship);
        tryAutoShoot(&ship, bullets, MAX_BULLETS, mx, my, &lastShot);
        updateBullets(bullets, MAX_BULLETS);

        /* speedMul cho time slow */
        float spdM = skills.slowActive ? skills.slowFactor : 1.0f;
        updateAsteroids(asteroids, MAX_ASTEROIDS, level, spdM);

        updateItems(items, MAX_ITEMS);
        updateExplosions(explosions, MAX_EXPLOSIONS);
        updateSkillEffects(&skills, now);

        /* 3) COLLISION */
        handleBulletAsteroidCollisions(bullets, MAX_BULLETS,
                                       asteroids, MAX_ASTEROIDS,
                                       &score, level, items, explosions);
        handleShipAsteroidCollision(&ship, asteroids, MAX_ASTEROIDS,
                                    &hp, &lastHit, explosions, level);
        handleItemPickup(&ship, items, MAX_ITEMS);

        /* 4) SKILL DAMAGE */
        applySkillDamage(&skills, asteroids, MAX_ASTEROIDS,
                         &score, level, items, explosions);

        /* 5) LEVEL CHECK */
        if (checkLevelUp(score, level)) {
            level++;
            updateDifficultyByLevel(level, &gCfg);
            levelBanner = LEVEL_BANNER_FR;
            { int i; for (i = 0; i < MAX_ASTEROIDS; i++) {
                initAsteroid(&asteroids[i], level, &gCfg);
                asteroids[i].y = (float)(-(rand() % 300) - 200);
            }}
        }
        if (hp <= 0) gameOver = 1;

        /* 6) RENDER (double buffer) */
        setactivepage(page);
        cleardevice();

        drawStars();
        drawAsteroids(asteroids, MAX_ASTEROIDS);
        drawItems(items, MAX_ITEMS);
        drawBullets(bullets, MAX_BULLETS);
        drawExplosions(explosions, MAX_EXPLOSIONS);
        drawSkillEffects(&skills);
        drawShip(ship, mx, my, frameCount);
        drawHUD(score, hp, level, ship, &skills);
        if (levelBanner > 0) { drawLevelBanner(level); levelBanner--; }

        setvisualpage(page);
        page = 1 - page;

        /* 7) DELAY */
        delay(16);
    }

    /* GAME OVER */
    setactivepage(0); setvisualpage(0);
    drawGameOver(score, level);
    getch();
    closegraph();
    return 0;
}
