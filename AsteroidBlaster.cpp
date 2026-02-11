/*
 * ============================================================
 *  ASTEROID BLASTER - Ban Thien Thach
 *  Game C su dung WinBGIm (graphics.h)
 * ============================================================
 *
 *  HUONG DAN COMPILE:
 *  ------------------
 *  1) Dev-C++ (TDM-GCC) + WinBGIm:
 *     - Copy graphics.h, winbgim.h vao thu muc include cua compiler
 *     - Copy libbgi.a vao thu muc lib cua compiler
 *     - Project > Project Options > Parameters > Linker:
 *       -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
 *     - Hoac dung dong lenh:
 *       g++ AsteroidBlaster.cpp -o AsteroidBlaster.exe -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
 *
 *  2) CodeBlocks + WinBGIm:
 *     - Tuong tu, them cac linker flags o Settings > Compiler > Linker settings
 *
 *  DIEU KHIEN:
 *  -----------
 *  - Phim mui ten / WASD: di chuyen phi thuyen
 *  - Chuot: huong ngam ban (tu dong ban lien tuc)
 *  - ESC: thoat game
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
 *  1) HANG SO + STRUCT
 * ============================================================ */

#define WIDTH          1200      /* Chieu rong cua so */
#define HEIGHT         800       /* Chieu cao cua so  */
#define MAX_ASTEROIDS  15        /* So thien thach toi da */
#define MAX_BULLETS    50        /* So dan toi da tren man hinh */

#define SHIP_SPEED     5         /* Toc do di chuyen phi thuyen (pixel/frame) */
#define SHIP_RADIUS    18        /* Ban kinh va cham phi thuyen */
#define BULLET_SPEED   12        /* Toc do dan (pixel/frame) */
#define BULLET_RADIUS  3         /* Ban kinh vien dan */

#define SHOOT_COOLDOWN 120       /* Cooldown ban dan (ms) */
#define HIT_COOLDOWN   500       /* Cooldown nhan sat thuong (ms) */

#define INITIAL_HP     100       /* HP ban dau */
#define HIT_DAMAGE     15        /* Sat thuong khi bi thien thach cham */

#define AST_MIN_RADIUS 15        /* Ban kinh thien thach nho nhat */
#define AST_MAX_RADIUS 45        /* Ban kinh thien thach lon nhat */
#define AST_MIN_SPEED  1         /* Toc do roi nho nhat */
#define AST_MAX_SPEED  4         /* Toc do roi lon nhat */

#define PI 3.14159265358979f

/* --- Struct Phi thuyen --- */
typedef struct {
    float x, y;     /* Vi tri tam */
    int radius;     /* Ban kinh (de ve + va cham) */
    int speed;      /* Toc do di chuyen */
} Ship;

/* --- Struct Vien dan --- */
typedef struct {
    float x, y;     /* Vi tri hien tai */
    float dx, dy;   /* Vector van toc (da normalize * BULLET_SPEED) */
    int active;     /* 1 = dang bay, 0 = khong hoat dong */
    int radius;     /* Ban kinh */
} Bullet;

/* --- Struct Thien thach --- */
typedef struct {
    float x, y;     /* Vi tri tam */
    int radius;     /* Ban kinh (ngau nhien) */
    float speed;    /* Toc do roi xuong */
    int active;     /* 1 = dang hoat dong */
} Asteroid;

/* ============================================================
 *  2) KHOI TAO (Init functions)
 * ============================================================ */

/* Khoi tao phi thuyen o giua man hinh phia duoi */
void initShip(Ship *ship) {
    ship->x      = WIDTH / 2.0f;
    ship->y      = HEIGHT - 80.0f;
    ship->radius = SHIP_RADIUS;
    ship->speed  = SHIP_SPEED;
}

/* Dat 1 vien dan ve trang thai khong hoat dong */
void initBullet(Bullet *b) {
    b->x      = 0;
    b->y      = 0;
    b->dx     = 0;
    b->dy     = 0;
    b->active = 0;
    b->radius = BULLET_RADIUS;
}

/* Khoi tao 1 thien thach o vi tri ngau nhien phia tren man hinh */
void initAsteroid(Asteroid *a) {
    a->radius = AST_MIN_RADIUS + rand() % (AST_MAX_RADIUS - AST_MIN_RADIUS + 1);
    a->x      = (float)(a->radius + rand() % (WIDTH - 2 * a->radius));
    a->y      = (float)(-(rand() % 400) - a->radius);  /* Phia tren man hinh */
    a->speed  = (float)(AST_MIN_SPEED + rand() % (AST_MAX_SPEED - AST_MIN_SPEED + 1));
    /* Them 1 chut ngau nhien thap phan cho tu nhien hon */
    a->speed += (rand() % 10) / 10.0f;
    a->active = 1;
}

/* Khoi tao tat ca thien thach */
void initAllAsteroids(Asteroid ast[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        initAsteroid(&ast[i]);
    }
}

/* Khoi tao toan bo game */
void initGame(Ship *ship, Asteroid ast[], Bullet bullets[], int *score, int *hp) {
    int i;
    srand((unsigned int)time(NULL));

    initShip(ship);
    initAllAsteroids(ast, MAX_ASTEROIDS);

    for (i = 0; i < MAX_BULLETS; i++) {
        initBullet(&bullets[i]);
    }

    *score = 0;
    *hp    = INITIAL_HP;
}

/* ============================================================
 *  3) XU LY INPUT
 * ============================================================ */

/*
 * handleKeyboard: dung GetAsyncKeyState() de doc phim muot
 * Ho tro ca Arrow keys va WASD
 */
void handleKeyboard(Ship *ship) {
    /* Phim mui ten + WASD */
    if (GetAsyncKeyState(VK_LEFT)  & 0x8000 || GetAsyncKeyState('A') & 0x8000)
        ship->x -= ship->speed;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000)
        ship->x += ship->speed;
    if (GetAsyncKeyState(VK_UP)    & 0x8000 || GetAsyncKeyState('W') & 0x8000)
        ship->y -= ship->speed;
    if (GetAsyncKeyState(VK_DOWN)  & 0x8000 || GetAsyncKeyState('S') & 0x8000)
        ship->y += ship->speed;
}

/*
 * handleMouse: lay vi tri chuot tu WinBGIm
 */
void handleMouse(int *mx, int *my) {
    *mx = mousex();
    *my = mousey();
}

/* ============================================================
 *  4) LOGIC CAP NHAT (Update)
 * ============================================================ */

/* Gioi han phi thuyen trong man hinh */
void updateShip(Ship *ship) {
    if (ship->x - ship->radius < 0)
        ship->x = (float)ship->radius;
    if (ship->x + ship->radius > WIDTH)
        ship->x = (float)(WIDTH - ship->radius);
    if (ship->y - ship->radius < 0)
        ship->y = (float)ship->radius;
    if (ship->y + ship->radius > HEIGHT)
        ship->y = (float)(HEIGHT - ship->radius);
}

/*
 * tryAutoShoot: tu dong ban dan theo huong chuot
 * - Tim vien dan chua active, gan vi tri = ship, huong = ship->mouse (normalize)
 * - Chi ban khi da qua cooldown (SHOOT_COOLDOWN ms)
 */
void tryAutoShoot(Ship *ship, Bullet bullets[], int n, int mx, int my,
                  unsigned long *lastShootTime) {
    unsigned long now = GetTickCount();
    float dirX, dirY, length;
    int i;

    /* Chua het cooldown -> khong ban */
    if (now - *lastShootTime < SHOOT_COOLDOWN) return;

    /* Tinh vector huong tu ship -> mouse */
    dirX = mx - ship->x;
    dirY = my - ship->y;
    length = sqrtf(dirX * dirX + dirY * dirY);

    /* Neu chuot qua gan ship -> khong ban (tranh chia 0) */
    if (length < 5.0f) return;

    /* Chuan hoa vector */
    dirX /= length;
    dirY /= length;

    /* Tim vien dan chua hoat dong */
    for (i = 0; i < n; i++) {
        if (!bullets[i].active) {
            bullets[i].x      = ship->x;
            bullets[i].y      = ship->y;
            bullets[i].dx     = dirX * BULLET_SPEED;
            bullets[i].dy     = dirY * BULLET_SPEED;
            bullets[i].active = 1;
            bullets[i].radius = BULLET_RADIUS;
            *lastShootTime    = now;
            break;
        }
    }
}

/* Cap nhat vi tri tat ca dan; tat dan khi ra khoi man hinh */
void updateBullets(Bullet bullets[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!bullets[i].active) continue;

        bullets[i].x += bullets[i].dx;
        bullets[i].y += bullets[i].dy;

        /* Ra khoi man hinh -> tat */
        if (bullets[i].x < -10 || bullets[i].x > WIDTH + 10 ||
            bullets[i].y < -10 || bullets[i].y > HEIGHT + 10) {
            bullets[i].active = 0;
        }
    }
}

/* Cap nhat thien thach: roi xuong, respawn khi ra khoi man hinh */
void updateAsteroids(Asteroid ast[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!ast[i].active) continue;

        ast[i].y += ast[i].speed;

        /* Ra khoi man hinh phia duoi -> respawn lai phia tren */
        if (ast[i].y - ast[i].radius > HEIGHT) {
            initAsteroid(&ast[i]);
        }
    }
}

/*
 * Kiem tra va cham 2 hinh tron:
 * Tra ve 1 neu khoang cach tam < tong ban kinh, nguoc lai 0
 */
int checkCircleCollision(float x1, float y1, float r1,
                         float x2, float y2, float r2) {
    float dx   = x2 - x1;
    float dy   = y2 - y1;
    float dist = sqrtf(dx * dx + dy * dy);
    return (dist < r1 + r2) ? 1 : 0;
}

/*
 * handleCollisions: xu ly:
 *   - Dan trung thien thach -> +diem, respawn thien thach
 *   - Ship cham thien thach -> -HP (co cooldown)
 */
void handleCollisions(Ship *ship, Asteroid ast[], int na,
                      Bullet bullets[], int nb,
                      int *score, int *hp,
                      unsigned long *lastHitTime) {
    int i, j;
    unsigned long now = GetTickCount();

    /* --- Dan vs Thien thach --- */
    for (i = 0; i < nb; i++) {
        if (!bullets[i].active) continue;

        for (j = 0; j < na; j++) {
            if (!ast[j].active) continue;

            if (checkCircleCollision(bullets[i].x, bullets[i].y, (float)bullets[i].radius,
                                     ast[j].x,     ast[j].y,     (float)ast[j].radius)) {
                /* Tinh diem: thien thach cang lon cang it diem (nho = kho ban) */
                if (ast[j].radius <= 20)
                    *score += 30;       /* Nho -> 30 diem */
                else if (ast[j].radius <= 35)
                    *score += 20;       /* Vua -> 20 diem */
                else
                    *score += 10;       /* Lon -> 10 diem */

                /* Respawn thien thach */
                initAsteroid(&ast[j]);

                /* Tat vien dan */
                bullets[i].active = 0;
                break;  /* 1 vien dan chi trung 1 thien thach */
            }
        }
    }

    /* --- Ship vs Thien thach --- */
    for (j = 0; j < na; j++) {
        if (!ast[j].active) continue;

        if (checkCircleCollision(ship->x, ship->y, (float)ship->radius,
                                 ast[j].x, ast[j].y, (float)ast[j].radius)) {
            /* Chi tru HP khi da qua cooldown */
            if (now - *lastHitTime >= HIT_COOLDOWN) {
                *hp -= HIT_DAMAGE;
                if (*hp < 0) *hp = 0;
                *lastHitTime = now;
            }
            /* Respawn thien thach sau va cham */
            initAsteroid(&ast[j]);
        }
    }
}

/* ============================================================
 *  5) VE (Render)
 * ============================================================ */

/*
 * drawShip: ve phi thuyen dang tam giac + duong ngam huong ve chuot
 *  - Than: tam giac mau CYAN
 *  - Mui: chi ve phia chuot
 *  - Duong ngam: net dut tu ship -> chuot
 */
void drawShip(Ship ship, int mx, int my) {
    float angle;
    int x1, y1, x2, y2, x3, y3;
    int ax, ay;
    float dirX, dirY, len;

    /* Tinh goc tu ship -> mouse */
    angle = atan2f((float)(my - ship.y), (float)(mx - ship.x));

    /* 3 dinh cua tam giac (mui quay ve phia chuot) */
    /* Dinh mui (phia truoc) */
    x1 = (int)(ship.x + cosf(angle) * (ship.radius + 10));
    y1 = (int)(ship.y + sinf(angle) * (ship.radius + 10));

    /* 2 dinh phia sau (mo rong 2 ben) */
    x2 = (int)(ship.x + cosf(angle + 2.5f) * ship.radius);
    y2 = (int)(ship.y + sinf(angle + 2.5f) * ship.radius);

    x3 = (int)(ship.x + cosf(angle - 2.5f) * ship.radius);
    y3 = (int)(ship.y + sinf(angle - 2.5f) * ship.radius);

    /* Ve than phi thuyen (tam giac) */
    setcolor(LIGHTCYAN);
    setfillstyle(SOLID_FILL, CYAN);
    int poly[] = { x1, y1, x2, y2, x3, y3, x1, y1 };
    fillpoly(4, poly);

    /* Ve vien sang */
    setcolor(WHITE);
    line(x1, y1, x2, y2);
    line(x1, y1, x3, y3);
    line(x2, y2, x3, y3);

    /* Ve dong co (nho o phia sau) */
    ax = (int)(ship.x - cosf(angle) * (ship.radius + 5));
    ay = (int)(ship.y - sinf(angle) * (ship.radius + 5));
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, LIGHTRED);
    /* Ngon lua dong co */
    int flame[] = {
        (int)(ship.x - cosf(angle) * ship.radius * 0.5f + cosf(angle + 1.57f) * 5),
        (int)(ship.y - sinf(angle) * ship.radius * 0.5f + sinf(angle + 1.57f) * 5),
        ax, ay,
        (int)(ship.x - cosf(angle) * ship.radius * 0.5f - cosf(angle + 1.57f) * 5),
        (int)(ship.y - sinf(angle) * ship.radius * 0.5f - sinf(angle + 1.57f) * 5),
        (int)(ship.x - cosf(angle) * ship.radius * 0.5f + cosf(angle + 1.57f) * 5),
        (int)(ship.y - sinf(angle) * ship.radius * 0.5f + sinf(angle + 1.57f) * 5)
    };
    fillpoly(4, flame);

    /* Ve duong ngam (dashed line) tu ship -> chuot */
    setcolor(DARKGRAY);
    setlinestyle(DASHED_LINE, 0, 1);
    dirX = (float)(mx - ship.x);
    dirY = (float)(my - ship.y);
    len = sqrtf(dirX * dirX + dirY * dirY);
    if (len > 1.0f) {
        /* Ve duong ngam dai 80 pixel */
        int aimX = (int)(ship.x + (dirX / len) * 80.0f);
        int aimY = (int)(ship.y + (dirY / len) * 80.0f);
        line((int)ship.x, (int)ship.y, aimX, aimY);
    }
    setlinestyle(SOLID_LINE, 0, 1);  /* Reset ve net lien */

    /* Ve crosshair nho tai vi tri chuot */
    setcolor(LIGHTGREEN);
    circle(mx, my, 8);
    line(mx - 12, my, mx + 12, my);
    line(mx, my - 12, mx, my + 12);
}

/* Ve tat ca vien dan dang hoat dong */
void drawBullets(Bullet bullets[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!bullets[i].active) continue;

        /* Vien dan mau vang sang */
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse((int)bullets[i].x, (int)bullets[i].y,
                     bullets[i].radius, bullets[i].radius);
    }
}

/*
 * drawAsteroids: ve thien thach dang da goc (polygon ngau nhien)
 * Moi thien thach co hinh dang khac nhau de nhin tu nhien hon
 */
void drawAsteroids(Asteroid ast[], int n) {
    int i, j;
    int numVertices = 8;  /* So dinh cua polygon */

    for (i = 0; i < n; i++) {
        if (!ast[i].active) continue;

        /* Tao polygon khong deu de nhin giong thien thach */
        int poly[18];  /* numVertices * 2 + 2 (dong polygon) */
        for (j = 0; j < numVertices; j++) {
            float a = (float)j / numVertices * 2.0f * PI;
            /* Dao dong ban kinh +/- 20% de tao hinh goc canh */
            float r = ast[i].radius * (0.8f + (rand() % 40) / 100.0f);
            poly[j * 2]     = (int)(ast[i].x + cosf(a) * r);
            poly[j * 2 + 1] = (int)(ast[i].y + sinf(a) * r);
        }
        /* Dong polygon */
        poly[numVertices * 2]     = poly[0];
        poly[numVertices * 2 + 1] = poly[1];

        /* Mau sac tuy theo kich thuoc */
        if (ast[i].radius <= 20) {
            setcolor(LIGHTGRAY);
            setfillstyle(SOLID_FILL, DARKGRAY);
        } else if (ast[i].radius <= 35) {
            setcolor(BROWN);
            setfillstyle(SOLID_FILL, RED);
        } else {
            setcolor(LIGHTRED);
            setfillstyle(SOLID_FILL, BROWN);
        }

        fillpoly(numVertices + 1, poly);

        /* Ve vien ngoai */
        setcolor(WHITE);
        for (j = 0; j < numVertices; j++) {
            int next = (j + 1) % numVertices;
            line(poly[j * 2], poly[j * 2 + 1],
                 poly[next * 2], poly[next * 2 + 1]);
        }
    }
}

/* Ve HUD (Head-Up Display): HP ben trai, Score ben phai */
void drawHUD(int score, int hp) {
    char buf[64];

    /* === HP Bar (goc tren trai) === */
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(15, 12, (char*)"HP:");

    /* Khung HP bar */
    setcolor(WHITE);
    rectangle(50, 10, 250, 28);

    /* Thanh HP (mau thay doi theo HP) */
    int barWidth = (int)(200.0f * hp / INITIAL_HP);
    if (barWidth < 0) barWidth = 0;
    if (hp > 60)
        setfillstyle(SOLID_FILL, GREEN);
    else if (hp > 30)
        setfillstyle(SOLID_FILL, YELLOW);
    else
        setfillstyle(SOLID_FILL, RED);
    bar(51, 11, 51 + barWidth, 27);

    /* Hien so HP */
    sprintf(buf, "%d/%d", hp, INITIAL_HP);
    setcolor(WHITE);
    outtextxy(260, 12, buf);

    /* === Score (goc tren phai) === */
    sprintf(buf, "SCORE: %d", score);
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(WIDTH - 220, 10, buf);

    /* Reset font */
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* Ve man hinh GAME OVER */
void drawGameOver(int score) {
    char buf[64];

    /* Lam toi man hinh */
    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 0, WIDTH, HEIGHT);

    /* Hieu ung vien do */
    setcolor(RED);
    rectangle(WIDTH / 2 - 260, HEIGHT / 2 - 120, WIDTH / 2 + 260, HEIGHT / 2 + 100);
    rectangle(WIDTH / 2 - 262, HEIGHT / 2 - 122, WIDTH / 2 + 262, HEIGHT / 2 + 102);

    /* Chu GAME OVER */
    setcolor(RED);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 6);
    outtextxy(WIDTH / 2 - 180, HEIGHT / 2 - 90, (char*)"GAME OVER");

    /* Hien diem */
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    sprintf(buf, "Score: %d", score);
    outtextxy(WIDTH / 2 - 100, HEIGHT / 2, buf);

    /* Huong dan */
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(WIDTH / 2 - 120, HEIGHT / 2 + 60, (char*)"Press any key to exit...");

    /* Reset font */
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* Ve nen sao (tao hieu ung khong gian) */
void drawStars(void) {
    /* Ve ngoi sao co dinh (dung seed co dinh de khong nhap nhay) */
    static int starX[80], starY[80], starInit = 0;
    int i;
    if (!starInit) {
        for (i = 0; i < 80; i++) {
            starX[i] = rand() % WIDTH;
            starY[i] = rand() % HEIGHT;
        }
        starInit = 1;
    }
    for (i = 0; i < 80; i++) {
        /* Ngoi sao sang toi xen ke */
        if (i % 3 == 0)
            putpixel(starX[i], starY[i], WHITE);
        else if (i % 3 == 1)
            putpixel(starX[i], starY[i], LIGHTGRAY);
        else
            putpixel(starX[i], starY[i], DARKGRAY);
    }
}

/* ============================================================
 *  6) VONG LAP GAME (Main)
 * ============================================================ */

int main() {
    /* --- Bien game --- */
    Ship ship;
    Bullet bullets[MAX_BULLETS];
    Asteroid asteroids[MAX_ASTEROIDS];
    int score = 0;
    int hp    = INITIAL_HP;
    int gameOver = 0;

    /* Bien thoi gian cooldown */
    unsigned long lastShootTime = 0;
    unsigned long lastHitTime   = 0;

    /* Bien chuot */
    int mx = WIDTH / 2, my = HEIGHT / 2;

    /* --- Khoi tao cua so do hoa --- */
    int gd = DETECT, gm;
    initwindow(WIDTH, HEIGHT, "Asteroid Blaster - Ban Thien Thach");

    /* Nen den (khong gian) */
    setbkcolor(BLACK);
    cleardevice();

    /* --- Khoi tao game --- */
    initGame(&ship, asteroids, bullets, &score, &hp);

    /* === VONG LAP CHINH === */
    /*
     * Moi frame:
     *   1) Xu ly input (phim + chuot)
     *   2) Cap nhat logic (ship, bullets, asteroids)
     *   3) Xu ly va cham
     *   4) Ve lai man hinh
     *   5) delay(16) ~ 60fps
     */
    while (!gameOver) {

        /* --- Kiem tra thoat game (ESC) --- */
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            break;

        /* ----- 1) INPUT ----- */
        handleKeyboard(&ship);
        handleMouse(&mx, &my);

        /* ----- 2) UPDATE ----- */
        updateShip(&ship);
        tryAutoShoot(&ship, bullets, MAX_BULLETS, mx, my, &lastShootTime);
        updateBullets(bullets, MAX_BULLETS);
        updateAsteroids(asteroids, MAX_ASTEROIDS);

        /* ----- 3) COLLISION ----- */
        handleCollisions(&ship, asteroids, MAX_ASTEROIDS,
                         bullets, MAX_BULLETS,
                         &score, &hp, &lastHitTime);

        /* ----- Kiem tra Game Over ----- */
        if (hp <= 0) {
            gameOver = 1;
        }

        /* ----- 4) RENDER (Double Buffer) ----- */
        /* Su dung visual page va active page de tranh nhap nhay */
        setactivepage(1);      /* Ve vao trang an */
        setvisualpage(0);      /* Hien trang hien tai */

        cleardevice();         /* Xoa trang an */

        /* Ve nen sao */
        drawStars();

        /* Ve cac thanh phan game */
        drawAsteroids(asteroids, MAX_ASTEROIDS);
        drawBullets(bullets, MAX_BULLETS);
        drawShip(ship, mx, my);
        drawHUD(score, hp);

        /* Hoan doi 2 trang (swap buffers) */
        setactivepage(0);
        setvisualpage(1);

        /* ----- 5) DELAY ----- */
        delay(16);  /* ~60 FPS */
    }

    /* === MAN HINH GAME OVER === */
    /* Ve truc tiep, khong can double buffer */
    setactivepage(0);
    setvisualpage(0);
    drawGameOver(score);

    /* Cho nguoi choi bam phim bat ky */
    getch();

    /* Dong cua so do hoa */
    closegraph();
    return 0;
}
