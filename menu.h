#ifndef MENU_H
#define MENU_H

/* drawMenuBackground(): Ve background dep cho menu voi hanh tinh, phi thuyen, hieu ung */
void drawMenuBackground(void) {
  int i;
  drawStars();

  /* --- Hanh tinh lon o goc duoi phai --- */
  setfillstyle(SOLID_FILL, BLUE);
  fillellipse(WIDTH - 100, HEIGHT + 50, 180, 180);
  setfillstyle(SOLID_FILL, GREEN);
  fillellipse(WIDTH - 130, HEIGHT + 10, 50, 40);
  fillellipse(WIDTH - 60, HEIGHT + 60, 40, 30);
  setfillstyle(SOLID_FILL, WHITE);
  fillellipse(WIDTH - 160, HEIGHT - 20, 15, 10);

  /* --- Mat Trang nho o goc trai tren --- */
  setfillstyle(SOLID_FILL, DARKGRAY);
  fillellipse(82, 82, 38, 38);
  setfillstyle(SOLID_FILL, LIGHTGRAY);
  fillellipse(80, 80, 35, 35);
  setfillstyle(SOLID_FILL, DARKGRAY);
  fillellipse(70, 72, 6, 6);
  fillellipse(88, 85, 8, 8);
  fillellipse(78, 90, 5, 5);

  /* --- Sao Tho nho (voi vanh dai) o goc tren phai --- */
  setfillstyle(SOLID_FILL, YELLOW);
  fillellipse(WIDTH - 180, 100, 20, 18);
  setfillstyle(SOLID_FILL, BROWN);
  fillellipse(WIDTH - 180, 100, 14, 12);
  setcolor(LIGHTGRAY);
  ellipse(WIDTH - 180, 100, 0, 360, 35, 10);
  ellipse(WIDTH - 180, 100, 0, 360, 33, 9);

  /* --- Phi thuyen Alien (UFO) bay qua --- */
  {
    int ux = WIDTH / 2 + 300, uy = 80;
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    fillellipse(ux, uy, 30, 10);
    setfillstyle(SOLID_FILL, CYAN);
    fillellipse(ux, uy - 7, 12, 9);
    setcolor(YELLOW);
    circle(ux - 10, uy + 8, 2);
    circle(ux, uy + 8, 2);
    circle(ux + 10, uy + 8, 2);
    /* Tia sang UFO */
    setcolor(LIGHTCYAN);
    line(ux, uy + 10, ux - 15, uy + 35);
    line(ux, uy + 10, ux + 15, uy + 35);
  }

  /* --- Sao bang bay cheo --- */
  for (i = 0; i < 4; i++) {
    int sx = 100 + i * 280;
    int sy = 50 + i * 70;
    setcolor(WHITE);
    line(sx, sy, sx + 25, sy + 12);
    setcolor(LIGHTGRAY);
    line(sx + 25, sy + 12, sx + 50, sy + 24);
  }

  /* --- Tinh van nho (cum sao mau) o giua ben trai --- */
  {
    unsigned int seed = 54321u;
    for (i = 0; i < 80; i++) {
      seed = seedRand(seed, i);
      int nx = 50 + (seed % 200);
      int ny = HEIGHT / 2 - 50 + ((seed / 7) % 150);
      int c = (i % 4 == 0) ? MAGENTA : (i % 4 == 1) ? LIGHTCYAN : (i % 4 == 2) ? CYAN : LIGHTMAGENTA;
      putpixel(nx, ny, c);
      if (i % 5 == 0) {
        putpixel(nx + 1, ny, c);
        putpixel(nx, ny + 1, c);
      }
    }
  }

  /* --- Thien thach trang tri roi rac --- */
  {
    int astPos[][2] = {{150, HEIGHT - 100}, {WIDTH - 350, HEIGHT - 80}, {WIDTH / 2 - 350, 120}};
    for (i = 0; i < 3; i++) {
      int ax = astPos[i][0], ay = astPos[i][1];
      int ar = 12 + i * 5;
      setfillstyle(SOLID_FILL, DARKGRAY);
      fillellipse(ax, ay, ar, ar);
      setcolor(LIGHTGRAY);
      circle(ax, ay, ar);
      setcolor(DARKGRAY);
      circle(ax - ar / 3, ay - ar / 4, ar / 4);
    }
  }
}

/* drawMainMenu(): Ve man hinh menu chinh voi text can giua */
void drawMainMenu(int mx, int my) {
  int tw, th, btnCY;
  int startHov = pointInRect(mx, my, MENU_START_X1, MENU_START_Y1,
                             MENU_START_X2, MENU_START_Y2);
  int guideHov = pointInRect(mx, my, MENU_GUIDE_X1, MENU_GUIDE_Y1,
                             MENU_GUIDE_X2, MENU_GUIDE_Y2);
  int exitHov = pointInRect(mx, my, MENU_EXIT_X1, MENU_EXIT_Y1, MENU_EXIT_X2,
                            MENU_EXIT_Y2);

  cleardevice();
  drawMenuBackground();

  /* --- Tieu de game (can giua) --- */
  setcolor(YELLOW);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 7);
  tw = textwidth((char *)"ASTEROID");
  outtextxy(WIDTH / 2 - tw / 2, HEIGHT / 2 - 200, (char *)"ASTEROID");
  setcolor(CYAN);
  tw = textwidth((char *)"BLASTER");
  outtextxy(WIDTH / 2 - tw / 2, HEIGHT / 2 - 120, (char *)"BLASTER");

  /* Phien ban (can giua) */
  setcolor(LIGHTGRAY);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  tw = textwidth((char *)"v3.0");
  outtextxy(WIDTH / 2 - tw / 2, HEIGHT / 2 - 50, (char *)"v3.0");

  /* --- Nut BAT DAU (can giua ngang + doc) --- */
  setfillstyle(SOLID_FILL, startHov ? GREEN : DARKGRAY);
  bar(MENU_START_X1, MENU_START_Y1, MENU_START_X2, MENU_START_Y2);
  setcolor(startHov ? YELLOW : WHITE);
  rectangle(MENU_START_X1, MENU_START_Y1, MENU_START_X2, MENU_START_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  tw = textwidth((char *)"BAT DAU");
  th = textheight((char *)"BAT DAU");
  btnCY = (MENU_START_Y1 + MENU_START_Y2) / 2;
  outtextxy(WIDTH / 2 - tw / 2, btnCY - th / 2, (char *)"BAT DAU");

  /* --- Nut HUONG DAN (can giua) --- */
  setfillstyle(SOLID_FILL, guideHov ? BLUE : DARKGRAY);
  bar(MENU_GUIDE_X1, MENU_GUIDE_Y1, MENU_GUIDE_X2, MENU_GUIDE_Y2);
  setcolor(guideHov ? YELLOW : LIGHTCYAN);
  rectangle(MENU_GUIDE_X1, MENU_GUIDE_Y1, MENU_GUIDE_X2, MENU_GUIDE_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  tw = textwidth((char *)"HUONG DAN");
  th = textheight((char *)"HUONG DAN");
  btnCY = (MENU_GUIDE_Y1 + MENU_GUIDE_Y2) / 2;
  outtextxy(WIDTH / 2 - tw / 2, btnCY - th / 2, (char *)"HUONG DAN");

  /* --- Nut THOAT (can giua) --- */
  setfillstyle(SOLID_FILL, exitHov ? RED : DARKGRAY);
  bar(MENU_EXIT_X1, MENU_EXIT_Y1, MENU_EXIT_X2, MENU_EXIT_Y2);
  setcolor(exitHov ? YELLOW : LIGHTRED);
  rectangle(MENU_EXIT_X1, MENU_EXIT_Y1, MENU_EXIT_X2, MENU_EXIT_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  tw = textwidth((char *)"THOAT");
  th = textheight((char *)"THOAT");
  btnCY = (MENU_EXIT_Y1 + MENU_EXIT_Y2) / 2;
  outtextxy(WIDTH / 2 - tw / 2, btnCY - th / 2, (char *)"THOAT");

  /* Reset */
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* handleMainMenuClick(): Xu ly click chuot trong menu chinh
 * Tra ve: 1=Start, 2=Guide, 3=Exit, 0=khong click gi */
int handleMainMenuClick(int mx, int my) {
  if (pointInRect(mx, my, MENU_START_X1, MENU_START_Y1, MENU_START_X2,
                  MENU_START_Y2))
    return 1;
  if (pointInRect(mx, my, MENU_GUIDE_X1, MENU_GUIDE_Y1, MENU_GUIDE_X2,
                  MENU_GUIDE_Y2))
    return 2;
  if (pointInRect(mx, my, MENU_EXIT_X1, MENU_EXIT_Y1, MENU_EXIT_X2,
                  MENU_EXIT_Y2))
    return 3;
  return 0;
}

#endif
