#ifndef MENU_H
#define MENU_H

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

/* drawMainMenu(): Ve man hinh menu chinh */
void drawMainMenu(int mx, int my) {
  int startHov = pointInRect(mx, my, MENU_START_X1, MENU_START_Y1, MENU_START_X2, MENU_START_Y2);
  int guideHov = pointInRect(mx, my, MENU_GUIDE_X1, MENU_GUIDE_Y1, MENU_GUIDE_X2, MENU_GUIDE_Y2);
  int exitHov = pointInRect(mx, my, MENU_EXIT_X1, MENU_EXIT_Y1, MENU_EXIT_X2, MENU_EXIT_Y2);

  cleardevice();
  drawStars();

  /* --- Tieu de game --- */
  setcolor(YELLOW);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 7);
  outtextxy(WIDTH / 2 - 280, HEIGHT / 2 - 180, (char *)"ASTEROID");
  setcolor(CYAN);
  outtextxy(WIDTH / 2 - 200, HEIGHT / 2 - 100, (char *)"BLASTER");

  /* Phien ban */
  setcolor(LIGHTGRAY);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  outtextxy(WIDTH / 2 - 30, HEIGHT / 2 - 50, (char *)"v3.0");

  /* --- Nut START GAME --- */
  setfillstyle(SOLID_FILL, startHov ? GREEN : DARKGRAY);
  bar(MENU_START_X1, MENU_START_Y1, MENU_START_X2, MENU_START_Y2);
  setcolor(startHov ? YELLOW : WHITE);
  rectangle(MENU_START_X1, MENU_START_Y1, MENU_START_X2, MENU_START_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  outtextxy(WIDTH / 2 - 90, MENU_START_Y1 + 12, (char *)"START GAME");

  /* --- Nut GUIDE --- */
  setfillstyle(SOLID_FILL, guideHov ? BLUE : DARKGRAY);
  bar(MENU_GUIDE_X1, MENU_GUIDE_Y1, MENU_GUIDE_X2, MENU_GUIDE_Y2);
  setcolor(guideHov ? YELLOW : LIGHTCYAN);
  rectangle(MENU_GUIDE_X1, MENU_GUIDE_Y1, MENU_GUIDE_X2, MENU_GUIDE_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  outtextxy(WIDTH / 2 - 50, MENU_GUIDE_Y1 + 12, (char *)"GUIDE");

  /* --- Nut EXIT --- */
  setfillstyle(SOLID_FILL, exitHov ? RED : DARKGRAY);
  bar(MENU_EXIT_X1, MENU_EXIT_Y1, MENU_EXIT_X2, MENU_EXIT_Y2);
  setcolor(exitHov ? YELLOW : LIGHTRED);
  rectangle(MENU_EXIT_X1, MENU_EXIT_Y1, MENU_EXIT_X2, MENU_EXIT_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  outtextxy(WIDTH / 2 - 35, MENU_EXIT_Y1 + 12, (char *)"EXIT");

  /* Reset */
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* handleMainMenuClick(): Xu ly click chuot trong menu chinh
 * Tra ve: 1=Start, 2=Guide, 3=Exit, 0=khong click gi */
int handleMainMenuClick(int mx, int my) {
  if (pointInRect(mx, my, MENU_START_X1, MENU_START_Y1, MENU_START_X2, MENU_START_Y2))
    return 1;
  if (pointInRect(mx, my, MENU_GUIDE_X1, MENU_GUIDE_Y1, MENU_GUIDE_X2, MENU_GUIDE_Y2))
    return 2;
  if (pointInRect(mx, my, MENU_EXIT_X1, MENU_EXIT_Y1, MENU_EXIT_X2, MENU_EXIT_Y2))
    return 3;
  return 0;
}

#endif
