#ifndef INTRODUCTION_H
#define INTRODUCTION_H

/* drawGuideScreen(): Hien thi huong dan choi game (tieng Viet, can giua) */
void drawGuideScreen(int mx, int my)
{
  int tw, th, btnCY;
  int backHov = pointInRect(mx, my, GUIDE_BACK_X1, GUIDE_BACK_Y1, GUIDE_BACK_X2,
                            GUIDE_BACK_Y2);

  cleardevice();
  drawStars();

  /* Tieu de (can giua) */
  setcolor(COL_TEXT_TITLE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
  tw = textwidth((char *)"HUONG DAN CHOI");
  outtextxy(WIDTH / 2 - tw / 2, 20, (char *)"HUONG DAN CHOI");

  /* --- DIEU KHIEN --- */
  setcolor(COL_TEXT_SUBTITLE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  outtextxy(100, 70, (char *)"DIEU KHIEN:");

  setcolor(COL_TEXT_BODY);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  outtextxy(120, 100, (char *)"WASD / Phim mui ten  -  Di chuyen phi thuyen");
  outtextxy(120, 122, (char *)"Chuot                -  Ngam huong ban");
  outtextxy(120, 144, (char *)"Tu dong ban          -  Phi thuyen tu dong ban dan");
  outtextxy(120, 166, (char *)"ESC                  -  Tam dung game");

  /* --- TOC DO GAME --- */
  setcolor(COL_TEXT_SUBTITLE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  outtextxy(100, 200, (char *)"TOC DO GAME:");

  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  setcolor(COL_TEXT_DANGER);
  outtextxy(120, 230, (char *)"[0] DUNG       -  Tam dung game (hien menu Pause)");
  setcolor(COL_TEXT_SUBTITLE);
  outtextxy(120, 252, (char *)"[1] CHAM       -  Giam toc do game");
  setcolor(COL_TEXT_SUCCESS);
  outtextxy(120, 274, (char *)"[2] BINH THUONG-  Toc do mac dinh");
  setcolor(COL_TEXT_TITLE);
  outtextxy(120, 296, (char *)"[3] NHANH      -  Tang toc do game");

  /* --- KY NANG (phim 7-8-9) --- */
  setcolor(COL_TEXT_SUBTITLE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  outtextxy(100, 330, (char *)"KY NANG:");

  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  setcolor(COL_BOMB_HOT);
  outtextxy(120, 360, (char *)"[7] BOM NO     -  No vung tron quanh phi thuyen (CD: 8s)");

  setcolor(COL_SLOW_TEXT);
  outtextxy(120, 382, (char *)"[8] LAM CHAM   -  Lam cham thien thach 4 giay (CD: 12s)");

  setcolor(COL_BEAM_OUTER);
  outtextxy(120, 404, (char *)"[9] TIA XUYEN  -  Ban tia laser xuyen qua (CD: 10s)");

  /* --- MUC TIEU --- */
  setcolor(COL_TEXT_SUBTITLE);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  outtextxy(100, 440, (char *)"MUC TIEU:");

  setcolor(COL_TEXT_BODY);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  outtextxy(120, 470, (char *)"- Pha huy thien thach de ghi diem");
  outtextxy(120, 492, (char *)"- Thu thap tinh the nang luong de nang cap");
  outtextxy(120, 514, (char *)"- Dat 1000 diem moi cap de len level");
  outtextxy(120, 536, (char *)"- Song sot cang lau cang tot!");

  /* --- TINH THE NANG CAP --- */
  setcolor(COL_CRYSTAL_B);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
  outtextxy(100, 565, (char *)"TINH THE NANG CAP:");

  setcolor(COL_TEXT_BODY);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  outtextxy(120, 595, (char *)"Xoay vong: So dan -> Sat thuong -> Toc do ban");

  /* --- Nut QUAY LAI (can giua) --- */
  setfillstyle(SOLID_FILL, backHov ? COL_BTN_HOVER_CYAN : COL_BTN_NORMAL);
  bar(GUIDE_BACK_X1, GUIDE_BACK_Y1, GUIDE_BACK_X2, GUIDE_BACK_Y2);
  setcolor(backHov ? COL_BTN_TEXT_HOV : COL_BTN_TEXT);
  rectangle(GUIDE_BACK_X1, GUIDE_BACK_Y1, GUIDE_BACK_X2, GUIDE_BACK_Y2);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
  tw = textwidth((char *)"QUAY LAI");
  th = textHeightCompat("QUAY LAI");
  btnCY = (GUIDE_BACK_Y1 + GUIDE_BACK_Y2) / 2;
  outtextxy(WIDTH / 2 - tw / 2, btnCY - th / 2, (char *)"QUAY LAI");

  settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
}

/* handleGuideClick(): Kiem tra click nut BACK
 * Tra ve: 1=Back, 0=khong click */
int handleGuideClick(int mx, int my)
{
  if (pointInRect(mx, my, GUIDE_BACK_X1, GUIDE_BACK_Y1, GUIDE_BACK_X2,
                  GUIDE_BACK_Y2))
    return 1;
  return 0;
}

#endif
