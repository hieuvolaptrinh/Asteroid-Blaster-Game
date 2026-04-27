#ifndef MID_POINT_H
#define MID_POINT_H

#include <graphics.h>
#include <math.h>
#include <stdlib.h>

/* ====================================================================== */
/*                        MIDPOINT DRAW ALGORITHMS                        */
/* ====================================================================== */

static void mpPutPixelSafe(int x, int y, int color)
{
    if (x < 0 || y < 0 || x > getmaxx() || y > getmaxy())
        return;
    putpixel(x, y, color);
}

static void midPointLineShallow(int x1, int y1, int x2, int y2, int sx, int sy,
                                int color)
{
    int x = x1;
    int y = y1;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int d = 2 * dy - dx;
    int step;

    mpPutPixelSafe(x, y, color);

    for (step = 0; step < dx; step++)
    {
        x += sx;
        if (d > 0)
        {
            y += sy;
            d += 2 * (dy - dx);
        }
        else
        {
            d += 2 * dy;
        }
        mpPutPixelSafe(x, y, color);
    }
}

static void midPointLineSteep(int x1, int y1, int x2, int y2, int sx, int sy,
                              int color)
{
    int x = x1;
    int y = y1;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int d = 2 * dx - dy;
    int step;

    mpPutPixelSafe(x, y, color);

    for (step = 0; step < dy; step++)
    {
        y += sy;
        if (d > 0)
        {
            x += sx;
            d += 2 * (dx - dy);
        }
        else
        {
            d += 2 * dx;
        }
        mpPutPixelSafe(x, y, color);
    }
}

/*
 * midPointLine(): Midpoint/Bresenham line cho day du 8 truong hop octant.
 * Co ghi chu tung truong hop de de hoc va map sang huong do thi.
 */
void midPointLine(int x1, int y1, int x2, int y2, int color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int adx = abs(dx);
    int ady = abs(dy);
    int sx = (dx >= 0) ? 1 : -1;
    int sy = (dy >= 0) ? 1 : -1;
    int octant;

    if (dx >= 0 && dy >= 0)
        octant = (adx >= ady) ? 1 : 2;
    else if (dx < 0 && dy >= 0)
        octant = (adx < ady) ? 3 : 4;
    else if (dx < 0 && dy < 0)
        octant = (adx >= ady) ? 5 : 6;
    else
        octant = (adx < ady) ? 7 : 8;

    switch (octant)
    {
    /* Case 1: 0 <= m <= 1, ve sang phai va di len nhe */
    case 1:
        midPointLineShallow(x1, y1, x2, y2, sx, sy, color);
        break;

    /* Case 2: m > 1, ve sang phai va di len doc */
    case 2:
        midPointLineSteep(x1, y1, x2, y2, sx, sy, color);
        break;

    /* Case 3: m < -1 (huong trai-len), buoc chinh theo truc Y */
    case 3:
        midPointLineSteep(x1, y1, x2, y2, sx, sy, color);
        break;

    /* Case 4: -1 <= m < 0 (huong trai-len), buoc chinh theo truc X */
    case 4:
        midPointLineShallow(x1, y1, x2, y2, sx, sy, color);
        break;

    /* Case 5: 0 <= m <= 1 (huong trai-xuong), doi xung voi case 1 */
    case 5:
        midPointLineShallow(x1, y1, x2, y2, sx, sy, color);
        break;

    /* Case 6: m > 1 (huong trai-xuong), doi xung voi case 2 */
    case 6:
        midPointLineSteep(x1, y1, x2, y2, sx, sy, color);
        break;

    /* Case 7: m < -1 (huong phai-xuong), doi xung voi case 3 */
    case 7:
        midPointLineSteep(x1, y1, x2, y2, sx, sy, color);
        break;

    /* Case 8: -1 <= m < 0 (huong phai-xuong), doi xung voi case 4 */
    case 8:
        midPointLineShallow(x1, y1, x2, y2, sx, sy, color);
        break;
    }
}

void midPointLineThick(int x1, int y1, int x2, int y2, int thickness,
                       int color)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int i;

    if (thickness <= 1)
    {
        midPointLine(x1, y1, x2, y2, color);
        return;
    }

    /* Day line bang cach ve cac line song song theo truc vuong goc chu dao. */
    if (dx >= dy)
    {
        int half = thickness / 2;
        for (i = -half; i <= half; i++)
            midPointLine(x1, y1 + i, x2, y2 + i, color);
    }
    else
    {
        int half = thickness / 2;
        for (i = -half; i <= half; i++)
            midPointLine(x1 + i, y1, x2 + i, y2, color);
    }
}

static void midPointPlotCircle8(int xc, int yc, int x, int y, int color)
{
    mpPutPixelSafe(xc + x, yc + y, color);
    mpPutPixelSafe(xc - x, yc + y, color);
    mpPutPixelSafe(xc + x, yc - y, color);
    mpPutPixelSafe(xc - x, yc - y, color);
    mpPutPixelSafe(xc + y, yc + x, color);
    mpPutPixelSafe(xc - y, yc + x, color);
    mpPutPixelSafe(xc + y, yc - x, color);
    mpPutPixelSafe(xc - y, yc - x, color);
}

void midPointCircle(int xc, int yc, int radius, int color)
{
    int x = 0;
    int y = radius;
    int d = 1 - radius;

    if (radius < 0)
        return;

    midPointPlotCircle8(xc, yc, x, y, color);

    while (x < y)
    {
        x++;
        if (d < 0)
        {
            d += 2 * x + 1;
        }
        else
        {
            y--;
            d += 2 * (x - y) + 1;
        }
        midPointPlotCircle8(xc, yc, x, y, color);
    }
}

static void midPointPlotEllipse4(int xc, int yc, int x, int y, int color)
{
    mpPutPixelSafe(xc + x, yc + y, color);
    mpPutPixelSafe(xc - x, yc + y, color);
    mpPutPixelSafe(xc + x, yc - y, color);
    mpPutPixelSafe(xc - x, yc - y, color);
}

void midPointEllipse(int xc, int yc, int rx, int ry, int color)
{
    long long rx2;
    long long ry2;
    long long twoRx2;
    long long twoRy2;
    long long x;
    long long y;
    long long px;
    long long py;
    long long p;

    if (rx < 0 || ry < 0)
        return;

    if (rx == 0 && ry == 0)
    {
        mpPutPixelSafe(xc, yc, color);
        return;
    }

    rx2 = (long long)rx * (long long)rx;
    ry2 = (long long)ry * (long long)ry;
    twoRx2 = 2 * rx2;
    twoRy2 = 2 * ry2;

    x = 0;
    y = ry;
    px = 0;
    py = twoRx2 * y;

    /* Region 1: do doc |m| <= 1 */
    p = ry2 - rx2 * ry + rx2 / 4;
    while (px < py)
    {
        midPointPlotEllipse4(xc, yc, (int)x, (int)y, color);
        x++;
        px += twoRy2;
        if (p < 0)
        {
            p += ry2 + px;
        }
        else
        {
            y--;
            py -= twoRx2;
            p += ry2 + px - py;
        }
    }

    /* Region 2: do doc |m| > 1 */
    p = ry2 * (x + 1) * (x + 1) + rx2 * (y - 1) * (y - 1) - rx2 * ry2;
    while (y >= 0)
    {
        midPointPlotEllipse4(xc, yc, (int)x, (int)y, color);
        y--;
        py -= twoRx2;
        if (p > 0)
        {
            p += rx2 - py;
        }
        else
        {
            x++;
            px += twoRy2;
            p += rx2 - py + px;
        }
    }
}

/* xy format: [x0, y0, x1, y1, ...] */
void midPointPolyline(const int *xy, int nPoints, int color, int closed)
{
    int i;
    if (!xy || nPoints < 2)
        return;

    for (i = 0; i < nPoints - 1; i++)
    {
        int x1 = xy[i * 2];
        int y1 = xy[i * 2 + 1];
        int x2 = xy[(i + 1) * 2];
        int y2 = xy[(i + 1) * 2 + 1];
        midPointLine(x1, y1, x2, y2, color);
    }

    if (closed)
    {
        int x1 = xy[(nPoints - 1) * 2];
        int y1 = xy[(nPoints - 1) * 2 + 1];
        int x2 = xy[0];
        int y2 = xy[1];
        midPointLine(x1, y1, x2, y2, color);
    }
}

#endif
