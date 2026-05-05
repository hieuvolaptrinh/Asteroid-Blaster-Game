#ifndef DRAW_SHIP_H
#define DRAW_SHIP_H

#include "mid_point.h"
#include "fill_color.h"

/* ====================================================================== */
/*                  SHIP DRAWING FUNCTIONS (Enhanced)                      */
/*  Su dung midPointLine, midPointCircle, midPointEllipse tu mid_point.h  */
/*  Su dung scanline fill tu fill_color.h de to mau nhanh (real-time)     */
/* ====================================================================== */

/* ------------------------------------------------------------------ */
/*  Helper: Ve va to hinh tron bang scanline fill + midpoint outline   */
/*  Scanline: ve duong ngang doi xung, khong can getpixel()           */
/* ------------------------------------------------------------------ */
static void midPointFilledCircle(int xc, int yc, int radius, int outlineColor,
                                 int fillColor)
{
  if (radius <= 0) return;
  /* Buoc 1: To mau bang scanline (nhanh) */
  scanlineFillCircle(xc, yc, radius, fillColor);
  /* Buoc 2: Ve vien bang midpoint algorithm */
  midPointCircle(xc, yc, radius, outlineColor);
}

/* ------------------------------------------------------------------ */
/*  Helper: Ve va to ellipse bang scanline fill + midpoint outline     */
/* ------------------------------------------------------------------ */
static void midPointFilledEllipse(int xc, int yc, int rx, int ry,
                                  int outlineColor, int fillColor)
{
  if (rx <= 0 || ry <= 0) return;
  /* Buoc 1: To mau bang scanline */
  scanlineFillEllipse(xc, yc, rx, ry, fillColor);
  /* Buoc 2: Ve vien bang midpoint algorithm */
  midPointEllipse(xc, yc, rx, ry, outlineColor);
}

/* ------------------------------------------------------------------ */
/*  Helper: Ve polygon kin bang scanline triangle fill + midpoint edge */
/*  xy format: [x0,y0, x1,y1, ...] (nPoints dinh)                    */
/*  Toi uu cho tam giac (nPoints == 3) – dung scanline rasterization  */
/* ------------------------------------------------------------------ */
static void midPointFilledPoly(const int *xy, int nPoints, int outlineColor,
                               int fillColor)
{
  if (!xy || nPoints < 3) return;

  if (nPoints == 3)
  {
    /* Tam giac: dung scanline fill (cuc nhanh) */
    scanlineFillTriangle(xy[0], xy[1], xy[2], xy[3], xy[4], xy[5], fillColor);
    /* Ve vien bang midpoint line */
    midPointPolyline(xy, nPoints, outlineColor, 1);
  }
  else
  {
    /* Polygon chung: ve vien midpoint + dung floodfill BGI */
    int i, seedX = 0, seedY = 0;
    midPointPolyline(xy, nPoints, outlineColor, 1);
    for (i = 0; i < nPoints; i++)
    {
      seedX += xy[i * 2];
      seedY += xy[i * 2 + 1];
    }
    seedX /= nPoints;
    seedY /= nPoints;
    setfillstyle(SOLID_FILL, fillColor);
    floodfill(seedX, seedY, outlineColor);
  }
}

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
    int shadow[] = {snx, sny, slx, sly, srx, sry};
    midPointFilledPoly(shadow, 3, COL_SHIP_SHADOW, COL_SHADOW_GRAD_0);
  }

  /* ---- Than chinh: lop dark base ---- */
  {
    int body[] = {nx, ny, lx, ly, rx, ry};
    midPointFilledPoly(body, 3, COL_SHIP_GRAD_0, COL_SHIP_BODY);
  }

  /* ---- Highlight lop 1 – vung sang chinh ---- */
  {
    float hs = 0.7f;
    int hnx = (int)(s.x + cosf(angle) * (s.radius + 16) * 0.95f);
    int hny = (int)(s.y + sinf(angle) * (s.radius + 16) * 0.95f);
    int hlx = (int)(s.x + cosf(angle + 2.3f) * s.radius * hs);
    int hly = (int)(s.y + sinf(angle + 2.3f) * s.radius * hs);
    int hrx = (int)(s.x + cosf(angle - 2.3f) * s.radius * hs);
    int hry = (int)(s.y + sinf(angle - 2.3f) * s.radius * hs);
    int hl[] = {hnx, hny, hlx, hly, hrx, hry};
    midPointFilledPoly(hl, 3, COL_SHIP_GRAD_2, COL_SHIP_HL1);
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
    int h2[] = {h2nx, h2ny, h2lx, h2ly, h2rx, h2ry};
    midPointFilledPoly(h2, 3, COL_SHIP_GRAD_3, COL_SHIP_HL2);
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
    int h3[] = {h3nx, h3ny, h3lx, h3ly, h3rx, h3ry};
    midPointFilledPoly(h3, 3, COL_SHIP_GRAD_4, COL_SHIP_HL3);
  }

  /* ---- Vien sang (edge highlights) bang midPointLine ---- */
  midPointLine(nx, ny, lx, ly, COL_SHIP_EDGE2);
  midPointLine(nx, ny, rx, ry, COL_SHIP_EDGE2);
  midPointLine(lx, ly, rx, ry, COL_SHIP_EDGE);

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
      int swL[] = {sw1x, sw1y, sw2x, sw2y, sw3x, sw3y};
      midPointFilledPoly(swL, 3, COL_SHIP_SHADOW, COL_SHADOW_GRAD_0);
    }

    /* Than canh */
    {
      int wL[] = {w1x, w1y, w2x, w2y, w3x, w3y};
      midPointFilledPoly(wL, 3, COL_WING_GRAD_0, COL_SHIP_WING);
    }

    /* Highlight canh */
    {
      int hw1x = (int)(s.x + cosf(angle + 2.05f) * s.radius * 0.6f);
      int hw1y = (int)(s.y + sinf(angle + 2.05f) * s.radius * 0.6f);
      int hw2x = (int)(s.x + cosf(angle + 1.75f) * (s.radius + 8));
      int hw2y = (int)(s.y + sinf(angle + 1.75f) * (s.radius + 8));
      int hw3x = (int)(s.x + cosf(angle + 2.7f) * s.radius * 0.9f);
      int hw3y = (int)(s.y + sinf(angle + 2.7f) * s.radius * 0.9f);
      int hwL[] = {hw1x, hw1y, hw2x, hw2y, hw3x, hw3y};
      midPointFilledPoly(hwL, 3, COL_WING_GRAD_3, COL_SHIP_WING_HL);
    }
    midPointLine(w1x, w1y, w2x, w2y, COL_SHIP_EDGE);
    midPointLine(w2x, w2y, w3x, w3y, COL_SHIP_EDGE);
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
      int swR[] = {sw1x, sw1y, sw2x, sw2y, sw3x, sw3y};
      midPointFilledPoly(swR, 3, COL_SHIP_SHADOW, COL_SHADOW_GRAD_0);
    }

    /* Than canh */
    {
      int wR[] = {w1x, w1y, w2x, w2y, w3x, w3y};
      midPointFilledPoly(wR, 3, COL_WING_GRAD_0, COL_SHIP_WING);
    }

    /* Highlight canh */
    {
      int hw1x = (int)(s.x + cosf(angle - 2.05f) * s.radius * 0.6f);
      int hw1y = (int)(s.y + sinf(angle - 2.05f) * s.radius * 0.6f);
      int hw2x = (int)(s.x + cosf(angle - 1.75f) * (s.radius + 8));
      int hw2y = (int)(s.y + sinf(angle - 1.75f) * (s.radius + 8));
      int hw3x = (int)(s.x + cosf(angle - 2.7f) * s.radius * 0.9f);
      int hw3y = (int)(s.y + sinf(angle - 2.7f) * s.radius * 0.9f);
      int hwR[] = {hw1x, hw1y, hw2x, hw2y, hw3x, hw3y};
      midPointFilledPoly(hwR, 3, COL_WING_GRAD_3, COL_SHIP_WING_HL);
    }
    midPointLine(w1x, w1y, w2x, w2y, COL_SHIP_EDGE);
    midPointLine(w2x, w2y, w3x, w3y, COL_SHIP_EDGE);
  }

  /* ---- Kinh lai (cockpit) voi nhieu lop gradient ---- */
  {
    int cx = (int)(s.x + cosf(angle) * s.radius * 0.35f);
    int cy = (int)(s.y + sinf(angle) * s.radius * 0.35f);

    /* Vien ngoai cockpit – gradient ring */
    midPointFilledEllipse(cx, cy, 7, 6, COL_COCKPIT_GRAD_0, COL_SHIP_HL1);

    /* Glass cockpit – gradient tu xanh -> sang */
    midPointFilledEllipse(cx, cy, 6, 5, COL_COCKPIT_GRAD_1, COL_SHIP_COCKPIT);

    /* Anh sang phan chieu – diem sang nho */
    midPointFilledEllipse(cx - 2, cy - 2, 2, 2, COL_COCKPIT_GRAD_3,
                          COL_SHIP_COCKPIT_HL);

    /* Vien cockpit bang midPointEllipse */
    midPointEllipse(cx, cy, 7, 6, COL_SHIP_EDGE);
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
      int fg[] = {g1x, g1y, gtx, gty, g2x, g2y};
      midPointFilledPoly(fg, 3, COL_FLAME_GRAD_7, COL_FLAME_SPARK);
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
      int fl[] = {e1x, e1y, tx, ty, e2x, e2y};
      midPointFilledPoly(fl, 3, COL_FLAME_GRAD_8, outerCol);
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
      int fi[] = {i1x, i1y, itx, ity, i2x, i2y};
      midPointFilledPoly(fi, 3, COL_FLAME_GRAD_4, COL_FLAME_MID);
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
      int fc[] = {c1x, c1y, ctx, cty, c2x, c2y};
      midPointFilledPoly(fc, 3, COL_FLAME_GRAD_1, COL_FLAME_CORE);
    }
  }
}

/* drawShipShieldAura(): Ve hao quang bao ve quanh tau voi gradient dong tam. */
static void drawShipShieldAura(Ship s, int frame)
{
  int auraR = s.radius + 8 + (frame % 8) / 2;
  int sx = (int)s.x, sy = (int)s.y;

  /* Vong ngoai cung – gradient mo */
  midPointCircle(sx, sy, auraR + 6, COL_AURA_GRAD_0);
  midPointCircle(sx, sy, auraR + 4, COL_AURA_GRAD_1);

  /* Vong chinh – nhap nhay giua 2 mau */
  {
    int mainCol = ((frame / 4) % 2) ? COL_AURA_GRAD_3 : COL_AURA_GRAD_2;
    midPointCircle(sx, sy, auraR, mainCol);
  }

  /* Vong trong – sang hon */
  midPointCircle(sx, sy, auraR - 2, COL_AURA_GRAD_4);

  /* Vong sat nhat – glow effect */
  midPointCircle(sx, sy, auraR - 4, COL_AURA_GRAD_5);
}

/* drawShip(): Ve toan bo phi thuyen (aura, lua, than, duong ngam, crosshair). */
void drawShip(Ship s, int mx, int my, int frame)
{
  float angle = atan2f((float)(my - s.y), (float)(mx - s.x));

  drawShipShieldAura(s, frame);
  drawShipEngineFlame(s, angle, frame);
  drawShipBody(s, angle);

  /* ---- Duong ngam bang midPointLine ---- */
  {
    float dx = (float)(mx - s.x), dy = (float)(my - s.y);
    float ln = sqrtf(dx * dx + dy * dy);
    if (ln > 1.0f)
    {
      int ax = (int)(s.x + dx / ln * 80);
      int ay = (int)(s.y + dy / ln * 80);

      /* Ve duong ngam bang cac doan ngan (mo phong DASHED_LINE) */
      {
        int i, nSegs = 8;
        float segLen = 80.0f / (float)(nSegs * 2);
        for (i = 0; i < nSegs; i++)
        {
          float t0 = (float)(i * 2) * segLen;
          float t1 = (float)(i * 2 + 1) * segLen;
          int x0 = (int)(s.x + dx / ln * t0);
          int y0 = (int)(s.y + dy / ln * t0);
          int x1 = (int)(s.x + dx / ln * t1);
          int y1 = (int)(s.y + dy / ln * t1);
          midPointLine(x0, y0, x1, y1, COL_AIM_LINE);
        }
      }
    }
  }

  /* ---- Crosshair (Tam ngam) voi gradient dong tam ---- */
  {
    int cr = 10 + (frame % 6) / 3;

    /* Cac vong tron dong tam – tu ngoai vao trong sang dan */
    midPointCircle(mx, my, cr + 6, COL_CROSS_GRAD_0);
    midPointCircle(mx, my, cr + 4, COL_CROSS_GRAD_1);
    midPointCircle(mx, my, cr + 2, COL_CROSS_OUTER);
    midPointCircle(mx, my, cr, COL_CROSS_INNER);

    /* 4 thanh ngang/doc bang midPointLineThick cho ro net */
    midPointLineThick(mx - 14, my, mx - 5, my, 2, COL_CROSS_INNER);
    midPointLineThick(mx + 5, my, mx + 14, my, 2, COL_CROSS_INNER);
    midPointLineThick(mx, my - 14, mx, my - 5, 2, COL_CROSS_INNER);
    midPointLineThick(mx, my + 5, mx, my + 14, 2, COL_CROSS_INNER);

    /* Tam diem trung tam bang filled circle */
    midPointFilledCircle(mx, my, 2, COL_CROSS_DOT, COL_CROSS_DOT);
  }
}

#endif
