#ifndef GAME_OVER_H
#define GAME_OVER_H

/* Toa do cac nut trong Game Over */
#define GAMEOVER_REPLAY_X1 (WIDTH / 2 - 150)
#define GAMEOVER_REPLAY_Y1 (HEIGHT / 2 + 50)
#define GAMEOVER_REPLAY_X2 (WIDTH / 2 + 150)
#define GAMEOVER_REPLAY_Y2 (HEIGHT / 2 + 100)

#define GAMEOVER_MENU_X1 (WIDTH / 2 - 150)
#define GAMEOVER_MENU_Y1 (HEIGHT / 2 + 120)
#define GAMEOVER_MENU_X2 (WIDTH / 2 + 150)
#define GAMEOVER_MENU_Y2 (HEIGHT / 2 + 170)

/* drawGameOverScreen(): Ve man hinh game over voi cac nut */
void drawGameOverScreen(int score, int level, int mx, int my) {
  char buf[80];
  int replayHov = pointInRect(mx, my, GAMEOVER_REPLAY_X1, GAMEOVER_REPLAY_Y1, GAMEOVER_REPLAY_X2, GAMEOVER_REPLAY_Y2);
  int menuHov = pointInRect(mx, my, GAMEOVER_MENU_X1, GAMEOVER_MENU_Y1, GAMEOVER_MENU_X2, GAMEOVER_MENU_Y2);

  cleardevice();
  drawStars();

  /* Khung chinh */
  setcolor(RED);
  rectangle(WIDTH / 2 - 280, HEIGHT / 2 - 140, WIDTH / 2 + 280, HEIGHT / 2 + 190);
  rectangle(WIDTH / 2 - 282, HEIGHT / 2 - 142, WIDTH / 2 + 282, HEIGHT / 2 + 192);

  /* Tieu de GAME OVER */
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 7);
  outtextxy(WIDTH / 2 - 210, HEIGHT / 2 - 110, (char *)"GAME OVER");

  /* Diem so */
  setcolor(YELLOW);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  sprintf(buf, "Final Score: %d", score);
  outtextxy(WIDTH / 2 - 130, HEIGHT / 2 - 10, buf);

  /* Level dat duoc */
  setcolor(LIGHTCYAN);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  sprintf(buf, "Level Reached: %d", level);
  outtextxy(WIDTH / 2 - 100, HEIGHT / 2 + 20, buf);

  /* --- Nut REPLAY --- */
  setfillstyle(SOLID_FILL, replayHov ? GREEN : DARKGRAY);
  bar(GAMEOVER_REPLAY_X1, GAMEOVER_REPLAY_Y1, GAMEOVER_REPLAY_X2, GAMEOVER_REPLAY_Y2);
  setcolor(replayHov ? YELLOW : WHITE);
  rectangle(GAMEOVER_REPLAY_X1, GAMEOVER_REPLAY_Y1, GAMEOVER_REPLAY_X2, GAMEOVER_REPLAY_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  outtextxy(WIDTH / 2 - 60, GAMEOVER_REPLAY_Y1 + 12, (char *)"REPLAY");

  /* --- Nut EXIT TO MENU --- */
  setfillstyle(SOLID_FILL, menuHov ? RED : DARKGRAY);
  bar(GAMEOVER_MENU_X1, GAMEOVER_MENU_Y1, GAMEOVER_MENU_X2, GAMEOVER_MENU_Y2);
  setcolor(menuHov ? YELLOW : LIGHTRED);
  rectangle(GAMEOVER_MENU_X1, GAMEOVER_MENU_Y1, GAMEOVER_MENU_X2, GAMEOVER_MENU_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  outtextxy(WIDTH / 2 - 95, GAMEOVER_MENU_Y1 + 8, (char *)"EXIT TO MENU");

  /* Reset */
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* handleGameOverClick(): Xu ly click trong game over screen
 * Tra ve: 1=Replay, 2=Exit to Menu, 0=khong click */
int handleGameOverClick(int mx, int my) {
  if (pointInRect(mx, my, GAMEOVER_REPLAY_X1, GAMEOVER_REPLAY_Y1, GAMEOVER_REPLAY_X2, GAMEOVER_REPLAY_Y2))
    return 1;
  if (pointInRect(mx, my, GAMEOVER_MENU_X1, GAMEOVER_MENU_Y1, GAMEOVER_MENU_X2, GAMEOVER_MENU_Y2))
    return 2;
  return 0;
}

#endif
