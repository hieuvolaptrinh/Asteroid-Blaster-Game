#ifndef GAME_OVER_H
#define GAME_OVER_H

/* drawGameOverScreen(): Ve man hinh ket thuc (can giua, tieng Viet) */
void drawGameOverScreen(int score, int level, int mx, int my) {
  char buf[80];
  int tw, th, btnCY;
  int replayHov = pointInRect(mx, my, GAMEOVER_REPLAY_X1, GAMEOVER_REPLAY_Y1,
                              GAMEOVER_REPLAY_X2, GAMEOVER_REPLAY_Y2);
  int menuHov = pointInRect(mx, my, GAMEOVER_MENU_X1, GAMEOVER_MENU_Y1,
                            GAMEOVER_MENU_X2, GAMEOVER_MENU_Y2);

  cleardevice();
  drawStars();

  /* Khung chinh */
  setcolor(COL_GAMEOVER_BORDER);
  rectangle(WIDTH / 2 - 280, HEIGHT / 2 - 140, WIDTH / 2 + 280,
            HEIGHT / 2 + 190);
  setcolor(COL_GAMEOVER_TITLE);
  rectangle(WIDTH / 2 - 282, HEIGHT / 2 - 142, WIDTH / 2 + 282,
            HEIGHT / 2 + 192);

  /* Tieu de KET THUC (can giua) */
  setcolor(COL_GAMEOVER_TITLE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 7);
  tw = textwidth((char *)"KET THUC");
  outtextxy(WIDTH / 2 - tw / 2, HEIGHT / 2 - 120, (char *)"KET THUC");

  /* Diem so (can giua) */
  setcolor(COL_TEXT_TITLE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  sprintf(buf, "Diem: %d", score);
  tw = textwidth(buf);
  outtextxy(WIDTH / 2 - tw / 2, HEIGHT / 2 - 10, buf);

  /* Level dat duoc (can giua) */
  setcolor(COL_TEXT_SUBTITLE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  sprintf(buf, "Cap do dat duoc: %d", level);
  tw = textwidth(buf);
  outtextxy(WIDTH / 2 - tw / 2, HEIGHT / 2 + 20, buf);

  /* --- Nut CHOI LAI (can giua) --- */
  setfillstyle(SOLID_FILL, replayHov ? COL_BTN_HOVER_GREEN : COL_BTN_NORMAL);
  bar(GAMEOVER_REPLAY_X1, GAMEOVER_REPLAY_Y1, GAMEOVER_REPLAY_X2,
      GAMEOVER_REPLAY_Y2);
  setcolor(replayHov ? COL_BTN_TEXT_HOV : COL_BTN_TEXT);
  rectangle(GAMEOVER_REPLAY_X1, GAMEOVER_REPLAY_Y1, GAMEOVER_REPLAY_X2,
            GAMEOVER_REPLAY_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  tw = textwidth((char *)"CHOI LAI");
  th = textHeightCompat("CHOI LAI");
  btnCY = (GAMEOVER_REPLAY_Y1 + GAMEOVER_REPLAY_Y2) / 2;
  outtextxy(WIDTH / 2 - tw / 2, btnCY - th / 2, (char *)"CHOI LAI");

  /* --- Nut VE MENU (can giua) --- */
  setfillstyle(SOLID_FILL, menuHov ? COL_BTN_HOVER_RED : COL_BTN_NORMAL);
  bar(GAMEOVER_MENU_X1, GAMEOVER_MENU_Y1, GAMEOVER_MENU_X2, GAMEOVER_MENU_Y2);
  setcolor(menuHov ? COL_BTN_TEXT_HOV : COL_TEXT_DANGER);
  rectangle(GAMEOVER_MENU_X1, GAMEOVER_MENU_Y1, GAMEOVER_MENU_X2,
            GAMEOVER_MENU_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  tw = textwidth((char *)"VE MENU CHINH");
  th = textHeightCompat("VE MENU CHINH");
  btnCY = (GAMEOVER_MENU_Y1 + GAMEOVER_MENU_Y2) / 2;
  outtextxy(WIDTH / 2 - tw / 2, btnCY - th / 2, (char *)"VE MENU CHINH");

  /* Reset */
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* handleGameOverClick(): Xu ly click trong game over screen
 * Tra ve: 1=Replay, 2=Exit to Menu, 0=khong click */
int handleGameOverClick(int mx, int my) {
  if (pointInRect(mx, my, GAMEOVER_REPLAY_X1, GAMEOVER_REPLAY_Y1,
                  GAMEOVER_REPLAY_X2, GAMEOVER_REPLAY_Y2))
    return 1;
  if (pointInRect(mx, my, GAMEOVER_MENU_X1, GAMEOVER_MENU_Y1, GAMEOVER_MENU_X2,
                  GAMEOVER_MENU_Y2))
    return 2;
  return 0;
}

#endif
