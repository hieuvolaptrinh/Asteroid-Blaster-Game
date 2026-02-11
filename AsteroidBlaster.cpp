/*
 *  - Phi thuyen nhieu lop (than + kinh lai + canh + lua dong co)
 *  - Thien thach polygon on dinh (seed), xoay, roi da huong
 *  - He thong Level (qua man tang do kho)
 *  - 4 loai vu khi: NORMAL, SPREAD, RAPID, PIERCE
 *  - Vat pham roi (power-up) khi ban vo thien thach
 *  - Hieu ung no (explosion particles)
 *  - HUD day du: HP, Score, Level, Weapon + timer
 *  - Double buffer chong nhap nhay
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



#define WIDTH           1200
#define HEIGHT          800
#define MAX_ASTEROIDS   25
#define MAX_BULLETS     60
#define MAX_ITEMS       10
#define MAX_EXPLOSIONS  30
#define MAX_STARS       100

#define SHIP_SPEED      5
#define SHIP_RADIUS     20
#define BULLET_SPEED    12
#define BULLET_RADIUS   3

#define SHOOT_COOLDOWN  120       /* ms – mac dinh (level 1) */
#define HIT_COOLDOWN    500       /* ms */
#define POWERUP_DURATION 8000     /* ms (8 giay) */
#define ITEM_DROP_CHANCE 15       /* % */

#define INITIAL_HP      100
#define HIT_DAMAGE      15

#define AST_MIN_RADIUS  15
#define AST_MAX_RADIUS  45
#define AST_MIN_SPEED   1.0f
#define AST_MAX_SPEED   3.5f

#define LEVEL_UP_SCORE  300       /* score >= level * nay -> len level */
#define LEVEL_BANNER_FRAMES 90    /* ~1.5 giay hien banner */

#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

/* Loai vu khi */
typedef enum {
    WPN_NORMAL = 0,   /* 1 vien thang */
    WPN_SPREAD,       /* 3 vien xoe */
    WPN_RAPID,        /* ban nhanh */
    WPN_PIERCE        /* dan xuyen */
} WeaponType;

/* Phi thuyen */
typedef struct {
    float x, y;
    int   radius;
    int   speed;
    WeaponType weapon;
    unsigned long weaponEndTime;  /* thoi diem het hieu luc power-up (ms) */
} Ship;

/* Vien dan */
typedef struct {
    float x, y;
    float dx, dy;
    int   active;
    int   radius;
    int   damage;
    int   pierce;    /* so lan xuyen con lai (0 = khong xuyen) */
} Bullet;

/* Thien thach */
typedef struct {
    float x, y;
    int   radius;
    float vx, vy;       /* van toc theo 2 truc */
    int   active;
    int   seed;          /* seed de ve polygon on dinh */
    float rot;           /* goc xoay hien tai (rad) */
    float rotSpeed;      /* toc do xoay (rad/frame) */
} Asteroid;

/* Vat pham roi */
typedef struct {
    float x, y;
    float vy;
    int   active;
    int   type;    /* WPN_SPREAD / WPN_RAPID / WPN_PIERCE */
    int   radius;
} Item;

/* Hieu ung no */
typedef struct {
    float x, y;
    int   frame;
    int   maxFrames;
    float maxRadius;
    int   active;
} Explosion;

/* Cau hinh do kho (thay doi theo level) */
typedef struct {
    int   activeAsteroids;  /* So thien thach hoat dong */
    float astSpeedMin;
    float astSpeedMax;
    int   shootCooldown;    /* ms */
    float angleVariety;     /* do lech goc roi (rad) */
} GameConfig;

/* ============================================================
 *  BIEN TOAN CUC (Global)
 * ============================================================ */

/* Cau hinh game (duoc cap nhat khi doi level) */
GameConfig gCfg;

/* Mang sao nen (khoi tao 1 lan, khong doi) */
static int starX[MAX_STARS], starY[MAX_STARS], starBright[MAX_STARS];
static int starsInitialized = 0;

/* ============================================================
 *  3) HAM TIEN ICH
 * ============================================================ */

/*
 * seedRand: tao so ngau nhien on dinh tu seed + index
 * Dung de ve polygon thien thach khong nhap nhay
 */
static unsigned int seedRand(unsigned int seed, int index) {
    unsigned int val = seed;
    val = val * 1103515245u + 12345u + (unsigned int)index * 2654435761u;
    return val;
}

/* Khoi tao mang sao nen (goi 1 lan) */
static void initStars(void) {
    int i;
    for (i = 0; i < MAX_STARS; i++) {
        starX[i]      = rand() % WIDTH;
        starY[i]      = rand() % HEIGHT;
        starBright[i]  = rand() % 3;  /* 0=mo, 1=vua, 2=sang */
    }
    starsInitialized = 1;
}

/* ============================================================
 *  4) KHOI TAO (Init)
 * ============================================================ */

/* Cap nhat do kho theo level */
void updateDifficultyByLevel(int level, GameConfig *cfg) {
    cfg->activeAsteroids = 8 + level * 2;
    if (cfg->activeAsteroids > MAX_ASTEROIDS)
        cfg->activeAsteroids = MAX_ASTEROIDS;

    cfg->astSpeedMin = AST_MIN_SPEED + level * 0.15f;
    cfg->astSpeedMax = AST_MAX_SPEED + level * 0.25f;

    cfg->shootCooldown = SHOOT_COOLDOWN - level * 8;
    if (cfg->shootCooldown < 40) cfg->shootCooldown = 40;

    cfg->angleVariety = 0.3f + level * 0.1f;
    if (cfg->angleVariety > 1.2f) cfg->angleVariety = 1.2f;
}

void initShip(Ship *ship) {
    ship->x             = WIDTH / 2.0f;
    ship->y             = HEIGHT - 80.0f;
    ship->radius        = SHIP_RADIUS;
    ship->speed         = SHIP_SPEED;
    ship->weapon        = WPN_NORMAL;
    ship->weaponEndTime = 0;
}

void initBullet(Bullet *b) {
    b->x      = 0;  b->y  = 0;
    b->dx     = 0;  b->dy = 0;
    b->active = 0;
    b->radius = BULLET_RADIUS;
    b->damage = 1;
    b->pierce = 0;
}

/*
 * initAsteroid: tao thien thach moi
 * - Vi tri phia tren man hinh
 * - Toc do + goc roi dua theo level (doc tu gCfg)
 * - seed rieng de ve polygon on dinh
 */
void initAsteroid(Asteroid *a, int level) {
    float speed, maxAngle;

    a->radius = AST_MIN_RADIUS + rand() % (AST_MAX_RADIUS - AST_MIN_RADIUS + 1);
    a->x      = (float)(a->radius + rand() % (WIDTH - 2 * a->radius));
    a->y      = (float)(-(rand() % 400) - a->radius);

    /* Toc do roi trong khoang [min, max] */
    speed = gCfg.astSpeedMin
          + (rand() % 100) / 100.0f * (gCfg.astSpeedMax - gCfg.astSpeedMin);

    /* Goc roi: vy luon duong (xuong), vx lech trai/phai */
    maxAngle = gCfg.angleVariety;
    a->vy = speed;
    a->vx = ((rand() % 200 - 100) / 100.0f) * maxAngle * speed * 0.4f;

    /* Seed on dinh de ve polygon */
    a->seed     = rand() % 100000;
    a->rot      = 0.0f;
    a->rotSpeed = ((rand() % 100) - 50) / 1500.0f;  /* quay cham */
    a->active   = 1;
}

void initAllAsteroids(Asteroid ast[], int n, int level) {
    int i;
    for (i = 0; i < n; i++) {
        initAsteroid(&ast[i], level);
        /* Dat active chi cho so luong theo level */
        if (i >= gCfg.activeAsteroids)
            ast[i].active = 0;
    }
}

/* Khoi tao toan bo game */
void initGame(Ship *ship, Asteroid ast[], Bullet bullets[],
              Item items[], Explosion exps[],
              int *score, int *hp, int *level) {
    int i;
    srand((unsigned int)time(NULL));

    *level = 1;
    updateDifficultyByLevel(*level, &gCfg);

    initShip(ship);
    initAllAsteroids(ast, MAX_ASTEROIDS, *level);

    for (i = 0; i < MAX_BULLETS; i++)
        initBullet(&bullets[i]);

    for (i = 0; i < MAX_ITEMS; i++) {
        items[i].active = 0;
        items[i].radius = 10;
    }

    for (i = 0; i < MAX_EXPLOSIONS; i++)
        exps[i].active = 0;

    *score = 0;
    *hp    = INITIAL_HP;

    if (!starsInitialized) initStars();
}

/* ============================================================
 *  5) XU LY INPUT
 * ============================================================ */

void handleKeyboard(Ship *ship) {
    if (GetAsyncKeyState(VK_LEFT)  & 0x8000 || GetAsyncKeyState('A') & 0x8000)
        ship->x -= ship->speed;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000)
        ship->x += ship->speed;
    if (GetAsyncKeyState(VK_UP)    & 0x8000 || GetAsyncKeyState('W') & 0x8000)
        ship->y -= ship->speed;
    if (GetAsyncKeyState(VK_DOWN)  & 0x8000 || GetAsyncKeyState('S') & 0x8000)
        ship->y += ship->speed;
}

void handleMouse(int *mx, int *my) {
    *mx = mousex();
    *my = mousey();
}

/* ============================================================
 *  6) CAP NHAT LOGIC (Update)
 * ============================================================ */

/* Gioi han phi thuyen trong man hinh */
void updateShip(Ship *ship) {
    if (ship->x - ship->radius < 0)     ship->x = (float)ship->radius;
    if (ship->x + ship->radius > WIDTH)  ship->x = (float)(WIDTH - ship->radius);
    if (ship->y - ship->radius < 0)     ship->y = (float)ship->radius;
    if (ship->y + ship->radius > HEIGHT) ship->y = (float)(HEIGHT - ship->radius);

    /* Het thoi gian power-up -> quay ve NORMAL */
    if (ship->weapon != WPN_NORMAL && GetTickCount() >= ship->weaponEndTime) {
        ship->weapon = WPN_NORMAL;
    }
}

/* --- Cac ham ban theo tung loai vu khi --- */

/* Tim 1 slot dan trong */
static int findFreeBullet(Bullet bullets[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!bullets[i].active) return i;
    }
    return -1;
}

/* Ban 1 vien thang (NORMAL / RAPID) */
void shootNormal(Ship *ship, Bullet bullets[], int n, float dirX, float dirY) {
    int idx = findFreeBullet(bullets, n);
    if (idx < 0) return;

    bullets[idx].x      = ship->x;
    bullets[idx].y      = ship->y;
    bullets[idx].dx     = dirX * BULLET_SPEED;
    bullets[idx].dy     = dirY * BULLET_SPEED;
    bullets[idx].active = 1;
    bullets[idx].radius = BULLET_RADIUS;
    bullets[idx].damage = 1;
    bullets[idx].pierce = 0;
}

/* Ban 3 vien xoe (SPREAD: -10°, 0°, +10°) */
void shootSpread(Ship *ship, Bullet bullets[], int n, float dirX, float dirY) {
    float baseAngle = atan2f(dirY, dirX);
    float offsets[3] = { -0.17f, 0.0f, 0.17f };  /* ~10 do */
    int j, idx;

    for (j = 0; j < 3; j++) {
        idx = findFreeBullet(bullets, n);
        if (idx < 0) return;

        float a = baseAngle + offsets[j];
        bullets[idx].x      = ship->x;
        bullets[idx].y      = ship->y;
        bullets[idx].dx     = cosf(a) * BULLET_SPEED;
        bullets[idx].dy     = sinf(a) * BULLET_SPEED;
        bullets[idx].active = 1;
        bullets[idx].radius = BULLET_RADIUS;
        bullets[idx].damage = 1;
        bullets[idx].pierce = 0;
    }
}

/* Ban dan xuyen (PIERCE: xuyen qua 3 thien thach) */
void shootPierce(Ship *ship, Bullet bullets[], int n, float dirX, float dirY) {
    int idx = findFreeBullet(bullets, n);
    if (idx < 0) return;

    bullets[idx].x      = ship->x;
    bullets[idx].y      = ship->y;
    bullets[idx].dx     = dirX * BULLET_SPEED;
    bullets[idx].dy     = dirY * BULLET_SPEED;
    bullets[idx].active = 1;
    bullets[idx].radius = BULLET_RADIUS + 1;  /* to hon chut */
    bullets[idx].damage = 1;
    bullets[idx].pierce = 3;  /* xuyen 3 thien thach */
}

/*
 * tryAutoShoot: tu dong ban theo huong chuot
 * Chon kieu ban dua tren ship->weapon
 */
void tryAutoShoot(Ship *ship, Bullet bullets[], int n,
                  int mx, int my, unsigned long *lastShootTime) {
    unsigned long now = GetTickCount();
    int cooldown = gCfg.shootCooldown;
    float dirX, dirY, len;

    /* RAPID giam cooldown 3 lan */
    if (ship->weapon == WPN_RAPID)
        cooldown = cooldown / 3;
    if (cooldown < 30) cooldown = 30;

    if (now - *lastShootTime < (unsigned long)cooldown) return;

    /* Tinh huong ban: ship -> mouse */
    dirX = mx - ship->x;
    dirY = my - ship->y;
    len  = sqrtf(dirX * dirX + dirY * dirY);
    if (len < 5.0f) return;
    dirX /= len;
    dirY /= len;

    /* Goi ham ban tuong ung */
    switch (ship->weapon) {
        case WPN_SPREAD:  shootSpread(ship, bullets, n, dirX, dirY); break;
        case WPN_PIERCE:  shootPierce(ship, bullets, n, dirX, dirY); break;
        default:          shootNormal(ship, bullets, n, dirX, dirY);  break;
    }
    *lastShootTime = now;
}

/* Cap nhat cac vien dan */
void updateBullets(Bullet bullets[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!bullets[i].active) continue;
        bullets[i].x += bullets[i].dx;
        bullets[i].y += bullets[i].dy;
        if (bullets[i].x < -20 || bullets[i].x > WIDTH + 20 ||
            bullets[i].y < -20 || bullets[i].y > HEIGHT + 20)
            bullets[i].active = 0;
    }
}

/* Cap nhat thien thach: di chuyen + xoay + respawn khi ra man hinh */
void updateAsteroids(Asteroid ast[], int n, int level) {
    int i;
    for (i = 0; i < n; i++) {
        if (!ast[i].active) continue;

        ast[i].x   += ast[i].vx;
        ast[i].y   += ast[i].vy;
        ast[i].rot += ast[i].rotSpeed;

        /* Ra khoi man hinh -> respawn */
        if (ast[i].y - ast[i].radius > HEIGHT ||
            ast[i].x + ast[i].radius < -50 ||
            ast[i].x - ast[i].radius > WIDTH + 50) {
            initAsteroid(&ast[i], level);
        }
    }

    /* Kich hoat them thien thach khi level tang */
    for (i = 0; i < n && i < gCfg.activeAsteroids; i++) {
        if (!ast[i].active) {
            initAsteroid(&ast[i], level);
        }
    }
}

/* Cap nhat vat pham roi */
void updateItems(Item items[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!items[i].active) continue;
        items[i].y += items[i].vy;
        if (items[i].y > HEIGHT + 20) items[i].active = 0;
    }
}

/* Cap nhat hieu ung no */
void updateExplosions(Explosion exps[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!exps[i].active) continue;
        exps[i].frame++;
        if (exps[i].frame >= exps[i].maxFrames)
            exps[i].active = 0;
    }
}

/* ============================================================
 *  7) VA CHAM
 * ============================================================ */

int checkCircleCollision(float x1, float y1, float r1,
                         float x2, float y2, float r2) {
    float dx   = x2 - x1;
    float dy   = y2 - y1;
    float dist = sqrtf(dx * dx + dy * dy);
    return (dist < r1 + r2) ? 1 : 0;
}

/* Tao vat pham roi tai vi tri (x, y) */
void spawnItem(Item items[], int n, float x, float y) {
    int i, types[3] = { WPN_SPREAD, WPN_RAPID, WPN_PIERCE };

    for (i = 0; i < n; i++) {
        if (!items[i].active) {
            items[i].x      = x;
            items[i].y      = y;
            items[i].vy     = 1.5f;
            items[i].active = 1;
            items[i].type   = types[rand() % 3];
            items[i].radius = 12;
            return;
        }
    }
}

/* Ap dung vat pham cho phi thuyen */
void applyItemToShip(Ship *ship, int itemType, unsigned long now) {
    ship->weapon        = (WeaponType)itemType;
    ship->weaponEndTime = now + POWERUP_DURATION;
}

/* Tao hieu ung no */
void spawnExplosion(Explosion exps[], int n, float x, float y, float maxR) {
    int i;
    for (i = 0; i < n; i++) {
        if (!exps[i].active) {
            exps[i].x         = x;
            exps[i].y         = y;
            exps[i].frame     = 0;
            exps[i].maxFrames = 12;
            exps[i].maxRadius = maxR;
            exps[i].active    = 1;
            return;
        }
    }
}

/*
 * handleCollisions: xu ly toan bo va cham
 *   - Dan   vs Thien thach -> diem + no + co the drop item
 *   - Ship  vs Thien thach -> tru HP (co cooldown)
 *   - Ship  vs Vat pham    -> nhat power-up
 */
void handleCollisions(Ship *ship, Asteroid ast[], int na,
                      Bullet bullets[], int nb,
                      int *score, int *hp, unsigned long *lastHitTime,
                      Item items[], Explosion exps[], int level) {
    int i, j;
    unsigned long now = GetTickCount();

    /* --- Dan vs Thien thach --- */
    for (i = 0; i < nb; i++) {
        if (!bullets[i].active) continue;

        for (j = 0; j < na; j++) {
            if (!ast[j].active) continue;

            if (checkCircleCollision(bullets[i].x, bullets[i].y,
                                     (float)bullets[i].radius,
                                     ast[j].x, ast[j].y,
                                     (float)ast[j].radius)) {
                /* Tinh diem theo kich thuoc (nho = kho ban = nhieu diem) */
                if (ast[j].radius <= 22)
                    *score += 30;
                else if (ast[j].radius <= 35)
                    *score += 20;
                else
                    *score += 10;

                /* Hieu ung no */
                spawnExplosion(exps, MAX_EXPLOSIONS,
                               ast[j].x, ast[j].y, (float)ast[j].radius);

                /* Co hoi drop item (15%) */
                if (rand() % 100 < ITEM_DROP_CHANCE)
                    spawnItem(items, MAX_ITEMS, ast[j].x, ast[j].y);

                /* Respawn thien thach */
                initAsteroid(&ast[j], level);

                /* Xu ly xuyen / tat dan */
                if (bullets[i].pierce > 0) {
                    bullets[i].pierce--;
                    /* Dan xuyen -> tiep tuc kiem tra thien thach khac */
                } else {
                    bullets[i].active = 0;
                    break; /* Dan thuong chi trung 1 lan */
                }
            }
        }
    }

    /* --- Ship vs Thien thach --- */
    for (j = 0; j < na; j++) {
        if (!ast[j].active) continue;
        if (checkCircleCollision(ship->x, ship->y, (float)ship->radius,
                                 ast[j].x, ast[j].y, (float)ast[j].radius)) {
            if (now - *lastHitTime >= HIT_COOLDOWN) {
                *hp -= HIT_DAMAGE;
                if (*hp < 0) *hp = 0;
                *lastHitTime = now;
            }
            spawnExplosion(exps, MAX_EXPLOSIONS, ast[j].x, ast[j].y,
                           (float)ast[j].radius * 0.5f);
            initAsteroid(&ast[j], level);
        }
    }

    /* --- Ship vs Vat pham --- */
    for (i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].active) continue;
        if (checkCircleCollision(ship->x, ship->y, (float)ship->radius,
                                 items[i].x, items[i].y,
                                 (float)items[i].radius)) {
            applyItemToShip(ship, items[i].type, now);
            items[i].active = 0;
        }
    }
}

/* Kiem tra dieu kien len level */
int checkLevelUp(int score, int level) {
    return (score >= level * LEVEL_UP_SCORE) ? 1 : 0;
}

/* ============================================================
 *  8) VE (Render)
 * ============================================================ */

/* ---- VE NEN SAO ---- */
void drawStars(void) {
    int i, color;
    for (i = 0; i < MAX_STARS; i++) {
        switch (starBright[i]) {
            case 0:  color = DARKGRAY;  break;
            case 1:  color = LIGHTGRAY; break;
            default: color = WHITE;     break;
        }
        putpixel(starX[i], starY[i], color);
    }
}

/* ---- VE PHI THUYEN ---- */

/* Ve than tau (tam giac + canh + kinh lai) */
void drawShipBody(Ship s, float angle) {
    int px[8], py[8];   /* diem phuc vu ve */
    int i;

    /* ----- Than chinh (tam giac) ----- */
    /* Dinh mui */
    px[0] = (int)(s.x + cosf(angle) * (s.radius + 14));
    py[0] = (int)(s.y + sinf(angle) * (s.radius + 14));
    /* 2 dinh sau */
    px[1] = (int)(s.x + cosf(angle + 2.5f) * s.radius);
    py[1] = (int)(s.y + sinf(angle + 2.5f) * s.radius);
    px[2] = (int)(s.x + cosf(angle - 2.5f) * s.radius);
    py[2] = (int)(s.y + sinf(angle - 2.5f) * s.radius);

    /* To mau than */
    setfillstyle(SOLID_FILL, CYAN);
    int body[] = { px[0], py[0], px[1], py[1], px[2], py[2], px[0], py[0] };
    fillpoly(4, body);

    /* Vien sang */
    setcolor(WHITE);
    line(px[0], py[0], px[1], py[1]);
    line(px[0], py[0], px[2], py[2]);
    line(px[1], py[1], px[2], py[2]);

    /* ----- Canh trai ----- */
    int wx1 = (int)(s.x + cosf(angle + 2.0f) * s.radius * 0.7f);
    int wy1 = (int)(s.y + sinf(angle + 2.0f) * s.radius * 0.7f);
    int wx2 = (int)(s.x + cosf(angle + 1.8f) * (s.radius + 12));
    int wy2 = (int)(s.y + sinf(angle + 1.8f) * (s.radius + 12));
    int wx3 = (int)(s.x + cosf(angle + 2.8f) * s.radius * 1.1f);
    int wy3 = (int)(s.y + sinf(angle + 2.8f) * s.radius * 1.1f);

    setfillstyle(SOLID_FILL, BLUE);
    int wingL[] = { wx1, wy1, wx2, wy2, wx3, wy3, wx1, wy1 };
    fillpoly(4, wingL);
    setcolor(LIGHTCYAN);
    line(wx1, wy1, wx2, wy2);
    line(wx2, wy2, wx3, wy3);

    /* ----- Canh phai (doi xung) ----- */
    wx1 = (int)(s.x + cosf(angle - 2.0f) * s.radius * 0.7f);
    wy1 = (int)(s.y + sinf(angle - 2.0f) * s.radius * 0.7f);
    wx2 = (int)(s.x + cosf(angle - 1.8f) * (s.radius + 12));
    wy2 = (int)(s.y + sinf(angle - 1.8f) * (s.radius + 12));
    wx3 = (int)(s.x + cosf(angle - 2.8f) * s.radius * 1.1f);
    wy3 = (int)(s.y + sinf(angle - 2.8f) * s.radius * 1.1f);

    setfillstyle(SOLID_FILL, BLUE);
    int wingR[] = { wx1, wy1, wx2, wy2, wx3, wy3, wx1, wy1 };
    fillpoly(4, wingR);
    setcolor(LIGHTCYAN);
    line(wx1, wy1, wx2, wy2);
    line(wx2, wy2, wx3, wy3);

    /* ----- Kinh lai (cockpit) ----- */
    int cx = (int)(s.x + cosf(angle) * s.radius * 0.35f);
    int cy = (int)(s.y + sinf(angle) * s.radius * 0.35f);
    setcolor(LIGHTCYAN);
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    fillellipse(cx, cy, 5, 4);
}

/* Ve ngon lua dong co (nhap nhay theo frame) */
void drawShipEngineFlame(Ship s, float angle, int frame) {
    /* Chieu dai ngon lua dao dong */
    float flameLen = 10.0f + (frame % 6) * 2.0f;
    float halfW    = 5.0f + (frame % 4);

    /* Diem goc lua: phia sau tau */
    float bx = s.x - cosf(angle) * s.radius * 0.5f;
    float by = s.y - sinf(angle) * s.radius * 0.5f;

    /* Dinh ngon lua */
    int tipX = (int)(s.x - cosf(angle) * (s.radius * 0.5f + flameLen));
    int tipY = (int)(s.y - sinf(angle) * (s.radius * 0.5f + flameLen));

    /* 2 mep ngon lua */
    float perpAngle = angle + (float)M_PI / 2.0f;
    int e1x = (int)(bx + cosf(perpAngle) * halfW);
    int e1y = (int)(by + sinf(perpAngle) * halfW);
    int e2x = (int)(bx - cosf(perpAngle) * halfW);
    int e2y = (int)(by - sinf(perpAngle) * halfW);

    /* Lop ngoai: mau do/cam */
    setfillstyle(SOLID_FILL, (frame % 3 == 0) ? LIGHTRED : RED);
    int flame[] = { e1x, e1y, tipX, tipY, e2x, e2y, e1x, e1y };
    fillpoly(4, flame);

    /* Lop trong: mau vang (nho hon) */
    float innerLen = flameLen * 0.6f;
    float innerW   = halfW * 0.5f;
    int itX = (int)(s.x - cosf(angle) * (s.radius * 0.5f + innerLen));
    int itY = (int)(s.y - sinf(angle) * (s.radius * 0.5f + innerLen));
    int ie1x = (int)(bx + cosf(perpAngle) * innerW);
    int ie1y = (int)(by + sinf(perpAngle) * innerW);
    int ie2x = (int)(bx - cosf(perpAngle) * innerW);
    int ie2y = (int)(by - sinf(perpAngle) * innerW);

    setfillstyle(SOLID_FILL, YELLOW);
    int innerFlame[] = { ie1x, ie1y, itX, itY, ie2x, ie2y, ie1x, ie1y };
    fillpoly(4, innerFlame);
}

/*
 * drawShip: ve phi thuyen day du + duong ngam + crosshair
 */
void drawShip(Ship s, int mx, int my, int frame) {
    float angle = atan2f((float)(my - s.y), (float)(mx - s.x));

    /* 1) Ngon lua dong co (ve truoc, nam duoi than) */
    drawShipEngineFlame(s, angle, frame);

    /* 2) Than tau */
    drawShipBody(s, angle);

    /* 3) Duong ngam (dashed) */
    setcolor(DARKGRAY);
    setlinestyle(DASHED_LINE, 0, 1);
    float dirX = (float)(mx - s.x);
    float dirY = (float)(my - s.y);
    float len  = sqrtf(dirX * dirX + dirY * dirY);
    if (len > 1.0f) {
        int aimX = (int)(s.x + (dirX / len) * 80.0f);
        int aimY = (int)(s.y + (dirY / len) * 80.0f);
        line((int)s.x, (int)s.y, aimX, aimY);
    }
    setlinestyle(SOLID_LINE, 0, 1);

    /* 4) Crosshair tai vi tri chuot */
    setcolor(LIGHTGREEN);
    circle(mx, my, 8);
    line(mx - 12, my, mx + 12, my);
    line(mx, my - 12, mx, my + 12);
}

/* ---- VE DAN ---- */
void drawBullets(Bullet bullets[], int n) {
    int i, color;
    for (i = 0; i < n; i++) {
        if (!bullets[i].active) continue;

        /* Mau dan theo loai */
        if (bullets[i].pierce > 0)
            color = LIGHTCYAN;   /* PIERCE: xanh nhat */
        else
            color = YELLOW;      /* NORMAL / SPREAD / RAPID */

        setcolor(color);
        setfillstyle(SOLID_FILL, color);
        fillellipse((int)bullets[i].x, (int)bullets[i].y,
                     bullets[i].radius, bullets[i].radius);
    }
}

/* ---- VE 1 THIEN THACH (polygon on dinh theo seed) ---- */
void drawAsteroid(Asteroid a) {
    int numVerts = 10;
    int poly[22];  /* 10 dinh * 2 toa do + 2 (dong) */
    int i, color_fill, color_line;
    unsigned int s;

    if (!a.active) return;

    /* Tao polygon on dinh bang seed (khong random moi frame) */
    s = (unsigned int)a.seed;
    for (i = 0; i < numVerts; i++) {
        float vertAngle = a.rot + (float)i / numVerts * 2.0f * (float)M_PI;
        /* Bien thien ban kinh 75%-125% dua theo seed */
        s = seedRand(s, i);
        float rVar = 0.75f + (float)(s % 50) / 100.0f;
        float r = a.radius * rVar;
        poly[i * 2]     = (int)(a.x + cosf(vertAngle) * r);
        poly[i * 2 + 1] = (int)(a.y + sinf(vertAngle) * r);
    }

    /* Mau sac theo kich thuoc */
    if (a.radius <= 22) {
        color_fill = DARKGRAY;
        color_line = LIGHTGRAY;
    } else if (a.radius <= 35) {
        color_fill = RED;
        color_line = LIGHTRED;
    } else {
        color_fill = BROWN;
        color_line = YELLOW;
    }

    /* To mau */
    setfillstyle(SOLID_FILL, color_fill);
    fillpoly(numVerts, poly);

    /* Vien ngoai */
    setcolor(color_line);
    for (i = 0; i < numVerts; i++) {
        int next = (i + 1) % numVerts;
        line(poly[i * 2], poly[i * 2 + 1],
             poly[next * 2], poly[next * 2 + 1]);
    }

    /* Them chi tiet: 1-2 dem (crater) nho tren be mat */
    s = seedRand((unsigned int)a.seed, 99);
    int crX = (int)(a.x + cosf(a.rot + 1.0f) * a.radius * 0.3f);
    int crY = (int)(a.y + sinf(a.rot + 1.0f) * a.radius * 0.3f);
    setcolor(color_line);
    circle(crX, crY, a.radius / 5);
}

void drawAsteroids(Asteroid ast[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        drawAsteroid(ast[i]);
    }
}

/* ---- VE VAT PHAM ---- */
void drawItems(Item items[], int n) {
    int i;
    char label;
    int bgColor;

    for (i = 0; i < n; i++) {
        if (!items[i].active) continue;

        /* Mau + ky tu theo loai */
        switch (items[i].type) {
            case WPN_SPREAD: bgColor = RED;       label = 'S'; break;
            case WPN_RAPID:  bgColor = GREEN;     label = 'R'; break;
            case WPN_PIERCE: bgColor = LIGHTBLUE; label = 'P'; break;
            default:         bgColor = WHITE;     label = 'N'; break;
        }

        /* Ve hinh tron nen */
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, bgColor);
        fillellipse((int)items[i].x, (int)items[i].y,
                     items[i].radius, items[i].radius);

        /* Vien */
        circle((int)items[i].x, (int)items[i].y, items[i].radius);

        /* Chu ky hieu */
        char buf[2] = { label, '\0' };
        setcolor(WHITE);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        outtextxy((int)items[i].x - 3, (int)items[i].y - 4, buf);
    }
}

/* ---- VE HIEU UNG NO ---- */
void drawExplosions(Explosion exps[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!exps[i].active) continue;

        float progress = (float)exps[i].frame / exps[i].maxFrames;
        float r = exps[i].maxRadius * progress;
        int color;

        /* Mau thay doi: YELLOW -> LIGHTRED -> RED -> DARKGRAY */
        if (progress < 0.25f)      color = YELLOW;
        else if (progress < 0.50f) color = LIGHTRED;
        else if (progress < 0.75f) color = RED;
        else                       color = DARKGRAY;

        setcolor(color);
        circle((int)exps[i].x, (int)exps[i].y, (int)r);
        if (r > 5) {
            circle((int)exps[i].x, (int)exps[i].y, (int)(r * 0.6f));
        }
        if (r > 10) {
            circle((int)exps[i].x, (int)exps[i].y, (int)(r * 0.3f));
        }
    }
}

/* ---- VE HUD ---- */
void drawHUD(int score, int hp, int level, Ship ship) {
    char buf[80];

    /* === HP Bar (goc tren trai) === */
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(15, 12, (char*)"HP:");

    /* Khung */
    setcolor(WHITE);
    rectangle(50, 10, 260, 28);

    /* Thanh HP (doi mau theo %) */
    int barW = (int)(210.0f * hp / INITIAL_HP);
    if (barW < 0) barW = 0;
    if (hp > 60)       setfillstyle(SOLID_FILL, GREEN);
    else if (hp > 30)  setfillstyle(SOLID_FILL, YELLOW);
    else               setfillstyle(SOLID_FILL, RED);
    bar(51, 11, 51 + barW, 27);

    /* So HP */
    sprintf(buf, "%d/%d", hp, INITIAL_HP);
    setcolor(WHITE);
    outtextxy(270, 12, buf);

    /* === Level (giua tren) === */
    sprintf(buf, "LEVEL %d", level);
    setcolor(LIGHTCYAN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(WIDTH / 2 - 50, 8, buf);

    /* === Score (goc tren phai) === */
    sprintf(buf, "SCORE: %d", score);
    setcolor(YELLOW);
    outtextxy(WIDTH - 230, 8, buf);

    /* === Weapon (goc duoi trai) === */
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    const char *wpnName;
    int wpnColor;
    switch (ship.weapon) {
        case WPN_SPREAD: wpnName = "SPREAD"; wpnColor = RED;       break;
        case WPN_RAPID:  wpnName = "RAPID";  wpnColor = GREEN;     break;
        case WPN_PIERCE: wpnName = "PIERCE"; wpnColor = LIGHTBLUE; break;
        default:         wpnName = "NORMAL"; wpnColor = WHITE;     break;
    }

    setcolor(wpnColor);
    sprintf(buf, "Weapon: %s", wpnName);
    outtextxy(15, HEIGHT - 30, buf);

    /* Thoi gian con lai cua power-up */
    if (ship.weapon != WPN_NORMAL) {
        unsigned long now = GetTickCount();
        float remain = 0;
        if (ship.weaponEndTime > now)
            remain = (float)(ship.weaponEndTime - now) / 1000.0f;
        sprintf(buf, "(%.1fs)", remain);
        setcolor(YELLOW);
        outtextxy(170, HEIGHT - 30, buf);
    }

    /* Reset font */
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* Ve banner "LEVEL UP!" */
void drawLevelBanner(int level) {
    char buf[32];

    /* Khung ban trong suot (semi-transparent bang cach ve box) */
    setfillstyle(SOLID_FILL, BLUE);
    bar(WIDTH / 2 - 200, HEIGHT / 2 - 60, WIDTH / 2 + 200, HEIGHT / 2 + 40);

    /* Vien */
    setcolor(LIGHTCYAN);
    rectangle(WIDTH / 2 - 200, HEIGHT / 2 - 60,
              WIDTH / 2 + 200, HEIGHT / 2 + 40);
    rectangle(WIDTH / 2 - 202, HEIGHT / 2 - 62,
              WIDTH / 2 + 202, HEIGHT / 2 + 42);

    /* Chu "LEVEL UP!" */
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    outtextxy(WIDTH / 2 - 100, HEIGHT / 2 - 45, (char*)"LEVEL UP!");

    /* So level */
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    sprintf(buf, "Level %d", level);
    outtextxy(WIDTH / 2 - 45, HEIGHT / 2 + 5, buf);

    /* Reset font */
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* Ve man hinh GAME OVER */
void drawGameOver(int score, int level) {
    char buf[80];

    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 0, WIDTH, HEIGHT);

    /* Vien do doi */
    setcolor(RED);
    rectangle(WIDTH / 2 - 280, HEIGHT / 2 - 140,
              WIDTH / 2 + 280, HEIGHT / 2 + 130);
    rectangle(WIDTH / 2 - 282, HEIGHT / 2 - 142,
              WIDTH / 2 + 282, HEIGHT / 2 + 132);

    /* GAME OVER */
    setcolor(RED);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 7);
    outtextxy(WIDTH / 2 - 210, HEIGHT / 2 - 110, (char*)"GAME OVER");

    /* Score */
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    sprintf(buf, "Final Score: %d", score);
    outtextxy(WIDTH / 2 - 130, HEIGHT / 2 - 10, buf);

    /* Level */
    setcolor(LIGHTCYAN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    sprintf(buf, "Level Reached: %d", level);
    outtextxy(WIDTH / 2 - 100, HEIGHT / 2 + 40, buf);

    /* Huong dan */
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(WIDTH / 2 - 110, HEIGHT / 2 + 90,
              (char*)"Press any key to exit...");

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* ============================================================
 *  9) VONG LAP CHINH (Main)
 * ============================================================ */

int main() {
    /* --- Khai bao bien --- */
    Ship       ship;
    Bullet     bullets[MAX_BULLETS];
    Asteroid   asteroids[MAX_ASTEROIDS];
    Item       items[MAX_ITEMS];
    Explosion  explosions[MAX_EXPLOSIONS];

    int score    = 0;
    int hp       = INITIAL_HP;
    int level    = 1;
    int gameOver = 0;

    unsigned long lastShootTime = 0;
    unsigned long lastHitTime   = 0;

    int mx = WIDTH / 2, my = HEIGHT / 2;
    int frameCount       = 0;
    int levelBannerTimer = 0;
    int page = 0;  /* trang double buffer */

    /* --- Khoi tao cua so do hoa --- */
    int gd = DETECT, gm;
    initwindow(WIDTH, HEIGHT, "Asteroid Blaster v2.0 - Ban Thien Thach");

    setbkcolor(BLACK);
    cleardevice();

    /* --- Khoi tao game --- */
    initGame(&ship, asteroids, bullets, items, explosions,
             &score, &hp, &level);

    /* ====== VONG LAP CHINH ======
     *  Moi frame:
     *    1) Input  (phim + chuot)
     *    2) Update (ship, bullets, asteroids, items, explosions, weapon timer)
     *    3) Collision
     *    4) Level-up check
     *    5) Render (double buffer)
     *    6) Delay
     */
    while (!gameOver) {
        frameCount++;

        /* Thoat bang ESC */
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;

        /* ----- 1) INPUT ----- */
        handleKeyboard(&ship);
        handleMouse(&mx, &my);

        /* ----- 2) UPDATE ----- */
        updateShip(&ship);
        tryAutoShoot(&ship, bullets, MAX_BULLETS, mx, my, &lastShootTime);
        updateBullets(bullets, MAX_BULLETS);
        updateAsteroids(asteroids, MAX_ASTEROIDS, level);
        updateItems(items, MAX_ITEMS);
        updateExplosions(explosions, MAX_EXPLOSIONS);

        /* ----- 3) COLLISION ----- */
        handleCollisions(&ship, asteroids, MAX_ASTEROIDS,
                         bullets, MAX_BULLETS,
                         &score, &hp, &lastHitTime,
                         items, explosions, level);

        /* ----- 4) LEVEL UP ----- */
        if (checkLevelUp(score, level)) {
            level++;
            updateDifficultyByLevel(level, &gCfg);
            levelBannerTimer = LEVEL_BANNER_FRAMES;

            /* Respawn thien thach xa hon (cho nguoi choi tho) */
            {
                int i;
                for (i = 0; i < MAX_ASTEROIDS; i++) {
                    initAsteroid(&asteroids[i], level);
                    asteroids[i].y = (float)(-(rand() % 300) - 200);
                }
            }
        }

        /* Kiem tra Game Over */
        if (hp <= 0) gameOver = 1;

        /* ----- 5) RENDER (Double Buffer) ----- */
        setactivepage(page);
        cleardevice();

        drawStars();
        drawAsteroids(asteroids, MAX_ASTEROIDS);
        drawItems(items, MAX_ITEMS);
        drawBullets(bullets, MAX_BULLETS);
        drawExplosions(explosions, MAX_EXPLOSIONS);
        drawShip(ship, mx, my, frameCount);
        drawHUD(score, hp, level, ship);

        /* Banner level-up */
        if (levelBannerTimer > 0) {
            drawLevelBanner(level);
            levelBannerTimer--;
        }

        setvisualpage(page);
        page = 1 - page;  /* Hoan doi trang */

        /* ----- 6) DELAY ----- */
        delay(16);  /* ~60 FPS */
    }

    /* ====== MAN HINH GAME OVER ====== */
    setactivepage(0);
    setvisualpage(0);
    drawGameOver(score, level);

    getch();
    closegraph();
    return 0;
}
