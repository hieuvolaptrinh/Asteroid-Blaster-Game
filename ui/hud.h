#ifndef HUD_H
#define HUD_H

/* ====================================================================== */
/*              HUD, LEVEL BANNER, MENU BUTTON                            */
/* ====================================================================== */

/* drawHUD(): Ve giao dien nguoi choi: HP, Level, Score, chi so dan, cooldown skill. */
void drawHUD(int score, int hp, int level, Ship ship, SkillSystem *sk)
{
  char buf[80];
  unsigned long now = GetTickCount();

  /* HP bar */
  setcolor(WHITE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  outtextxy(15, 12, (char *)"HP:");
  rectangle(50, 10, 260, 28);
  int bw = (int)(210.0f * hp / INITIAL_HP);
  if (bw < 0)
    bw = 0;
  if (hp > 60)
    setfillstyle(SOLID_FILL, GREEN);
  else if (hp > 30)
    setfillstyle(SOLID_FILL, YELLOW);
  else
    setfillstyle(SOLID_FILL, RED);
  bar(51, 11, 51 + bw, 27);
  sprintf(buf, "%d/%d", hp, INITIAL_HP);
  setcolor(WHITE);
  outtextxy(268, 12, buf);

  /* Level */
  setcolor(LIGHTCYAN);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  sprintf(buf, "LEVEL %d", level);
  outtextxy(WIDTH / 2 - 55, 8, buf);

  /* Score */
  setcolor(YELLOW);
  sprintf(buf, "SCORE: %d", score);
  outtextxy(WIDTH - 240, 8, buf);

  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

  /* Weapon stats */
  setcolor(LIGHTGREEN);
  sprintf(buf, "DMG:%d  PELLETS:%d  CD:%dms", ship.bulletDamage,
          ship.pelletsPerShot, ship.shootCooldown);
  outtextxy(15, HEIGHT - 50, buf);

  /* Score de len level */
  setcolor(DARKGRAY);
  sprintf(buf, "Next Lv: %d", level * LEVEL_UP_SCORE);
  outtextxy(15, HEIGHT - 30, buf);

  /* Skill cooldown */
  {
    int sx = WIDTH - 360, sy = HEIGHT - 80;
    float cd;
    int color;

    /* Skill 1 – Bomb */
    cd = 0;
    if (now - sk->bombLast < sk->bombCD)
      cd = (float)(sk->bombCD - (now - sk->bombLast)) / 1000.0f;
    color = (cd <= 0) ? GREEN : RED;
    setcolor(color);
    if (cd <= 0)
      sprintf(buf, "[1] BOMB: Ready");
    else
      sprintf(buf, "[1] BOMB: %.1fs", cd);
    outtextxy(sx, sy, buf);

    /* Skill 2 – Slow */
    cd = 0;
    if (now - sk->slowLast < sk->slowCD)
      cd = (float)(sk->slowCD - (now - sk->slowLast)) / 1000.0f;
    if (sk->slowActive)
    {
      color = YELLOW;
      sprintf(buf, "[2] SLOW: Active");
    }
    else if (cd <= 0)
    {
      color = GREEN;
      sprintf(buf, "[2] SLOW: Ready");
    }
    else
    {
      color = RED;
      sprintf(buf, "[2] SLOW: %.1fs", cd);
    }
    setcolor(color);
    outtextxy(sx, sy + 16, buf);

    /* Skill 3 – Beam */
    cd = 0;
    if (now - sk->beamLast < sk->beamCD)
      cd = (float)(sk->beamCD - (now - sk->beamLast)) / 1000.0f;
    color = (cd <= 0) ? GREEN : RED;
    setcolor(color);
    if (cd <= 0)
      sprintf(buf, "[3] BEAM: Ready");
    else
      sprintf(buf, "[3] BEAM: %.1fs", cd);
    outtextxy(sx, sy + 32, buf);
  }
}

/* --- Level Banner --- */
void drawLevelBanner(int level)
{
  char buf[32];
  int tw, th;
  setfillstyle(SOLID_FILL, BLUE);
  bar(WIDTH / 2 - 200, HEIGHT / 2 - 60, WIDTH / 2 + 200, HEIGHT / 2 + 40);
  setcolor(LIGHTCYAN);
  rectangle(WIDTH / 2 - 200, HEIGHT / 2 - 60, WIDTH / 2 + 200, HEIGHT / 2 + 40);
  rectangle(WIDTH / 2 - 202, HEIGHT / 2 - 62, WIDTH / 2 + 202, HEIGHT / 2 + 42);
  setcolor(YELLOW);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
  tw = textwidth((char *)"LEVEL UP!");
  th = textHeightCompat("LEVEL UP!");
  outtextxy(WIDTH / 2 - tw / 2, HEIGHT / 2 - 50 - th / 2, (char *)"LEVEL UP!");
  setcolor(WHITE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  sprintf(buf, "Level %d", level);
  tw = textwidth(buf);
  outtextxy(WIDTH / 2 - tw / 2, HEIGHT / 2 + 5, buf);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* drawMenuButton(): Ve nut hamburger (3 gach ngang) goc trai tren. */
void drawMenuButton(int hovered)
{
  setfillstyle(SOLID_FILL, hovered ? DARKGRAY : BLACK);
  bar(MENU_BTN_X1, MENU_BTN_Y1, MENU_BTN_X2, MENU_BTN_Y2);
  setcolor(hovered ? WHITE : LIGHTGRAY);
  rectangle(MENU_BTN_X1, MENU_BTN_Y1, MENU_BTN_X2, MENU_BTN_Y2);
  int cx = (MENU_BTN_X1 + MENU_BTN_X2) / 2;
  int cy = (MENU_BTN_Y1 + MENU_BTN_Y2) / 2;
  setcolor(hovered ? YELLOW : WHITE);
  line(cx - 12, cy - 7, cx + 12, cy - 7);
  line(cx - 12, cy, cx + 12, cy);
  line(cx - 12, cy + 7, cx + 12, cy + 7);
}

#endif
