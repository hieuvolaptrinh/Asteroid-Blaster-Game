#ifndef MENU_H
#define MENU_H

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
  drawStars();

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
