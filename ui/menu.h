#ifndef MENU_H
#define MENU_H

/* drawMenuBackground(): Ve background dep cho menu voi hanh tinh, phi thuyen, hieu ung */
void drawMenuBackground(void)
{
  int i;
  drawStars();

  /* --- Hanh tinh lon o goc duoi phai --- */
  setfillstyle(SOLID_FILL, COL_EARTH_OCEAN);
  fillellipse(WIDTH - 100, HEIGHT + 50, 180, 180);
  setfillstyle(SOLID_FILL, COL_EARTH_LAND);
  fillellipse(WIDTH - 130, HEIGHT + 10, 50, 40);
  fillellipse(WIDTH - 60, HEIGHT + 60, 40, 30);
  setfillstyle(SOLID_FILL, COL_EARTH_CLOUD);
  fillellipse(WIDTH - 160, HEIGHT - 20, 15, 10);

  /* --- Mat Trang nho o goc trai tren --- */
  setfillstyle(SOLID_FILL, COL_MOON_SHADOW);
  fillellipse(82, 82, 38, 38);
  setfillstyle(SOLID_FILL, COL_MOON_SURFACE);
  fillellipse(80, 80, 35, 35);
  setfillstyle(SOLID_FILL, COL_MOON_CRATER);
  fillellipse(70, 72, 6, 6);
  fillellipse(88, 85, 8, 8);
  fillellipse(78, 90, 5, 5);

  /* --- Sao Tho nho (voi vanh dai) o goc tren phai --- */
  setfillstyle(SOLID_FILL, COL_SATURN_BODY_2);
  fillellipse(WIDTH - 180, 100, 20, 18);
  setfillstyle(SOLID_FILL, COL_SATURN_BODY_1);
  fillellipse(WIDTH - 180, 100, 14, 12);
  setcolor(COL_SATURN_RING_LT);
  ellipse(WIDTH - 180, 100, 0, 360, 35, 10);
  ellipse(WIDTH - 180, 100, 0, 360, 33, 9);

  /* --- Phi thuyen Alien (UFO) bay qua --- */
  {
    int ux = WIDTH / 2 + 300, uy = 80;
    setfillstyle(SOLID_FILL, COL_UFO_BODY);
    fillellipse(ux, uy, 30, 10);
    setfillstyle(SOLID_FILL, COL_UFO_CABIN);
    fillellipse(ux, uy - 7, 12, 9);
    setcolor(COL_UFO_LIGHT);
    circle(ux - 10, uy + 8, 2);
    circle(ux, uy + 8, 2);
    circle(ux + 10, uy + 8, 2);
    /* Tia sang UFO */
    setcolor(COL_UFO_BEAM);
    line(ux, uy + 10, ux - 15, uy + 35);
    line(ux, uy + 10, ux + 15, uy + 35);
  }

  /* --- Sao bang bay cheo --- */
  for (i = 0; i < 4; i++)
  {
    int sx = 100 + i * 280;
    int sy = 50 + i * 70;
    setcolor(COL_SHOOTING_HEAD);
    line(sx, sy, sx + 25, sy + 12);
    setcolor(COL_SHOOTING_MID);
    line(sx + 25, sy + 12, sx + 50, sy + 24);
  }

  /* --- Tinh van nho (cum sao mau) o giua ben trai --- */
  {
    unsigned int seed = 54321u;
    for (i = 0; i < 80; i++)
    {
      seed = seedRand(seed, i);
      int nx = 50 + (seed % 200);
      int ny = HEIGHT / 2 - 50 + ((seed / 7) % 150);
      int c = (i % 4 == 0) ? COL_NEBULA_PURPLE : (i % 4 == 1) ? COL_NEBULA_LTCYAN
                                               : (i % 4 == 2)  ? COL_NEBULA_CYAN
                                                                : COL_NEBULA_PINK;
      putpixel(nx, ny, c);
      if (i % 5 == 0)
      {
        putpixel(nx + 1, ny, c);
        putpixel(nx, ny + 1, c);
      }
    }
  }

  /* --- Thien thach trang tri roi rac --- */
  {
    int astPos[][2] = {{150, HEIGHT - 100}, {WIDTH - 350, HEIGHT - 80}, {WIDTH / 2 - 350, 120}};
    for (i = 0; i < 3; i++)
    {
      int ax = astPos[i][0], ay = astPos[i][1];
      int ar = 12 + i * 5;
      setfillstyle(SOLID_FILL, COL_AST_S_DARK);
      fillellipse(ax, ay, ar, ar);
      setcolor(COL_AST_S_LINE);
      circle(ax, ay, ar);
      setcolor(COL_AST_S_CRATER);
      circle(ax - ar / 3, ay - ar / 4, ar / 4);
    }
  }
}

/* drawMainMenu(): Ve man hinh menu chinh voi text can giua */
void drawMainMenu(int mx, int my)
{
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
  setcolor(COL_TEXT_TITLE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 7);
  tw = textwidth((char *)"ASTEROID");
  outtextxy(WIDTH / 2 - tw / 2, HEIGHT / 2 - 200, (char *)"ASTEROID");
  setcolor(COL_TEXT_SUBTITLE);
  tw = textwidth((char *)"BLASTER");
  outtextxy(WIDTH / 2 - tw / 2, HEIGHT / 2 - 120, (char *)"BLASTER");

  /* Phien ban (can giua) */
  setcolor(COL_TEXT_DIM);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  tw = textwidth((char *)"v3.0");
  outtextxy(WIDTH / 2 - tw / 2, HEIGHT / 2 - 50, (char *)"v3.0");

  /* --- Nut BAT DAU (can giua ngang + doc) --- */
  setfillstyle(SOLID_FILL, startHov ? COL_BTN_HOVER_GREEN : COL_BTN_NORMAL);
  bar(MENU_START_X1, MENU_START_Y1, MENU_START_X2, MENU_START_Y2);
  setcolor(startHov ? COL_BTN_TEXT_HOV : COL_BTN_TEXT);
  rectangle(MENU_START_X1, MENU_START_Y1, MENU_START_X2, MENU_START_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  tw = textwidth((char *)"BAT DAU");
  th = textHeightCompat("BAT DAU");
  btnCY = (MENU_START_Y1 + MENU_START_Y2) / 2;
  outtextxy(WIDTH / 2 - tw / 2, btnCY - th / 2, (char *)"BAT DAU");

  /* --- Nut HUONG DAN (can giua) --- */
  setfillstyle(SOLID_FILL, guideHov ? COL_BTN_HOVER_BLUE : COL_BTN_NORMAL);
  bar(MENU_GUIDE_X1, MENU_GUIDE_Y1, MENU_GUIDE_X2, MENU_GUIDE_Y2);
  setcolor(guideHov ? COL_BTN_TEXT_HOV : COL_TEXT_SUBTITLE);
  rectangle(MENU_GUIDE_X1, MENU_GUIDE_Y1, MENU_GUIDE_X2, MENU_GUIDE_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  tw = textwidth((char *)"HUONG DAN");
  th = textHeightCompat("HUONG DAN");
  btnCY = (MENU_GUIDE_Y1 + MENU_GUIDE_Y2) / 2;
  outtextxy(WIDTH / 2 - tw / 2, btnCY - th / 2, (char *)"HUONG DAN");

  /* --- Nut THOAT (can giua) --- */
  setfillstyle(SOLID_FILL, exitHov ? COL_BTN_HOVER_RED : COL_BTN_NORMAL);
  bar(MENU_EXIT_X1, MENU_EXIT_Y1, MENU_EXIT_X2, MENU_EXIT_Y2);
  setcolor(exitHov ? COL_BTN_TEXT_HOV : COL_TEXT_DANGER);
  rectangle(MENU_EXIT_X1, MENU_EXIT_Y1, MENU_EXIT_X2, MENU_EXIT_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  tw = textwidth((char *)"THOAT");
  th = textHeightCompat("THOAT");
  btnCY = (MENU_EXIT_Y1 + MENU_EXIT_Y2) / 2;
  outtextxy(WIDTH / 2 - tw / 2, btnCY - th / 2, (char *)"THOAT");

  /* Reset */
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* handleMainMenuClick(): Xu ly click chuot trong menu chinh
 * Tra ve: 1=Start, 2=Guide, 3=Exit, 0=khong click gi */
int handleMainMenuClick(int mx, int my)
{
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
