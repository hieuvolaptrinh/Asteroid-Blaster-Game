#ifndef DRAW_ASTEROID_H
#define DRAW_ASTEROID_H

/* ====================================================================== */
/*        ASTEROID DRAWING FUNCTIONS (Enhanced with gradient & depth)      */
/* ====================================================================== */

/*
 * drawAsteroid(): Ve thien thach voi nhieu lop chieu sau,
 * gradient mau, crater co chieu sau 3D, va hieu ung anh sang.
 */
void drawAsteroid(Asteroid a)
{
  int nV = 12, i;
  int poly[26]; /* 12 dinh + 1 dong */
  unsigned int sd;
  int cFill, cDark, cMid, cLine, cHighlight, cCrater;

  if (!a.active)
    return;

  sd = (unsigned int)a.seed;
  for (i = 0; i < nV; i++)
  {
    float va = a.rot + (float)i / nV * 2.0f * (float)M_PI;
    sd = seedRand(sd, i);
    float rv = 0.78f + (float)(sd % 44) / 100.0f;
    poly[i * 2] = (int)(a.x + cosf(va) * a.radius * rv);
    poly[i * 2 + 1] = (int)(a.y + sinf(va) * a.radius * rv);
  }
  poly[nV * 2] = poly[0];
  poly[nV * 2 + 1] = poly[1];

  /* Bang mau theo kich thuoc asteroid */
  if (a.radius <= 22)
  {
    cDark = COL_AST_S_DARK;
    cMid = COL_AST_S_MID;
    cFill = COL_AST_S_FILL;
    cLine = COL_AST_S_LINE;
    cHighlight = COL_AST_S_HL;
    cCrater = COL_AST_S_CRATER;
  }
  else if (a.radius <= 35)
  {
    cDark = COL_AST_M_DARK;
    cMid = COL_AST_M_MID;
    cFill = COL_AST_M_FILL;
    cLine = COL_AST_M_LINE;
    cHighlight = COL_AST_M_HL;
    cCrater = COL_AST_M_CRATER;
  }
  else
  {
    cDark = COL_AST_L_DARK;
    cMid = COL_AST_L_MID;
    cFill = COL_AST_L_FILL;
    cLine = COL_AST_L_LINE;
    cHighlight = COL_AST_L_HL;
    cCrater = COL_AST_L_CRATER;
  }

  /* ---- Lop 1: Bong dim (soft shadow) ---- */
  {
    int shadowPoly[26];
    for (i = 0; i <= nV; i++)
    {
      shadowPoly[i * 2] = poly[i * 2] + 4;
      shadowPoly[i * 2 + 1] = poly[i * 2 + 1] + 4;
    }
    setfillstyle(SOLID_FILL, COL_AST_SHADOW);
    setcolor(COL_AST_SHADOW);
    fillpoly(nV + 1, shadowPoly);
  }

  /* ---- Lop 2: Than chinh (dark base) ---- */
  setfillstyle(SOLID_FILL, cDark);
  setcolor(cDark);
  fillpoly(nV + 1, poly);

  /* ---- Lop 3: Lop giua (mid-tone) – tao gradient tu shadow sang body ---- */
  {
    int midPoly[26];
    float midScale = 0.88f;
    float mOffA = a.rot + 5.8f;
    float mox = cosf(mOffA) * a.radius * 0.06f;
    float moy = sinf(mOffA) * a.radius * 0.06f;
    for (i = 0; i < nV; i++)
    {
      float va2 = a.rot + (float)i / nV * 2.0f * (float)M_PI;
      sd = seedRand((unsigned int)a.seed, i);
      float rv2 = 0.78f + (float)(sd % 44) / 100.0f;
      midPoly[i * 2] = (int)(a.x + mox + cosf(va2) * a.radius * rv2 * midScale);
      midPoly[i * 2 + 1] =
          (int)(a.y + moy + sinf(va2) * a.radius * rv2 * midScale);
    }
    midPoly[nV * 2] = midPoly[0];
    midPoly[nV * 2 + 1] = midPoly[1];
    setfillstyle(SOLID_FILL, cMid);
    setcolor(cMid);
    fillpoly(nV + 1, midPoly);
  }

  /* ---- Lop 4: Vung sang (highlight zone) ---- */
  {
    int hlPoly[26];
    float hlScale = 0.65f;
    float offsetA = a.rot + 5.5f;
    float ox = cosf(offsetA) * a.radius * 0.15f;
    float oy = sinf(offsetA) * a.radius * 0.15f;
    for (i = 0; i < nV; i++)
    {
      float va2 = a.rot + (float)i / nV * 2.0f * (float)M_PI;
      sd = seedRand((unsigned int)a.seed, i);
      float rv2 = 0.78f + (float)(sd % 44) / 100.0f;
      hlPoly[i * 2] = (int)(a.x + ox + cosf(va2) * a.radius * rv2 * hlScale);
      hlPoly[i * 2 + 1] =
          (int)(a.y + oy + sinf(va2) * a.radius * rv2 * hlScale);
    }
    hlPoly[nV * 2] = hlPoly[0];
    hlPoly[nV * 2 + 1] = hlPoly[1];
    setfillstyle(SOLID_FILL, cFill);
    setcolor(cFill);
    fillpoly(nV + 1, hlPoly);
  }

  /* ---- Vien ngoai cung (edge lines) ---- */
  setcolor(cLine);
  for (i = 0; i < nV; i++)
  {
    int nx = (i + 1) % nV;
    line(poly[i * 2], poly[i * 2 + 1], poly[nx * 2], poly[nx * 2 + 1]);
  }

  /* ---- Vien dim (dark inner edge) de tao chieu sau hon ---- */
  {
    int innerEdge[26];
    float edgeScale = 0.94f;
    for (i = 0; i < nV; i++)
    {
      float va2 = a.rot + (float)i / nV * 2.0f * (float)M_PI;
      sd = seedRand((unsigned int)a.seed, i);
      float rv2 = 0.78f + (float)(sd % 44) / 100.0f;
      innerEdge[i * 2] = (int)(a.x + cosf(va2) * a.radius * rv2 * edgeScale);
      innerEdge[i * 2 + 1] = (int)(a.y + sinf(va2) * a.radius * rv2 * edgeScale);
    }
    setcolor(colorLerp(cDark, cMid, 0.4f));
    for (i = 0; i < nV; i++)
    {
      int nx = (i + 1) % nV;
      line(innerEdge[i * 2], innerEdge[i * 2 + 1],
           innerEdge[nx * 2], innerEdge[nx * 2 + 1]);
    }
  }

  /* ---- Crater 1: lon – voi bong va highlight rim ---- */
  {
    int cx1 = (int)(a.x + cosf(a.rot + 0.8f) * a.radius * 0.3f);
    int cy1 = (int)(a.y + sinf(a.rot + 0.8f) * a.radius * 0.3f);
    int cr1 = a.radius / 4;
    if (cr1 < 2) cr1 = 2;

    /* Bong crater */
    setfillstyle(SOLID_FILL, cCrater);
    fillellipse(cx1 + 1, cy1 + 1, cr1, cr1);

    /* Vien crater */
    setcolor(cDark);
    circle(cx1, cy1, cr1);

    /* Highlight rim (phia doi dien anh sang) */
    if (cr1 > 3)
    {
      setcolor(colorLerp(cFill, cHighlight, 0.5f));
      arc(cx1 - 1, cy1 - 1, 200, 20, cr1 - 1);
    }
  }

  /* ---- Crater 2: nho ---- */
  {
    int cx2 = (int)(a.x + cosf(a.rot + 3.5f) * a.radius * 0.45f);
    int cy2 = (int)(a.y + sinf(a.rot + 3.5f) * a.radius * 0.45f);
    int cr2 = a.radius / 6;
    if (cr2 < 2) cr2 = 2;
    setfillstyle(SOLID_FILL, cCrater);
    fillellipse(cx2, cy2, cr2, cr2);
    setcolor(cDark);
    circle(cx2, cy2, cr2);
    if (cr2 > 2)
    {
      setcolor(colorLerp(cMid, cFill, 0.6f));
      arc(cx2 - 1, cy2 - 1, 210, 30, cr2 - 1);
    }
  }

  /* ---- Crater 3: rat nho (chi asteroid lon) ---- */
  if (a.radius > 25)
  {
    int cx3 = (int)(a.x + cosf(a.rot + 5.0f) * a.radius * 0.2f);
    int cy3 = (int)(a.y + sinf(a.rot + 5.0f) * a.radius * 0.2f);
    int cr3 = a.radius / 8;
    if (cr3 < 2) cr3 = 2;
    setfillstyle(SOLID_FILL, cCrater);
    fillellipse(cx3, cy3, cr3, cr3);
    setcolor(cDark);
    circle(cx3, cy3, cr3);
  }

  /* ---- Vet nut / surface cracks ---- */
  if (a.radius > 20)
  {
    setcolor(colorLerp(cCrater, cDark, 0.5f));
    float ca1 = a.rot + 1.5f;
    int crk1x1 = (int)(a.x + cosf(ca1) * a.radius * 0.15f);
    int crk1y1 = (int)(a.y + sinf(ca1) * a.radius * 0.15f);
    int crk1x2 = (int)(a.x + cosf(ca1 + 0.3f) * a.radius * 0.55f);
    int crk1y2 = (int)(a.y + sinf(ca1 + 0.3f) * a.radius * 0.55f);
    line(crk1x1, crk1y1, crk1x2, crk1y2);
    int crk1x3 = (int)(a.x + cosf(ca1 + 0.6f) * a.radius * 0.4f);
    int crk1y3 = (int)(a.y + sinf(ca1 + 0.6f) * a.radius * 0.4f);
    line(crk1x2, crk1y2, crk1x3, crk1y3);

    /* Them 1 vet nut phia doi dien */
    if (a.radius > 30)
    {
      float ca2 = a.rot + 4.2f;
      int crk2x1 = (int)(a.x + cosf(ca2) * a.radius * 0.2f);
      int crk2y1 = (int)(a.y + sinf(ca2) * a.radius * 0.2f);
      int crk2x2 = (int)(a.x + cosf(ca2 + 0.4f) * a.radius * 0.5f);
      int crk2y2 = (int)(a.y + sinf(ca2 + 0.4f) * a.radius * 0.5f);
      line(crk2x1, crk2y1, crk2x2, crk2y2);
    }
  }

  /* ---- Diem sang highlight nho (specular) ---- */
  {
    int hx = (int)(a.x + cosf(a.rot + 5.2f) * a.radius * 0.4f);
    int hy = (int)(a.y + sinf(a.rot + 5.2f) * a.radius * 0.4f);
    setfillstyle(SOLID_FILL, cHighlight);
    fillellipse(hx, hy, 2, 2);
    /* Them 1 diem sang nho hon ben canh */
    int hx2 = (int)(a.x + cosf(a.rot + 5.5f) * a.radius * 0.28f);
    int hy2 = (int)(a.y + sinf(a.rot + 5.5f) * a.radius * 0.28f);
    setfillstyle(SOLID_FILL, colorLerp(cFill, cHighlight, 0.7f));
    fillellipse(hx2, hy2, 1, 1);
  }
}

/* ====================================================================== */
/*                    ASTEROID HP BAR (Thanh mau tren dau)                 */
/* ====================================================================== */
void drawAsteroidHPBar(Asteroid a)
{
  if (!a.active || a.maxHp <= 1)
    return;
  int bW = a.radius * 2, bH = 4;
  int x1 = (int)(a.x - bW / 2);
  int y1 = (int)(a.y - a.radius - 10);
  int fillW = (int)(bW * (float)a.hp / a.maxHp);
  if (fillW < 0)
    fillW = 0;

  /* Nen thanh HP */
  setfillstyle(SOLID_FILL, COL_HP_BG);
  bar(x1, y1, x1 + bW, y1 + bH);

  /* Vien thanh HP */
  setcolor(COL_HP_BORDER);
  rectangle(x1, y1, x1 + bW, y1 + bH);

  /* Mau thanh HP theo trang thai */
  int hpColor;
  if (a.hp * 2 > a.maxHp)
    hpColor = COL_HP_HIGH;
  else if (a.hp * 4 > a.maxHp)
    hpColor = COL_HP_MED;
  else
    hpColor = COL_HP_LOW;

  setfillstyle(SOLID_FILL, hpColor);
  bar(x1 + 1, y1 + 1, x1 + fillW, y1 + bH - 1);

  /* Highlight nho tren thanh HP */
  if (fillW > 4)
  {
    setcolor(colorLerp(hpColor, COL_TEXT_WHITE, 0.3f));
    line(x1 + 2, y1 + 1, x1 + fillW - 1, y1 + 1);
  }
}

/* ====================================================================== */
/*                    drawAsteroids(): Ve tat ca asteroid                  */
/* ====================================================================== */
void drawAsteroids(Asteroid a[], int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    drawAsteroid(a[i]);
    drawAsteroidHPBar(a[i]);
  }
}

#endif
