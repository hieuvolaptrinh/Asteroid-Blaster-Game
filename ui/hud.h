#ifndef HUD_H
#define HUD_H

/* ====================================================================== */
/*              HUD, LEVEL BANNER, MENU BUTTON                            */
/* ====================================================================== */

/* drawHUD(): Ve giao dien nguoi choi: HP, Level, Score, chi so dan,
 *            cooldown skill, va toc do game. */
void drawHUD(int score, int hp, int level, Ship ship, SkillSystem *sk,
             int gameSpeed)
{
  char buf[80];
  unsigned long now = GetTickCount();

  /* HP bar */
  setcolor(COL_TEXT_BODY);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  outtextxy(15, 12, (char *)"HP:");

  /* Nen thanh HP */
  setfillstyle(SOLID_FILL, COL_HP_BG);
  bar(50, 10, 260, 28);

  /* Vien thanh HP */
  setcolor(COL_HP_BORDER);
  rectangle(50, 10, 260, 28);

  int bw = (int)(210.0f * hp / INITIAL_HP);
  if (bw < 0)
    bw = 0;

  int hpColor;
  if (hp > 60)
    hpColor = COL_HP_HIGH;
  else if (hp > 30)
    hpColor = COL_HP_MED;
  else
    hpColor = COL_HP_LOW;

  setfillstyle(SOLID_FILL, hpColor);
  bar(51, 11, 51 + bw, 27);

  /* Highlight tren thanh HP */
  if (bw > 4)
  {
    setcolor(colorLerp(hpColor, COL_TEXT_WHITE, 0.25f));
    line(52, 11, 50 + bw, 11);
  }

  /* So HP */
  sprintf(buf, "%d/%d", hp, INITIAL_HP);
  setcolor(COL_TEXT_BODY);
  outtextxy(268, 12, buf);

  /* Level */
  setcolor(COL_TEXT_SUBTITLE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  sprintf(buf, "LEVEL %d", level);
  outtextxy(WIDTH / 2 - 55, 8, buf);

  /* Score */
  setcolor(COL_TEXT_TITLE);
  sprintf(buf, "SCORE: %d", score);
  outtextxy(WIDTH - 240, 8, buf);

  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

  /* Weapon stats */
  setcolor(COL_TEXT_SUCCESS);
  sprintf(buf, "DMG:%d  PELLETS:%d  CD:%dms", ship.bulletDamage,
          ship.pelletsPerShot, ship.shootCooldown);
  outtextxy(15, HEIGHT - 50, buf);

  /* Score de len level */
  setcolor(COL_TEXT_DIM);
  sprintf(buf, "Next Lv: %d", level * LEVEL_UP_SCORE);
  outtextxy(15, HEIGHT - 30, buf);

  /* ============================================================== */
  /*  Hien thi TOC DO GAME (goc trai, duoi weapon stats)            */
  /* ============================================================== */
  {
    const char *speedLabel;
    int speedColor;
    switch (gameSpeed)
    {
    case 0:
      speedLabel = "DUNG";
      speedColor = COL_TEXT_DANGER;
      break;
    case 1:
      speedLabel = "CHAM";
      speedColor = COL_TEXT_SUBTITLE;
      break;
    case 2:
      speedLabel = "BINH THUONG";
      speedColor = COL_TEXT_SUCCESS;
      break;
    case 3:
      speedLabel = "NHANH";
      speedColor = COL_TEXT_TITLE;
      break;
    default:
      speedLabel = "BINH THUONG";
      speedColor = COL_TEXT_SUCCESS;
      break;
    }

    setcolor(COL_TEXT_DIM);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(15, HEIGHT - 12, (char *)"TOC DO [0-3]:");
    setcolor(speedColor);
    outtextxy(120, HEIGHT - 12, (char *)speedLabel);
  }

  /* ============================================================== */
  /*  Skill cooldown (phim 7-8-9)                                   */
  /* ============================================================== */
  {
    int sx = WIDTH - 360, sy = HEIGHT - 80;
    float cd;
    int color;

    /* Skill 1 – Bomb (phim 7) */
    cd = 0;
    if (now - sk->bombLast < sk->bombCD)
      cd = (float)(sk->bombCD - (now - sk->bombLast)) / 1000.0f;
    color = (cd <= 0) ? COL_TEXT_SUCCESS : COL_TEXT_DANGER;
    setcolor(color);
    if (cd <= 0)
      sprintf(buf, "[7] BOMB: Ready");
    else
      sprintf(buf, "[7] BOMB: %.1fs", cd);
    outtextxy(sx, sy, buf);

    /* Skill 2 – Slow (phim 8) */
    cd = 0;
    if (now - sk->slowLast < sk->slowCD)
      cd = (float)(sk->slowCD - (now - sk->slowLast)) / 1000.0f;
    if (sk->slowActive)
    {
      color = COL_TEXT_TITLE;
      sprintf(buf, "[8] SLOW: Active");
    }
    else if (cd <= 0)
    {
      color = COL_TEXT_SUCCESS;
      sprintf(buf, "[8] SLOW: Ready");
    }
    else
    {
      color = COL_TEXT_DANGER;
      sprintf(buf, "[8] SLOW: %.1fs", cd);
    }
    setcolor(color);
    outtextxy(sx, sy + 16, buf);

    /* Skill 3 – Beam (phim 9) */
    cd = 0;
    if (now - sk->beamLast < sk->beamCD)
      cd = (float)(sk->beamCD - (now - sk->beamLast)) / 1000.0f;
    color = (cd <= 0) ? COL_TEXT_SUCCESS : COL_TEXT_DANGER;
    setcolor(color);
    if (cd <= 0)
      sprintf(buf, "[9] BEAM: Ready");
    else
      sprintf(buf, "[9] BEAM: %.1fs", cd);
    outtextxy(sx, sy + 32, buf);
  }
}

/* --- Level Banner --- */
void drawLevelBanner(int level)
{
  char buf[32];
  int tw, th;

  /* Nen banner */
  setfillstyle(SOLID_FILL, COL_BANNER_BG);
  bar(WIDTH / 2 - 200, HEIGHT / 2 - 60, WIDTH / 2 + 200, HEIGHT / 2 + 40);

  /* Vien doi */
  setcolor(COL_BANNER_BORDER);
  rectangle(WIDTH / 2 - 200, HEIGHT / 2 - 60, WIDTH / 2 + 200, HEIGHT / 2 + 40);
  rectangle(WIDTH / 2 - 202, HEIGHT / 2 - 62, WIDTH / 2 + 202, HEIGHT / 2 + 42);

  /* Tieu de LEVEL UP! */
  setcolor(COL_TEXT_TITLE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
  tw = textwidth((char *)"LEVEL UP!");
  th = textHeightCompat("LEVEL UP!");
  outtextxy(WIDTH / 2 - tw / 2, HEIGHT / 2 - 50 - th / 2, (char *)"LEVEL UP!");

  /* So level */
  setcolor(COL_TEXT_WHITE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  sprintf(buf, "Level %d", level);
  tw = textwidth(buf);
  outtextxy(WIDTH / 2 - tw / 2, HEIGHT / 2 + 5, buf);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* drawMenuButton(): Ve nut hamburger (3 gach ngang) goc trai tren. */
void drawMenuButton(int hovered)
{
  setfillstyle(SOLID_FILL, hovered ? COL_MENU_BTN_HOVER : COL_MENU_BTN_BG);
  bar(MENU_BTN_X1, MENU_BTN_Y1, MENU_BTN_X2, MENU_BTN_Y2);
  setcolor(hovered ? COL_TEXT_WHITE : COL_STAR_MED);
  rectangle(MENU_BTN_X1, MENU_BTN_Y1, MENU_BTN_X2, MENU_BTN_Y2);
  int cx = (MENU_BTN_X1 + MENU_BTN_X2) / 2;
  int cy = (MENU_BTN_Y1 + MENU_BTN_Y2) / 2;
  setcolor(hovered ? COL_MENU_BTN_LINE_H : COL_MENU_BTN_LINE);
  line(cx - 12, cy - 7, cx + 12, cy - 7);
  line(cx - 12, cy, cx + 12, cy);
  line(cx - 12, cy + 7, cx + 12, cy + 7);
}

#endif
