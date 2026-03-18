#ifndef PAUSE_H
#define PAUSE_H

/* drawPauseMenu(): Ve lop overlay tam dung game */
void drawPauseMenu(int hmx, int hmy) {
  int contHov = pointInRect(hmx, hmy, CONT_X1, CONT_Y1, CONT_X2, CONT_Y2);
  int exitHov = pointInRect(hmx, hmy, EXIT_X1, EXIT_Y1, EXIT_X2, EXIT_Y2);

  /* --- Lop mo (dim overlay) --- */
  setcolor(BLACK);
  {
    int yy;
    for (yy = 0; yy < HEIGHT; yy += 2)
      line(0, yy, WIDTH, yy);
  }

  /* --- Panel chinh --- */
  setfillstyle(SOLID_FILL, BLACK);
  bar(PANEL_X1, PANEL_Y1, PANEL_X2, PANEL_Y2);
  
  /* Vien doi */
  setcolor(LIGHTCYAN);
  rectangle(PANEL_X1, PANEL_Y1, PANEL_X2, PANEL_Y2);
  rectangle(PANEL_X1 - 2, PANEL_Y1 - 2, PANEL_X2 + 2, PANEL_Y2 + 2);

  /* Tieu de */
  setcolor(YELLOW);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  outtextxy(WIDTH / 2 - 100, PANEL_Y1 + 18, (char *)"PAUSE MENU");

  /* --- Nut CONTINUE --- */
  setfillstyle(SOLID_FILL, contHov ? BLUE : DARKGRAY);
  bar(CONT_X1, CONT_Y1, CONT_X2, CONT_Y2);
  setcolor(contHov ? WHITE : LIGHTCYAN);
  rectangle(CONT_X1, CONT_Y1, CONT_X2, CONT_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  setcolor(contHov ? YELLOW : WHITE);
  outtextxy(WIDTH / 2 - 65, CONT_Y1 + 8, (char *)"CONTINUE");

  /* --- Nut EXIT --- */
  setfillstyle(SOLID_FILL, exitHov ? RED : DARKGRAY);
  bar(EXIT_X1, EXIT_Y1, EXIT_X2, EXIT_Y2);
  setcolor(exitHov ? WHITE : LIGHTRED);
  rectangle(EXIT_X1, EXIT_Y1, EXIT_X2, EXIT_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  setcolor(exitHov ? YELLOW : WHITE);
  outtextxy(WIDTH / 2 - 35, EXIT_Y1 + 8, (char *)"EXIT");

  /* Reset font */
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* handlePauseMenuClick(): Xu ly click trong pause menu
 * Tra ve: 1=Continue, 2=Exit, 0=khong click */
int handlePauseMenuClick(int mx, int my) {
  if (pointInRect(mx, my, CONT_X1, CONT_Y1, CONT_X2, CONT_Y2))
    return 1;
  if (pointInRect(mx, my, EXIT_X1, EXIT_Y1, EXIT_X2, EXIT_Y2))
    return 2;
  return 0;
}

#endif
