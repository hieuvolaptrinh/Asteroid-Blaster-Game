#ifndef DRAW_ASTEROID_H
#define DRAW_ASTEROID_H

/* ====================================================================== */
/*        ASTEROID DRAWING FUNCTIONS (floodfill -> fillpoly FIX)          */
/* ====================================================================== */

/*
 * drawAsteroid(): Ve thien thach voi nhieu lop chieu sau.
 * DA FIX: Thay floodfill() bang fillpoly() de tranh leak mau ra man hinh.
 */
void drawAsteroid(Asteroid a)
{
  int nV = 12, i;
  int poly[26]; /* 12 dinh + 1 dong */
  unsigned int sd;
  int cFill, cLine, cDark, cHighlight;

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
    cDark = DARKGRAY;
    cFill = LIGHTGRAY;
    cLine = WHITE;
    cHighlight = WHITE;
  }
  else if (a.radius <= 35)
  {
    cDark = RED;
    cFill = LIGHTRED;
    cLine = YELLOW;
    cHighlight = YELLOW;
  }
  else
  {
    cDark = BROWN;
    cFill = YELLOW;
    cLine = LIGHTRED;
    cHighlight = WHITE;
  }

  /* Lop 1: Bong diem (shadow) - dung fillpoly thay vi floodfill */
  {
    int shadowPoly[26];
    for (i = 0; i <= nV; i++)
    {
      shadowPoly[i * 2] = poly[i * 2] + 3;
      shadowPoly[i * 2 + 1] = poly[i * 2 + 1] + 3;
    }
    setfillstyle(SOLID_FILL, DARKGRAY);
    setcolor(DARKGRAY);
    fillpoly(nV + 1, shadowPoly);
  }

  /* Lop 2: Than chinh asteroid - dung fillpoly thay vi floodfill */
  setfillstyle(SOLID_FILL, cDark);
  setcolor(cDark);
  fillpoly(nV + 1, poly);

  /* Lop 3: Vung sang (highlight) - dung fillpoly thay vi floodfill */
  {
    int hlPoly[26];
    float hlScale = 0.7f;
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

  /* Ve lai vien ngoai cung */
  setcolor(cLine);
  for (i = 0; i < nV; i++)
  {
    int nx = (i + 1) % nV;
    line(poly[i * 2], poly[i * 2 + 1], poly[nx * 2], poly[nx * 2 + 1]);
  }

  /* Crater 1: lon */
  {
    int cx1 = (int)(a.x + cosf(a.rot + 0.8f) * a.radius * 0.3f);
    int cy1 = (int)(a.y + sinf(a.rot + 0.8f) * a.radius * 0.3f);
    int cr1 = a.radius / 4;
    if (cr1 < 2)
      cr1 = 2;
    setcolor(cDark);
    circle(cx1, cy1, cr1);
    if (cr1 > 3)
    {
      setcolor(cLine);
      arc(cx1 - 1, cy1 - 1, 200, 20, cr1 - 1);
    }
  }

  /* Crater 2: nho */
  {
    int cx2 = (int)(a.x + cosf(a.rot + 3.5f) * a.radius * 0.45f);
    int cy2 = (int)(a.y + sinf(a.rot + 3.5f) * a.radius * 0.45f);
    int cr2 = a.radius / 6;
    if (cr2 < 2)
      cr2 = 2;
    setcolor(cDark);
    circle(cx2, cy2, cr2);
  }

  /* Crater 3: rat nho */
  if (a.radius > 25)
  {
    int cx3 = (int)(a.x + cosf(a.rot + 5.0f) * a.radius * 0.2f);
    int cy3 = (int)(a.y + sinf(a.rot + 5.0f) * a.radius * 0.2f);
    int cr3 = a.radius / 8;
    if (cr3 < 2)
      cr3 = 2;
    setcolor(cDark);
    circle(cx3, cy3, cr3);
  }

  /* Vet nut / ran (surface cracks) */
  if (a.radius > 20)
  {
    setcolor(cDark);
    float ca1 = a.rot + 1.5f;
    int crk1x1 = (int)(a.x + cosf(ca1) * a.radius * 0.15f);
    int crk1y1 = (int)(a.y + sinf(ca1) * a.radius * 0.15f);
    int crk1x2 = (int)(a.x + cosf(ca1 + 0.3f) * a.radius * 0.55f);
    int crk1y2 = (int)(a.y + sinf(ca1 + 0.3f) * a.radius * 0.55f);
    line(crk1x1, crk1y1, crk1x2, crk1y2);
    int crk1x3 = (int)(a.x + cosf(ca1 + 0.6f) * a.radius * 0.4f);
    int crk1y3 = (int)(a.y + sinf(ca1 + 0.6f) * a.radius * 0.4f);
    line(crk1x2, crk1y2, crk1x3, crk1y3);
  }

  /* Diem sang highlight nho */
  {
    int hx = (int)(a.x + cosf(a.rot + 5.2f) * a.radius * 0.4f);
    int hy = (int)(a.y + sinf(a.rot + 5.2f) * a.radius * 0.4f);
    setfillstyle(SOLID_FILL, cHighlight);
    fillellipse(hx, hy, 2, 2);
  }
}

/* Thanh mau tren dau asteroid */
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

  setcolor(DARKGRAY);
  rectangle(x1, y1, x1 + bW, y1 + bH);
  if (a.hp * 2 > a.maxHp)
    setfillstyle(SOLID_FILL, GREEN);
  else if (a.hp * 4 > a.maxHp)
    setfillstyle(SOLID_FILL, YELLOW);
  else
    setfillstyle(SOLID_FILL, RED);
  bar(x1 + 1, y1 + 1, x1 + fillW, y1 + bH - 1);
}

/* drawAsteroids(): Ve tat ca asteroid dang active. */
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
