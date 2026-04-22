#ifndef DRAW_SHIP_H
#define DRAW_SHIP_H

/* ====================================================================== */
/*                  SHIP DRAWING FUNCTIONS (Enhanced)                      */
/* ====================================================================== */

/* drawShipBody(): Ve than phi thuyen voi nhieu lop gradient va chieu sau. */
void drawShipBody(Ship s, float angle)
{
  int nx = (int)(s.x + cosf(angle) * (s.radius + 16));
  int ny = (int)(s.y + sinf(angle) * (s.radius + 16));
  int lx = (int)(s.x + cosf(angle + 2.5f) * s.radius);
  int ly = (int)(s.y + sinf(angle + 2.5f) * s.radius);
  int rx = (int)(s.x + cosf(angle - 2.5f) * s.radius);
  int ry = (int)(s.y + sinf(angle - 2.5f) * s.radius);

  /* ---- Lop bong (shadow) ---- */
  {
    int snx = nx + 3, sny = ny + 3;
    int slx = lx + 3, sly = ly + 3;
    int srx = rx + 3, sry = ry + 3;
    setfillstyle(SOLID_FILL, COL_SHIP_SHADOW);
    int shadow[] = {snx, sny, slx, sly, srx, sry, snx, sny};
    fillpoly(4, shadow);
  }

  /* ---- Than chinh: lop dark base ---- */
  setfillstyle(SOLID_FILL, COL_SHIP_BODY);
  int body[] = {nx, ny, lx, ly, rx, ry, nx, ny};
  fillpoly(4, body);

  /* ---- Highlight lop 1 – vung sang chinh ---- */
  {
    float hs = 0.7f;
    int hnx = (int)(s.x + cosf(angle) * (s.radius + 16) * 0.95f);
    int hny = (int)(s.y + sinf(angle) * (s.radius + 16) * 0.95f);
    int hlx = (int)(s.x + cosf(angle + 2.3f) * s.radius * hs);
    int hly = (int)(s.y + sinf(angle + 2.3f) * s.radius * hs);
    int hrx = (int)(s.x + cosf(angle - 2.3f) * s.radius * hs);
    int hry = (int)(s.y + sinf(angle - 2.3f) * s.radius * hs);
    setfillstyle(SOLID_FILL, COL_SHIP_HL1);
    int hl[] = {hnx, hny, hlx, hly, hrx, hry, hnx, hny};
    fillpoly(4, hl);
  }

  /* ---- Highlight lop 2 – vung sang nho hon ---- */
  {
    float hs2 = 0.45f;
    int h2nx = (int)(s.x + cosf(angle) * (s.radius + 14) * 0.82f);
    int h2ny = (int)(s.y + sinf(angle) * (s.radius + 14) * 0.82f);
    int h2lx = (int)(s.x + cosf(angle + 2.1f) * s.radius * hs2);
    int h2ly = (int)(s.y + sinf(angle + 2.1f) * s.radius * hs2);
    int h2rx = (int)(s.x + cosf(angle - 2.1f) * s.radius * hs2);
    int h2ry = (int)(s.y + sinf(angle - 2.1f) * s.radius * hs2);
    setfillstyle(SOLID_FILL, COL_SHIP_HL2);
    int h2[] = {h2nx, h2ny, h2lx, h2ly, h2rx, h2ry, h2nx, h2ny};
    fillpoly(4, h2);
  }

  /* ---- Highlight lop 3 – specular nho nhat ---- */
  {
    float hs3 = 0.25f;
    int h3nx = (int)(s.x + cosf(angle) * (s.radius + 12) * 0.65f);
    int h3ny = (int)(s.y + sinf(angle) * (s.radius + 12) * 0.65f);
    int h3lx = (int)(s.x + cosf(angle + 1.9f) * s.radius * hs3);
    int h3ly = (int)(s.y + sinf(angle + 1.9f) * s.radius * hs3);
    int h3rx = (int)(s.x + cosf(angle - 1.9f) * s.radius * hs3);
    int h3ry = (int)(s.y + sinf(angle - 1.9f) * s.radius * hs3);
    setfillstyle(SOLID_FILL, COL_SHIP_HL3);
    int h3[] = {h3nx, h3ny, h3lx, h3ly, h3rx, h3ry, h3nx, h3ny};
    fillpoly(4, h3);
  }

  /* ---- Vien sang (edge highlights) ---- */
  setcolor(COL_SHIP_EDGE2);
  line(nx, ny, lx, ly);
  line(nx, ny, rx, ry);
  setcolor(COL_SHIP_EDGE);
  line(lx, ly, rx, ry);

  /* ---- Canh trai ---- */
  {
    int w1x = (int)(s.x + cosf(angle + 2.0f) * s.radius * 0.7f);
    int w1y = (int)(s.y + sinf(angle + 2.0f) * s.radius * 0.7f);
    int w2x = (int)(s.x + cosf(angle + 1.7f) * (s.radius + 14));
    int w2y = (int)(s.y + sinf(angle + 1.7f) * (s.radius + 14));
    int w3x = (int)(s.x + cosf(angle + 2.8f) * s.radius * 1.15f);
    int w3y = (int)(s.y + sinf(angle + 2.8f) * s.radius * 1.15f);

    /* Bong canh */
    {
      int sw1x = w1x + 2, sw1y = w1y + 2;
      int sw2x = w2x + 2, sw2y = w2y + 2;
      int sw3x = w3x + 2, sw3y = w3y + 2;
      setfillstyle(SOLID_FILL, COL_SHIP_SHADOW);
      int swL[] = {sw1x, sw1y, sw2x, sw2y, sw3x, sw3y, sw1x, sw1y};
      fillpoly(4, swL);
    }

    /* Than canh */
    setfillstyle(SOLID_FILL, COL_SHIP_WING);
    int wL[] = {w1x, w1y, w2x, w2y, w3x, w3y, w1x, w1y};
    fillpoly(4, wL);

    /* Highlight canh */
    {
      int hw1x = (int)(s.x + cosf(angle + 2.05f) * s.radius * 0.6f);
      int hw1y = (int)(s.y + sinf(angle + 2.05f) * s.radius * 0.6f);
      int hw2x = (int)(s.x + cosf(angle + 1.75f) * (s.radius + 8));
      int hw2y = (int)(s.y + sinf(angle + 1.75f) * (s.radius + 8));
      int hw3x = (int)(s.x + cosf(angle + 2.7f) * s.radius * 0.9f);
      int hw3y = (int)(s.y + sinf(angle + 2.7f) * s.radius * 0.9f);
      setfillstyle(SOLID_FILL, COL_SHIP_WING_HL);
      int hwL[] = {hw1x, hw1y, hw2x, hw2y, hw3x, hw3y, hw1x, hw1y};
      fillpoly(4, hwL);
    }
    setcolor(COL_SHIP_EDGE);
    line(w1x, w1y, w2x, w2y);
    line(w2x, w2y, w3x, w3y);
  }

  /* ---- Canh phai ---- */
  {
    int w1x = (int)(s.x + cosf(angle - 2.0f) * s.radius * 0.7f);
    int w1y = (int)(s.y + sinf(angle - 2.0f) * s.radius * 0.7f);
    int w2x = (int)(s.x + cosf(angle - 1.7f) * (s.radius + 14));
    int w2y = (int)(s.y + sinf(angle - 1.7f) * (s.radius + 14));
    int w3x = (int)(s.x + cosf(angle - 2.8f) * s.radius * 1.15f);
    int w3y = (int)(s.y + sinf(angle - 2.8f) * s.radius * 1.15f);

    /* Bong canh */
    {
      int sw1x = w1x + 2, sw1y = w1y + 2;
      int sw2x = w2x + 2, sw2y = w2y + 2;
      int sw3x = w3x + 2, sw3y = w3y + 2;
      setfillstyle(SOLID_FILL, COL_SHIP_SHADOW);
      int swR[] = {sw1x, sw1y, sw2x, sw2y, sw3x, sw3y, sw1x, sw1y};
      fillpoly(4, swR);
    }

    /* Than canh */
    setfillstyle(SOLID_FILL, COL_SHIP_WING);
    int wR[] = {w1x, w1y, w2x, w2y, w3x, w3y, w1x, w1y};
    fillpoly(4, wR);

    /* Highlight canh */
    {
      int hw1x = (int)(s.x + cosf(angle - 2.05f) * s.radius * 0.6f);
      int hw1y = (int)(s.y + sinf(angle - 2.05f) * s.radius * 0.6f);
      int hw2x = (int)(s.x + cosf(angle - 1.75f) * (s.radius + 8));
      int hw2y = (int)(s.y + sinf(angle - 1.75f) * (s.radius + 8));
      int hw3x = (int)(s.x + cosf(angle - 2.7f) * s.radius * 0.9f);
      int hw3y = (int)(s.y + sinf(angle - 2.7f) * s.radius * 0.9f);
      setfillstyle(SOLID_FILL, COL_SHIP_WING_HL);
      int hwR[] = {hw1x, hw1y, hw2x, hw2y, hw3x, hw3y, hw1x, hw1y};
      fillpoly(4, hwR);
    }
    setcolor(COL_SHIP_EDGE);
    line(w1x, w1y, w2x, w2y);
    line(w2x, w2y, w3x, w3y);
  }

  /* ---- Kinh lai (cockpit) voi nhieu lop ---- */
  {
    int cx = (int)(s.x + cosf(angle) * s.radius * 0.35f);
    int cy = (int)(s.y + sinf(angle) * s.radius * 0.35f);

    /* Vien ngoai cockpit */
    setfillstyle(SOLID_FILL, COL_SHIP_HL1);
    fillellipse(cx, cy, 7, 6);

    /* Glass cockpit */
    setfillstyle(SOLID_FILL, COL_SHIP_COCKPIT);
    fillellipse(cx, cy, 6, 5);

    /* Anh sang phan chieu */
    setfillstyle(SOLID_FILL, COL_SHIP_COCKPIT_HL);
    fillellipse(cx - 2, cy - 2, 2, 2);

    /* Vien cockpit */
    setcolor(COL_SHIP_EDGE);
    ellipse(cx, cy, 0, 360, 7, 6);
  }
}

/* drawShipEngineFlame(): Ve luoi lua dong co voi gradient 3 lop. */
void drawShipEngineFlame(Ship s, float angle, int frame)
{
  float perp = angle + (float)M_PI / 2.0f;
  int eng;
  float engOff[] = {0.35f, -0.35f};

  for (eng = 0; eng < 2; eng++)
  {
    float ebx = s.x - cosf(angle) * s.radius * 0.4f +
                cosf(perp) * s.radius * engOff[eng];
    float eby = s.y - sinf(angle) * s.radius * 0.4f +
                sinf(perp) * s.radius * engOff[eng];

    float fLen = 12.0f + (frame % 6) * 3.0f;
    float hW = 4.0f + (frame % 3);

    /* ---- Lop 1: Glow ngoai cung (hao quang lua) ---- */
    {
      float gLen = fLen * 1.15f;
      float gW = hW * 1.4f;
      int gtx = (int)(ebx - cosf(angle) * gLen);
      int gty = (int)(eby - sinf(angle) * gLen);
      int g1x = (int)(ebx + cosf(perp) * gW);
      int g1y = (int)(eby + sinf(perp) * gW);
      int g2x = (int)(ebx - cosf(perp) * gW);
      int g2y = (int)(eby - sinf(perp) * gW);
      setfillstyle(SOLID_FILL, COL_FLAME_SPARK);
      int fg[] = {g1x, g1y, gtx, gty, g2x, g2y, g1x, g1y};
      fillpoly(4, fg);
    }

    /* ---- Lop 2: Lua ngoai – do/cam ---- */
    {
      int tx = (int)(ebx - cosf(angle) * fLen);
      int ty = (int)(eby - sinf(angle) * fLen);
      int e1x = (int)(ebx + cosf(perp) * hW);
      int e1y = (int)(eby + sinf(perp) * hW);
      int e2x = (int)(ebx - cosf(perp) * hW);
      int e2y = (int)(eby - sinf(perp) * hW);
      int outerCol = ((frame + eng) % 3 == 0) ? COL_FLAME_OUTER_ALT : COL_FLAME_OUTER;
      setfillstyle(SOLID_FILL, outerCol);
      int fl[] = {e1x, e1y, tx, ty, e2x, e2y, e1x, e1y};
      fillpoly(4, fl);
    }

    /* ---- Lop 3: Lua giua – vang ---- */
    {
      float iL = fLen * 0.6f, iW = hW * 0.5f;
      int itx = (int)(ebx - cosf(angle) * iL);
      int ity = (int)(eby - sinf(angle) * iL);
      int i1x = (int)(ebx + cosf(perp) * iW);
      int i1y = (int)(eby + sinf(perp) * iW);
      int i2x = (int)(ebx - cosf(perp) * iW);
      int i2y = (int)(eby - sinf(perp) * iW);
      setfillstyle(SOLID_FILL, COL_FLAME_MID);
      int fi[] = {i1x, i1y, itx, ity, i2x, i2y, i1x, i1y};
      fillpoly(4, fi);
    }

    /* ---- Lop 4: Loi lua – trang am ---- */
    {
      float cL = fLen * 0.3f, cW = hW * 0.25f;
      int ctx = (int)(ebx - cosf(angle) * cL);
      int cty = (int)(eby - sinf(angle) * cL);
      int c1x = (int)(ebx + cosf(perp) * cW);
      int c1y = (int)(eby + sinf(perp) * cW);
      int c2x = (int)(ebx - cosf(perp) * cW);
      int c2y = (int)(eby - sinf(perp) * cW);
      setfillstyle(SOLID_FILL, COL_FLAME_CORE);
      int fc[] = {c1x, c1y, ctx, cty, c2x, c2y, c1x, c1y};
      fillpoly(4, fc);
    }
  }
}

/* drawShipShieldAura(): Ve hao quang bao ve quanh tau voi gradient. */
static void drawShipShieldAura(Ship s, int frame)
{
  int auraR = s.radius + 8 + (frame % 8) / 2;

  /* Vong ngoai mo */
  setcolor(COL_AURA_OUTER);
  circle((int)s.x, (int)s.y, auraR + 4);

  /* Vong chinh – nhap nhay */
  setcolor((frame / 4) % 2 ? COL_AURA_INNER : COL_AURA_ALT);
  circle((int)s.x, (int)s.y, auraR);

  /* Vong trong mo */
  setcolor(colorLerp(COL_AURA_INNER, COL_AURA_OUTER, 0.6f));
  circle((int)s.x, (int)s.y, auraR - 2);
}

/* drawShip(): Ve toan bo phi thuyen (aura, lua, than, duong ngam, crosshair). */
void drawShip(Ship s, int mx, int my, int frame)
{
  float angle = atan2f((float)(my - s.y), (float)(mx - s.x));

  drawShipShieldAura(s, frame);
  drawShipEngineFlame(s, angle, frame);
  drawShipBody(s, angle);

  /* ---- Duong ngam ---- */
  setcolor(COL_AIM_LINE);
  setlinestyle(DASHED_LINE, 0, 1);
  float dx = (float)(mx - s.x), dy = (float)(my - s.y);
  float ln = sqrtf(dx * dx + dy * dy);
  if (ln > 1.0f)
  {
    int ax = (int)(s.x + dx / ln * 80);
    int ay = (int)(s.y + dy / ln * 80);
    line((int)s.x, (int)s.y, ax, ay);
  }
  setlinestyle(SOLID_LINE, 0, 1);

  /* ---- Crosshair (Tam ngam) ---- */
  {
    int cr = 10 + (frame % 6) / 3;

    /* Vong ngoai */
    setcolor(COL_CROSS_OUTER);
    circle(mx, my, cr + 4);

    /* Vong trong */
    setcolor(COL_CROSS_INNER);
    circle(mx, my, cr);

    /* 4 thanh ngang/doc */
    setcolor(COL_CROSS_INNER);
    line(mx - 14, my, mx - 5, my);
    line(mx + 5, my, mx + 14, my);
    line(mx, my - 14, mx, my - 5);
    line(mx, my + 5, mx, my + 14);

    /* Tam diem trung tam */
    setfillstyle(SOLID_FILL, COL_CROSS_DOT);
    fillellipse(mx, my, 1, 1);
  }
}

#endif
