#ifndef INTRODUCTION_H
#define INTRODUCTION_H

#define GUIDE_BACK_X1 (WIDTH / 2 - 100)
#define GUIDE_BACK_Y1 (HEIGHT - 80)
#define GUIDE_BACK_X2 (WIDTH / 2 + 100)
#define GUIDE_BACK_Y2 (HEIGHT - 40)

/* drawGuideScreen(): Hien thi huong dan choi game */
void drawGuideScreen(int mx, int my) {
  int backHov = pointInRect(mx, my, GUIDE_BACK_X1, GUIDE_BACK_Y1, GUIDE_BACK_X2, GUIDE_BACK_Y2);
  
  cleardevice();
  drawStars();

  /* Tieu de */
  setcolor(YELLOW);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
  outtextxy(WIDTH / 2 - 150, 30, (char *)"HOW TO PLAY");

  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  
  /* Dieu khien */
  setcolor(LIGHTCYAN);
  outtextxy(100, 100, (char *)"CONTROLS:");
  
  setcolor(WHITE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  outtextxy(120, 130, (char *)"WASD / Arrow Keys - Move ship");
  outtextxy(120, 150, (char *)"Mouse - Aim direction");
  outtextxy(120, 170, (char *)"Auto Shoot - Shoots automatically");
  outtextxy(120, 190, (char *)"ESC - Pause game");

  /* Ky nang */
  setcolor(LIGHTCYAN);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  outtextxy(100, 230, (char *)"SKILLS:");
  
  setcolor(YELLOW);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  outtextxy(120, 260, (char *)"[1] BOMB BURST - Explode area around ship (CD: 8s)");
  
  setcolor(LIGHTBLUE);
  outtextxy(120, 280, (char *)"[2] TIME SLOW - Slow asteroids for 4s (CD: 12s)");
  
  setcolor(LIGHTCYAN);
  outtextxy(120, 300, (char *)"[3] PIERCING BEAM - Laser beam (CD: 10s)");

  /* Muc tieu */
  setcolor(LIGHTCYAN);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  outtextxy(100, 350, (char *)"OBJECTIVE:");
  
  setcolor(WHITE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  outtextxy(120, 380, (char *)"- Destroy asteroids to earn points");
  outtextxy(120, 400, (char *)"- Collect power crystals to upgrade");
  outtextxy(120, 420, (char *)"- Reach 1000 points per level to advance");
  outtextxy(120, 440, (char *)"- Survive as long as possible!");

  /* Power-ups */
  setcolor(LIGHTMAGENTA);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  outtextxy(100, 480, (char *)"POWER CRYSTALS:");
  
  setcolor(WHITE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  outtextxy(120, 510, (char *)"Cycle: Pellets -> Damage -> Fire Rate");

  /* Nut BACK */
  setfillstyle(SOLID_FILL, backHov ? CYAN : DARKGRAY);
  bar(GUIDE_BACK_X1, GUIDE_BACK_Y1, GUIDE_BACK_X2, GUIDE_BACK_Y2);
  setcolor(backHov ? YELLOW : WHITE);
  rectangle(GUIDE_BACK_X1, GUIDE_BACK_Y1, GUIDE_BACK_X2, GUIDE_BACK_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  outtextxy(WIDTH / 2 - 35, GUIDE_BACK_Y1 + 8, (char *)"BACK");

  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* handleGuideClick(): Kiem tra click nut BACK
 * Tra ve: 1=Back, 0=khong click */
int handleGuideClick(int mx, int my) {
  if (pointInRect(mx, my, GUIDE_BACK_X1, GUIDE_BACK_Y1, GUIDE_BACK_X2, GUIDE_BACK_Y2))
    return 1;
  return 0;
}

#endif
