#ifndef BACKGROUND_LEVEL_H
#define BACKGROUND_LEVEL_H

/* drawStars(): Ve cac ngoi sao nen voi hieu ung nhap nhay */
void drawStars(void) {
  int i, c, phase, brightness;
  gStarFrame++; /* Tang frame moi lan ve */
  for (i = 0; i < MAX_STARS; i++) {
    /* Tinh pha hien tai cua sao: ket hop frame toan cuc va pha rieng */
    phase = (gStarFrame + sPhase[i]) % 60;
    /* brightness: 0->30 tang sang, 30->60 giam sang (hieu ung tho) */
    brightness = (phase < 30) ? phase : (60 - phase);

    if (brightness < 8) {
      /* Toi nhat: chi 1 pixel xam */
      putpixel(sX[i], sY[i], DARKGRAY);
    } else if (brightness < 16) {
      /* Hoi sang: 1 pixel xam nhat */
      putpixel(sX[i], sY[i], LIGHTGRAY);
    } else if (brightness < 22) {
      /* Sang: 1 pixel trang + glow xam xung quanh */
      c = WHITE;
      putpixel(sX[i], sY[i], c);
      putpixel(sX[i] - 1, sY[i], DARKGRAY);
      putpixel(sX[i] + 1, sY[i], DARKGRAY);
      putpixel(sX[i], sY[i] - 1, DARKGRAY);
      putpixel(sX[i], sY[i] + 1, DARKGRAY);
    } else {
      /* Sang nhat: hinh chu thap sang + glow */
      c = WHITE;
      putpixel(sX[i], sY[i], c);
      putpixel(sX[i] - 1, sY[i], LIGHTGRAY);
      putpixel(sX[i] + 1, sY[i], LIGHTGRAY);
      putpixel(sX[i], sY[i] - 1, LIGHTGRAY);
      putpixel(sX[i], sY[i] + 1, LIGHTGRAY);
      /* Tia xa hon */
      if (brightness > 26) {
        putpixel(sX[i] - 2, sY[i], DARKGRAY);
        putpixel(sX[i] + 2, sY[i], DARKGRAY);
        putpixel(sX[i], sY[i] - 2, DARKGRAY);
        putpixel(sX[i], sY[i] + 2, DARKGRAY);
      }
    }
  }
}

/* ======================================================================
 * drawLevel2Background(): He Mat Troi ruc lua + Phi thuyen Alien
 * ====================================================================== */
void drawLevel2Background(void) {
  int i, r;
  drawStars();

  /* --- Hao quang Mat Troi (Corona) --- */
  for (r = 220; r >= 30; r -= 20) {
    int c;
    if (r > 180)
      c = DARKGRAY;
    else if (r > 140)
      c = RED;
    else if (r > 100)
      c = LIGHTRED;
    else if (r > 60)
      c = YELLOW;
    else
      c = WHITE;
    setcolor(c);
    circle(0, 0, r);
  }

  /* Than Mat Troi */
  setfillstyle(SOLID_FILL, RED);
  fillellipse(0, 0, 200, 200);
  setfillstyle(SOLID_FILL, LIGHTRED);
  fillellipse(0, 0, 140, 140);
  setfillstyle(SOLID_FILL, YELLOW);
  fillellipse(0, 0, 80, 80);
  setfillstyle(SOLID_FILL, WHITE);
  fillellipse(0, 0, 30, 30);

  /* --- Tia sang Mat Troi (Solar Flares) --- */
  for (i = 0; i < 12; i++) {
    float angle = i * 30.0f * 3.14159f / 180.0f;
    int x1 = (int)(cosf(angle) * 210);
    int y1 = (int)(sinf(angle) * 210);
    int x2 = (int)(cosf(angle) * 280);
    int y2 = (int)(sinf(angle) * 280);
    setcolor((i % 2) ? YELLOW : LIGHTRED);
    line(x1, y1, x2, y2);
  }

  /* --- Quy dao hanh tinh --- */
  setcolor(DARKGRAY);
  setlinestyle(DASHED_LINE, 0, 1);
  for (r = 250; r <= 850; r += 150) {
    ellipse(0, 0, 0, 90, r, r);
  }
  setlinestyle(SOLID_LINE, 0, 1);

  /* --- Cac hanh tinh voi chi tiet --- */
  {
    int pOrbit[] = {250, 400, 550, 700, 850};
    int pAngle[] = {75, 60, 45, 30, 15};
    int pColor[] = {LIGHTGRAY, YELLOW, CYAN, LIGHTRED, BROWN};
    for (i = 0; i < 5; i++) {
      float rad = (float)pAngle[i] * 3.14159f / 180.0f;
      int px = (int)(cosf(rad) * pOrbit[i]);
      int py = (int)(sinf(rad) * pOrbit[i]);

      /* Bong hanh tinh */
      setfillstyle(SOLID_FILL, DARKGRAY);
      fillellipse(px + 2, py + 2, 13, 13);

      /* Than hanh tinh */
      setfillstyle(SOLID_FILL, pColor[i]);
      fillellipse(px, py, 12, 12);

      /* Highlight 3D */
      setfillstyle(SOLID_FILL, WHITE);
      fillellipse(px - 4, py - 4, 3, 3);

      /* Vanh dai Sao Tho */
      if (i == 4) {
        setcolor(LIGHTGRAY);
        ellipse(px, py, 0, 360, 22, 8);
        ellipse(px, py, 0, 360, 20, 7);
      }
    }
  }

  /* --- Phi thuyen Alien bay qua (UFO) --- */
  {
    int ux = WIDTH - 200, uy = 150;
    /* Than UFO */
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    fillellipse(ux, uy, 35, 12);
    /* Cabin */
    setfillstyle(SOLID_FILL, CYAN);
    fillellipse(ux, uy - 8, 15, 10);
    /* Den UFO */
    setcolor(YELLOW);
    circle(ux - 10, uy + 10, 3);
    circle(ux, uy + 10, 3);
    circle(ux + 10, uy + 10, 3);
    /* Tia sang */
    setcolor(LIGHTCYAN);
    line(ux, uy + 12, ux - 20, uy + 40);
    line(ux, uy + 12, ux + 20, uy + 40);
  }
}

/* ======================================================================
 * drawLevel3Background(): Tho Tinh (Saturn) & Tinh Van Nebula Ruc Ro
 * ====================================================================== */
void drawLevel3Background(void) {
  int i, r;
  unsigned int seed;
  drawStars();

  /* --- Tinh van Nebula day mau sac --- */
  seed = 12345u;
  for (i = 0; i < 400; i++) {
    int nx, ny, dotColor;
    seed = seedRand(seed, i);
    nx = seed % WIDTH;
    ny = seed % HEIGHT;

    /* Tao dam may tinh van o nhieu vi tri */
    if ((nx < 450 && ny < 450) || (nx > 750 && ny > 250)) {
      dotColor = (i % 5 == 0)   ? MAGENTA
                 : (i % 5 == 1) ? CYAN
                 : (i % 5 == 2) ? BLUE
                 : (i % 5 == 3) ? LIGHTMAGENTA
                                : LIGHTCYAN;
      putpixel(nx, ny, dotColor);
      /* Them diem sang xung quanh */
      if (i % 3 == 0) {
        putpixel(nx + 1, ny, dotColor);
        putpixel(nx, ny + 1, dotColor);
      }
    }
  }

  /* --- Sao Tho (Saturn) khong lo --- */
  {
    int sx = WIDTH / 2;
    int sy = HEIGHT / 2;

    /* Bong hanh tinh */
    setfillstyle(SOLID_FILL, DARKGRAY);
    fillellipse(sx + 5, sy + 5, 108, 98);

    /* Than hanh tinh - nhieu lop tao hieu ung 3D */
    setfillstyle(SOLID_FILL, BROWN);
    fillellipse(sx, sy, 105, 95);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(sx, sy, 90, 80);
    setfillstyle(SOLID_FILL, LIGHTRED);
    fillellipse(sx - 10, sy - 10, 40, 35);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(sx - 25, sy - 25, 20, 15);

    /* Vanh dai (Rings) - nhieu lop */
    setcolor(DARKGRAY);
    for (r = 140; r < 200; r += 3) {
      ellipse(sx, sy, 20, 160, r, r / 3);
    }
    setcolor(LIGHTGRAY);
    for (r = 140; r < 200; r += 3) {
      ellipse(sx, sy, 200, 340, r, r / 3);
    }
    /* Vanh sang nhat */
    setcolor(WHITE);
    ellipse(sx, sy, 200, 340, 160, 53);
  }

  /* --- Cac ngoi sao sang --- */
  for (i = 0; i < 20; i++) {
    int sx = (i * 73) % WIDTH;
    int sy = (i * 97) % HEIGHT;
    setcolor(WHITE);
    putpixel(sx, sy, WHITE);
    putpixel(sx + 1, sy, WHITE);
    putpixel(sx, sy + 1, WHITE);
    putpixel(sx - 1, sy, WHITE);
    putpixel(sx, sy - 1, WHITE);
  }

  /* --- Phi thuyen Alien nho --- */
  {
    int ux = 150, uy = HEIGHT - 150;
    setfillstyle(SOLID_FILL, DARKGRAY);
    fillellipse(ux, uy, 25, 8);
    setfillstyle(SOLID_FILL, LIGHTCYAN);
    fillellipse(ux, uy - 5, 10, 7);
    setcolor(YELLOW);
    circle(ux - 8, uy + 6, 2);
    circle(ux + 8, uy + 6, 2);
  }
}

/* ======================================================================
 * drawLevel4Background(): Trai Dat + Mat Trang + Tram Vu Tru
 * ====================================================================== */
void drawLevel4Background(void) {
  int i;
  drawStars();

  /* --- Trai Dat o goc duoi phai --- */
  int ex = WIDTH - 150;
  int ey = HEIGHT + 80;

  /* Bong Trai Dat */
  setfillstyle(SOLID_FILL, DARKGRAY);
  fillellipse(ex + 5, ey + 5, 185, 185);

  /* Dai duong */
  setfillstyle(SOLID_FILL, BLUE);
  fillellipse(ex, ey, 180, 180);

  /* Luc dia chi tiet */
  setfillstyle(SOLID_FILL, GREEN);
  fillellipse(ex - 40, ey - 50, 60, 50);
  fillellipse(ex + 30, ey - 20, 50, 40);
  fillellipse(ex - 20, ey + 40, 45, 35);
  fillellipse(ex + 50, ey + 30, 35, 30);

  /* May trang */
  setfillstyle(SOLID_FILL, WHITE);
  fillellipse(ex - 60, ey - 80, 20, 12);
  fillellipse(ex + 40, ey - 90, 25, 15);

  /* --- Mat Trang --- */
  int mx = 120, my = 120;

  /* Bong Mat Trang */
  setfillstyle(SOLID_FILL, DARKGRAY);
  fillellipse(mx + 3, my + 3, 73, 73);

  /* Than Mat Trang */
  setfillstyle(SOLID_FILL, LIGHTGRAY);
  fillellipse(mx, my, 70, 70);

  /* Cac mieng nui lua */
  setfillstyle(SOLID_FILL, DARKGRAY);
  fillellipse(mx - 20, my - 15, 12, 12);
  fillellipse(mx + 15, my + 10, 15, 15);
  fillellipse(mx + 10, my - 25, 8, 8);
  fillellipse(mx - 25, my + 20, 10, 10);
  fillellipse(mx + 30, my - 10, 7, 7);

  /* --- Tram Vu Tru (Space Station) --- */
  {
    int stx = WIDTH / 2 + 200, sty = 100;

    /* Than chinh */
    setcolor(LIGHTGRAY);
    rectangle(stx - 30, sty - 10, stx + 30, sty + 10);
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(stx - 30, sty - 10, stx + 30, sty + 10);

    /* Tam giac */
    setfillstyle(SOLID_FILL, CYAN);
    int panel[] = {stx - 50, sty,      stx - 30, sty - 20,
                   stx - 30, sty + 20, stx - 50, sty};
    fillpoly(4, panel);
    int panel2[] = {stx + 50, sty,      stx + 30, sty - 20,
                    stx + 30, sty + 20, stx + 50, sty};
    fillpoly(4, panel2);

    /* Den */
    setcolor(YELLOW);
    circle(stx - 20, sty, 3);
    circle(stx, sty, 3);
    circle(stx + 20, sty, 3);
  }

  /* --- Sao bang --- */
  for (i = 0; i < 3; i++) {
    int sx = 300 + i * 200;
    int sy = 200 + i * 100;
    setcolor(WHITE);
    line(sx, sy, sx + 30, sy + 15);
    line(sx + 5, sy + 3, sx + 35, sy + 18);
    setcolor(YELLOW);
    line(sx + 10, sy + 6, sx + 40, sy + 21);
  }
}

/* ======================================================================
 * drawLevel5Background(): Sao Hoa + Giai Ngan Ha Spiral
 * ====================================================================== */
void drawLevel5Background(void) {
  int i, r;
  drawStars();

  /* --- Giai Ngan Ha Spiral (Milky Way) --- */
  {
    int gx = WIDTH - 250, gy = 150;

    /* Tam giai ngan ha */
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(gx, gy, 15, 15);

    /* Cac vong xoay oc */
    for (r = 25; r < 120; r += 15) {
      int c = (r < 50) ? WHITE : (r < 80) ? LIGHTCYAN : CYAN;
      setcolor(c);
      ellipse(gx, gy, 0, 360, r, r / 2);
      ellipse(gx, gy, 0, 360, r, r / 3);
    }

    /* Cac ngoi sao trong giai ngan ha */
    unsigned int seed = 99999u;
    for (i = 0; i < 100; i++) {
      seed = seedRand(seed, i);
      int sx = gx + (seed % 200) - 100;
      int sy = gy + ((seed / 7) % 100) - 50;
      int dist = (sx - gx) * (sx - gx) + (sy - gy) * (sy - gy);
      if (dist < 10000) {
        setcolor((i % 3) ? WHITE : YELLOW);
        putpixel(sx, sy, (i % 3) ? WHITE : YELLOW);
      }
    }
  }

  /* --- Sao Hoa (Venus) --- */
  int vx = 200, vy = HEIGHT / 2;

  /* Bong */
  setfillstyle(SOLID_FILL, DARKGRAY);
  fillellipse(vx + 5, vy + 5, 125, 125);

  /* Than hanh tinh */
  setfillstyle(SOLID_FILL, BROWN);
  fillellipse(vx, vy, 120, 120);
  setfillstyle(SOLID_FILL, YELLOW);
  fillellipse(vx, vy, 100, 100);
  setfillstyle(SOLID_FILL, LIGHTRED);
  fillellipse(vx - 20, vy - 20, 40, 40);
  setfillstyle(SOLID_FILL, WHITE);
  fillellipse(vx - 30, vy - 30, 25, 25);

  /* --- Tieu hanh tinh bay roi rac --- */
  {
    int astPos[][2] = {{500, 150},  {750, 400}, {950, 200}, {600, 550},
                       {1050, 500}, {400, 300}, {850, 150}};
    for (i = 0; i < 7; i++) {
      int ax = astPos[i][0];
      int ay = astPos[i][1];
      int ar = 15 + (i * 7) % 25;

      /* Bong */
      setfillstyle(SOLID_FILL, BLACK);
      fillellipse(ax + 2, ay + 2, ar + 2, ar + 2);

      /* Than */
      setfillstyle(SOLID_FILL, DARKGRAY);
      fillellipse(ax, ay, ar, ar);
      setcolor(LIGHTGRAY);
      circle(ax, ay, ar);

      /* Mieng nui lua */
      setcolor(DARKGRAY);
      circle(ax - ar / 3, ay - ar / 3, ar / 4);
    }
  }

  /* --- Phi thuyen Alien lon --- */
  {
    int ux = WIDTH / 2, uy = HEIGHT - 100;

    /* Bong UFO */
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(ux + 3, uy + 3, 48, 17);

    /* Than UFO */
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    fillellipse(ux, uy, 45, 15);

    /* Cabin */
    setfillstyle(SOLID_FILL, CYAN);
    fillellipse(ux, uy - 10, 20, 12);
    setfillstyle(SOLID_FILL, LIGHTCYAN);
    fillellipse(ux, uy - 12, 15, 8);

    /* Den UFO */
    setcolor(YELLOW);
    for (i = -2; i <= 2; i++) {
      circle(ux + i * 12, uy + 12, 3);
    }

    /* Tia sang xuong duoi */
    setcolor(LIGHTCYAN);
    line(ux - 15, uy + 15, ux - 35, uy + 60);
    line(ux + 15, uy + 15, ux + 35, uy + 60);
    line(ux, uy + 15, ux, uy + 65);
  }
}

/* ======================================================================
 * drawLevel6Background(): Lo Den (Black Hole) + Tinh Van + Vo Cuc
 * ====================================================================== */
void drawLevel6Background(void) {
  int i, r;
  drawStars();

  /* --- Tinh van vo cuc phia sau --- */
  {
    unsigned int seed = 77777u;
    for (i = 0; i < 500; i++) {
      seed = seedRand(seed, i);
      int nx = seed % WIDTH;
      int ny = seed % HEIGHT;
      int c = (i % 7 == 0)   ? MAGENTA
              : (i % 7 == 1) ? CYAN
              : (i % 7 == 2) ? BLUE
              : (i % 7 == 3) ? LIGHTMAGENTA
              : (i % 7 == 4) ? LIGHTCYAN
              : (i % 7 == 5) ? LIGHTBLUE
                             : DARKGRAY;
      putpixel(nx, ny, c);
    }
  }

  /* --- Lo Den o chinh giua --- */
  int bx = WIDTH / 2;
  int by = HEIGHT / 2;

  /* Hao quang xung quanh lo den */
  for (r = 220; r > 180; r -= 5) {
    setcolor(DARKGRAY);
    circle(bx, by, r);
  }

  /* Dia tich tu (Accretion Disk) - nhieu lop */
  for (r = 180; r > 40; r -= 8) {
    int c;
    if (r > 160)
      c = BLUE;
    else if (r > 140)
      c = CYAN;
    else if (r > 120)
      c = LIGHTCYAN;
    else if (r > 100)
      c = MAGENTA;
    else if (r > 80)
      c = LIGHTMAGENTA;
    else if (r > 60)
      c = RED;
    else
      c = DARKGRAY;

    setcolor(c);
    ellipse(bx, by, 0, 360, r, r / 2);
    ellipse(bx, by, 0, 360, r - 2, r / 2 - 1);
  }

  /* Tam lo den - den tuyet doi */
  setfillstyle(SOLID_FILL, BLACK);
  fillellipse(bx, by, 40, 40);

  /* Event Horizon */
  setcolor(DARKGRAY);
  circle(bx, by, 40);
  circle(bx, by, 41);
  circle(bx, by, 42);

  /* --- Cac ngoi sao bi hut vao --- */
  for (i = 0; i < 15; i++) {
    float angle = i * 24.0f * 3.14159f / 180.0f;
    int dist = 60 + (i * 13) % 80;
    int sx = bx + (int)(cosf(angle) * dist);
    int sy = by + (int)(sinf(angle) * dist);

    setcolor((i % 2) ? YELLOW : WHITE);
    circle(sx, sy, 2);

    /* Duong keo dai bi hut */
    setcolor(DARKGRAY);
    line(sx, sy, bx, by);
  }

  /* --- Phi thuyen Alien chay tron --- */
  {
    int ux = 150, uy = 150;

    /* Than UFO */
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    fillellipse(ux, uy, 30, 10);

    /* Cabin */
    setfillstyle(SOLID_FILL, LIGHTCYAN);
    fillellipse(ux, uy - 6, 12, 8);

    /* Den canh bao */
    setcolor(RED);
    circle(ux - 10, uy + 8, 2);
    circle(ux + 10, uy + 8, 2);

    /* Duong di chuyen */
    setcolor(DARKGRAY);
    setlinestyle(DASHED_LINE, 0, 1);
    line(ux, uy, ux + 100, uy - 50);
    setlinestyle(SOLID_LINE, 0, 1);
  }

  /* --- Sao bang bay qua --- */
  for (i = 0; i < 5; i++) {
    int sx = 100 + i * 250;
    int sy = 500 + i * 40;
    setcolor(WHITE);
    line(sx, sy, sx + 40, sy + 20);
    setcolor(YELLOW);
    line(sx + 5, sy + 2, sx + 45, sy + 22);
    setcolor(LIGHTCYAN);
    line(sx + 10, sy + 5, sx + 50, sy + 25);
  }

  /* --- Tinh van vo cuc (Infinity Symbol) --- */
  {
    int ix = WIDTH - 200, iy = HEIGHT - 150;
    setcolor(CYAN);
    ellipse(ix - 30, iy, 0, 360, 40, 25);
    ellipse(ix + 30, iy, 0, 360, 40, 25);
    setcolor(LIGHTCYAN);
    ellipse(ix - 30, iy, 0, 360, 35, 20);
    ellipse(ix + 30, iy, 0, 360, 35, 20);
  }
}

/* ======================================================================
 * drawBackground(): Ham dieu phoi tong
 * ====================================================================== */
void drawBackground(int level) {
  switch (level) {
  case 1:
    drawStars();
    break;
  case 2:
    drawLevel2Background();
    break;
  case 3:
    drawLevel3Background();
    break;
  case 4:
    drawLevel4Background();
    break;
  case 5:
    drawLevel5Background();
    break;
  case 6:
    drawLevel6Background();
    break;
  default:
    /* Level 7+: Xoay vong lai cac background */
    {
      int bgIndex = ((level - 1) % 6) + 1; /* 1-6 */
      drawBackground(bgIndex);
    }
    break;
  }
}

#endif
