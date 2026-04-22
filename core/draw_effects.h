#ifndef DRAW_EFFECTS_H
#define DRAW_EFFECTS_H

/* ====================================================================== */
/*           EFFECTS DRAWING: Bullets, Items, Explosions, Skills          */
/* ====================================================================== */

/* --- Dan (glow halo) --- */
void drawBullets(Bullet b[], int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    if (!b[i].active)
      continue;
    int bx = (int)b[i].x, by = (int)b[i].y;
    int br = b[i].radius;
    setcolor(RED);
    circle(bx, by, br + 3);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(bx, by, br + 1, br + 1);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(bx, by, br > 1 ? br - 1 : 1, br > 1 ? br - 1 : 1);
  }
}

/* --- Vat pham: Tinh the suc manh (lap lanh + bieu tuong set) --- */
void drawItems(Item it[], int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    if (!it[i].active)
      continue;
    it[i].sparkle++;

    int cx = (int)it[i].x, cy = (int)it[i].y;
    int r = it[i].radius;

    /* Hinh kim cuong (diamond) */
    int dia[] = {cx, cy - r,
                 cx + r, cy,
                 cx, cy + r,
                 cx - r, cy,
                 cx, cy - r};

    int glow = (it[i].sparkle / 5) % 2;
    setfillstyle(SOLID_FILL, glow ? LIGHTCYAN : LIGHTMAGENTA);
    fillpoly(5, dia);

    setcolor(WHITE);
    line(cx, cy - r, cx + r, cy);
    line(cx + r, cy, cx, cy + r);
    line(cx, cy + r, cx - r, cy);
    line(cx - r, cy, cx, cy - r);

    /* Bieu tuong tia set (lightning bolt) */
    setcolor(YELLOW);
    line(cx - 2, cy - r + 4, cx + 2, cy - 2);
    line(cx + 2, cy - 2, cx - 2, cy + 1);
    line(cx - 2, cy + 1, cx + 2, cy + r - 4);

    /* Tia sparkle xoay quanh */
    {
      float sa = it[i].sparkle * 0.15f;
      int k;
      for (k = 0; k < 4; k++)
      {
        float a = sa + k * (float)M_PI / 2.0f;
        int sx1 = cx + (int)(cosf(a) * (r + 3));
        int sy1 = cy + (int)(sinf(a) * (r + 3));
        int sx2 = cx + (int)(cosf(a) * (r + 7));
        int sy2 = cy + (int)(sinf(a) * (r + 7));
        setcolor((k + it[i].sparkle / 3) % 2 ? WHITE : YELLOW);
        line(sx1, sy1, sx2, sy2);
      }
    }
  }
}

/* --- Hieu ung no (nhieu lop, manh vo, hat lua) --- */
void drawExplosions(Explosion e[], int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    if (!e[i].active)
      continue;
    float p = (float)e[i].frame / e[i].maxFrames;
    float r = e[i].maxRadius * p;
    int ex = (int)e[i].x, ey = (int)e[i].y;

    /* Lop 1: Vong sang ngoai cung (glow) */
    if (p < 0.6f)
    {
      setcolor(RED);
      circle(ex, ey, (int)(r * 1.3f));
    }

    /* Lop 2: Vong no chinh */
    int c;
    if (p < 0.2f)
      c = WHITE;
    else if (p < 0.4f)
      c = YELLOW;
    else if (p < 0.6f)
      c = LIGHTRED;
    else if (p < 0.8f)
      c = RED;
    else
      c = DARKGRAY;
    setcolor(c);
    circle(ex, ey, (int)r);

    /* Lop 3: Vong giua */
    if (r > 4)
    {
      int c2 = (p < 0.3f) ? YELLOW : ((p < 0.6f) ? LIGHTRED : RED);
      setcolor(c2);
      circle(ex, ey, (int)(r * 0.65f));
    }

    /* Lop 4: Loi no */
    if (r > 8)
    {
      int c3 = (p < 0.4f) ? WHITE : YELLOW;
      setcolor(c3);
      circle(ex, ey, (int)(r * 0.3f));
      if (p < 0.5f)
      {
        setfillstyle(SOLID_FILL, WHITE);
        fillellipse(ex, ey, 2, 2);
      }
    }

    /* Lop 5: Manh vo bay ra (debris lines) */
    {
      int k;
      unsigned int ds = (unsigned int)(e[i].x * 100 + e[i].y);
      int numDebris = 6;
      for (k = 0; k < numDebris; k++)
      {
        ds = seedRand(ds, k);
        float da = (float)k / numDebris * 2.0f * (float)M_PI +
                   (float)(ds % 100) / 100.0f;
        float dLen = r * (0.8f + (float)(ds % 40) / 100.0f);
        int dx1 = ex + (int)(cosf(da) * r * 0.4f);
        int dy1 = ey + (int)(sinf(da) * r * 0.4f);
        int dx2 = ex + (int)(cosf(da) * dLen);
        int dy2 = ey + (int)(sinf(da) * dLen);
        setcolor((p < 0.5f) ? YELLOW : DARKGRAY);
        line(dx1, dy1, dx2, dy2);
        if (p < 0.6f)
        {
          setfillstyle(SOLID_FILL, (p < 0.3f) ? WHITE : YELLOW);
          fillellipse(dx2, dy2, 1, 1);
        }
      }
    }
  }
}

/* --- Hieu ung skill --- */
void drawSkillEffects(SkillSystem *sk)
{
  /* Bomb Burst: vong tron no */
  if (sk->bombActive)
  {
    float p = (float)sk->bombFrame / 22.0f;
    float r = sk->bombRadius * p;
    int c;
    if (p < 0.3f)
      c = YELLOW;
    else if (p < 0.6f)
      c = LIGHTRED;
    else
      c = RED;
    setcolor(c);
    circle((int)sk->bombX, (int)sk->bombY, (int)r);
    circle((int)sk->bombX, (int)sk->bombY, (int)(r * 0.7f));
    if (r > 20)
      circle((int)sk->bombX, (int)sk->bombY, (int)(r * 0.4f));
  }

  /* Piercing Beam: tia sang */
  if (sk->beamActive)
  {
    float endX = sk->beamSX + sk->beamDX * 1500;
    float endY = sk->beamSY + sk->beamDY * 1500;
    setcolor(LIGHTCYAN);
    setlinestyle(SOLID_LINE, 0, 3);
    line((int)sk->beamSX, (int)sk->beamSY, (int)endX, (int)endY);
    setcolor(WHITE);
    setlinestyle(SOLID_LINE, 0, 1);
    line((int)sk->beamSX, (int)sk->beamSY, (int)endX, (int)endY);
  }

  /* Time Slow indicator */
  if (sk->slowActive)
  {
    setcolor(LIGHTBLUE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(WIDTH / 2 - 70, HEIGHT - 40,
              (char *)"~ SLOW ACTIVE ~");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  }
}

#endif
