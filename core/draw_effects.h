#ifndef DRAW_EFFECTS_H
#define DRAW_EFFECTS_H

/* ====================================================================== */
/*      EFFECTS DRAWING: Bullets, Items, Explosions, Skills (Enhanced)     */
/* ====================================================================== */

/* --- Dan (glow halo) voi gradient 3 lop + trail --- */
void drawBullets(Bullet b[], int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    if (!b[i].active)
      continue;
    int bx = (int)b[i].x, by = (int)b[i].y;
    int br = b[i].radius;

    /* Lop 1: Hao quang ngoai – cam do */
    setcolor(COL_BULLET_GLOW);
    circle(bx, by, br + 4);
    circle(bx, by, br + 3);

    /* Lop 2: Trail mo phia sau */
    {
      float tLen = 6.0f;
      float bLen = sqrtf(b[i].dx * b[i].dx + b[i].dy * b[i].dy);
      if (bLen > 0.1f)
      {
        int tx = bx - (int)(b[i].dx / bLen * tLen);
        int ty = by - (int)(b[i].dy / bLen * tLen);
        setcolor(COL_BULLET_TRAIL);
        line(bx, by, tx, ty);
      }
    }

    /* Lop 3: Than dan – vang */
    setfillstyle(SOLID_FILL, COL_BULLET_FILL);
    fillellipse(bx, by, br + 1, br + 1);

    /* Lop 4: Loi sang – trang am */
    setfillstyle(SOLID_FILL, COL_BULLET_CORE);
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

    /* Hao quang mo xung quanh crystal */
    setcolor(COL_CRYSTAL_GLOW);
    circle(cx, cy, r + 5);

    /* Hinh kim cuong (diamond) */
    int dia[] = {cx, cy - r,
                 cx + r, cy,
                 cx, cy + r,
                 cx - r, cy,
                 cx, cy - r};

    int glow = (it[i].sparkle / 5) % 2;
    setfillstyle(SOLID_FILL, glow ? COL_CRYSTAL_A : COL_CRYSTAL_B);
    fillpoly(5, dia);

    /* Vien crystal – sang */
    setcolor(COL_CRYSTAL_EDGE);
    line(cx, cy - r, cx + r, cy);
    line(cx + r, cy, cx, cy + r);
    line(cx, cy + r, cx - r, cy);
    line(cx - r, cy, cx, cy - r);

    /* Inner diamond highlight */
    {
      int innerR = r / 2;
      if (innerR > 2)
      {
        int innerDia[] = {cx, cy - innerR,
                          cx + innerR, cy,
                          cx, cy + innerR,
                          cx - innerR, cy,
                          cx, cy - innerR};
        setfillstyle(SOLID_FILL, colorLerp(glow ? COL_CRYSTAL_A : COL_CRYSTAL_B,
                                           COL_CRYSTAL_EDGE, 0.5f));
        fillpoly(5, innerDia);
      }
    }

    /* Bieu tuong tia set (lightning bolt) */
    setcolor(COL_CRYSTAL_BOLT);
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
        int sx1 = cx + (int)(cosf(a) * (r + 4));
        int sy1 = cy + (int)(sinf(a) * (r + 4));
        int sx2 = cx + (int)(cosf(a) * (r + 8));
        int sy2 = cy + (int)(sinf(a) * (r + 8));
        setcolor((k + it[i].sparkle / 3) % 2 ? COL_CRYSTAL_SPARK_A : COL_CRYSTAL_SPARK_B);
        line(sx1, sy1, sx2, sy2);
      }
    }
  }
}

/* --- Hieu ung no (nhieu lop, gradient mau, manh vo, hat lua) --- */
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

    /* ---- Lop 1: Vong glow ngoai cung ---- */
    if (p < 0.6f)
    {
      setcolor(COL_EXPL_GLOW);
      circle(ex, ey, (int)(r * 1.35f));
      circle(ex, ey, (int)(r * 1.25f));
    }

    /* ---- Lop 2: Vong no chinh (gradient theo thoi gian) ---- */
    {
      int c;
      if (p < 0.15f)
        c = COL_EXPL_1;
      else if (p < 0.3f)
        c = COL_EXPL_2;
      else if (p < 0.5f)
        c = COL_EXPL_3;
      else if (p < 0.7f)
        c = COL_EXPL_4;
      else
        c = COL_EXPL_5;
      setcolor(c);
      circle(ex, ey, (int)r);
    }

    /* ---- Lop 3: Vong giua (inner ring) ---- */
    if (r > 4)
    {
      int c2;
      if (p < 0.25f)
        c2 = COL_EXPL_2;
      else if (p < 0.5f)
        c2 = COL_EXPL_3;
      else
        c2 = COL_EXPL_4;
      setcolor(c2);
      circle(ex, ey, (int)(r * 0.65f));
    }

    /* ---- Lop 4: Loi no (core) ---- */
    if (r > 8)
    {
      int c3;
      if (p < 0.3f)
        c3 = COL_EXPL_1;
      else if (p < 0.5f)
        c3 = COL_EXPL_2;
      else
        c3 = COL_EXPL_3;
      setcolor(c3);
      circle(ex, ey, (int)(r * 0.3f));

      /* Diem sang trung tam */
      if (p < 0.45f)
      {
        setfillstyle(SOLID_FILL, COL_EXPL_1);
        fillellipse(ex, ey, 3, 3);
        setfillstyle(SOLID_FILL, COL_EXPL_SPARK);
        fillellipse(ex, ey, 1, 1);
      }
    }

    /* ---- Lop 5: Manh vo bay ra (debris lines) ---- */
    {
      int k;
      unsigned int ds = (unsigned int)(e[i].x * 100 + e[i].y);
      int numDebris = 8; /* Them manh vo */
      for (k = 0; k < numDebris; k++)
      {
        ds = seedRand(ds, k);
        float da = (float)k / numDebris * 2.0f * (float)M_PI +
                   (float)(ds % 100) / 100.0f;
        float dLen = r * (0.8f + (float)(ds % 40) / 100.0f);
        int dx1 = ex + (int)(cosf(da) * r * 0.35f);
        int dy1 = ey + (int)(sinf(da) * r * 0.35f);
        int dx2 = ex + (int)(cosf(da) * dLen);
        int dy2 = ey + (int)(sinf(da) * dLen);

        /* Mau manh vo thay doi theo thoi gian */
        setcolor((p < 0.4f) ? COL_EXPL_DEBRIS : COL_EXPL_DEBRIS_DIM);
        line(dx1, dy1, dx2, dy2);

        /* Hat lua dau manh vo */
        if (p < 0.55f)
        {
          int sparkCol;
          if (p < 0.2f)
            sparkCol = COL_EXPL_SPARK;
          else if (p < 0.4f)
            sparkCol = COL_EXPL_2;
          else
            sparkCol = COL_EXPL_3;
          setfillstyle(SOLID_FILL, sparkCol);
          fillellipse(dx2, dy2, 1, 1);
        }
      }
    }

    /* ---- Lop 6: Vong khoi (smoke ring) phia ngoai ---- */
    if (p > 0.5f && r > 10)
    {
      float smokeP = (p - 0.5f) / 0.5f; /* 0->1 trong nua cuoi */
      int smokeCol = colorLerp(COL_SMOKE_1, COL_SMOKE_3, smokeP);
      setcolor(smokeCol);
      circle(ex, ey, (int)(r * 1.1f));
    }
  }
}

/* --- Hieu ung skill --- */
void drawSkillEffects(SkillSystem *sk)
{
  /* ---- Bomb Burst: vong tron no voi gradient ---- */
  if (sk->bombActive)
  {
    float p = (float)sk->bombFrame / 22.0f;
    float r = sk->bombRadius * p;
    int c;
    if (p < 0.25f)
      c = COL_BOMB_HOT;
    else if (p < 0.55f)
      c = COL_BOMB_MID;
    else
      c = COL_BOMB_COOL;

    /* Vong glow ngoai */
    setcolor(COL_BOMB_RING);
    circle((int)sk->bombX, (int)sk->bombY, (int)(r * 1.1f));

    /* Vong chinh */
    setcolor(c);
    circle((int)sk->bombX, (int)sk->bombY, (int)r);

    /* Vong giua */
    if (r > 15)
    {
      setcolor(colorLerp(c, COL_BOMB_HOT, 0.4f));
      circle((int)sk->bombX, (int)sk->bombY, (int)(r * 0.7f));
    }

    /* Vong loi */
    if (r > 30)
    {
      setcolor(COL_BOMB_HOT);
      circle((int)sk->bombX, (int)sk->bombY, (int)(r * 0.35f));
    }
  }

  /* ---- Piercing Beam: tia sang voi Dragon Curve ---- */
  if (sk->beamActive)
  {
    float endX = sk->beamSX + sk->beamDX * 1500;
    float endY = sk->beamSY + sk->beamDY * 1500;

    /* Lop glow ngoai voi Dragon Curve */
    drawDragonCurve((int)sk->beamSX, (int)sk->beamSY, (int)endX, (int)endY, 
                    5, 1, COL_BEAM_GLOW);

    /* Lop chinh */
    setcolor(COL_BEAM_OUTER);
    setlinestyle(SOLID_LINE, 0, 3);
    line((int)sk->beamSX, (int)sk->beamSY, (int)endX, (int)endY);

    /* Lop loi sang */
    setcolor(COL_BEAM_INNER);
    setlinestyle(SOLID_LINE, 0, 1);
    line((int)sk->beamSX, (int)sk->beamSY, (int)endX, (int)endY);
  }

  /* ---- Time Slow indicator ---- */
  if (sk->slowActive)
  {
    setcolor(COL_SLOW_TEXT);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(WIDTH / 2 - 70, HEIGHT - 40,
              (char *)"~ SLOW ACTIVE ~");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
  }
}

#endif
