#ifndef FILL_COLOR_H
#define FILL_COLOR_H

#include <graphics.h>
#include <stdlib.h>
#include <math.h>

/* ====================================================================== */
/*     FILL COLOR UTILITIES                                               */
/*     Bao gom: Scanline fill (nhanh) + Boundary fill (giao duc)         */
/* ====================================================================== */

typedef struct
{
    int minX, minY;
    int maxX, maxY;
    int maxCalls;
    int maxDepth;
    int useEightWay;
} RecursiveFillConfig;

typedef struct
{
    int calls;
    int aborted;
} RecursiveFillResult;

/* Default config */
RecursiveFillConfig makeDefaultRecursiveFillConfig(void)
{
    RecursiveFillConfig cfg;
    int w = getmaxx();
    int h = getmaxy();
    cfg.minX = 0;
    cfg.minY = 0;
    cfg.maxX = w;
    cfg.maxY = h;
    cfg.maxCalls = 30000;
    cfg.maxDepth = 30000;
    cfg.useEightWay = 0;
    return cfg;
}

/* ====================================================================== */
/*  PHAN 1: SCANLINE FILL – Hieu suat cao, dung cho real-time rendering  */
/*  Khong can getpixel(), chi dung line() de to tung dong ngang.         */
/* ====================================================================== */

/*
 * scanlineFillCircle():
 * To hinh tron bang midpoint algorithm + horizontal scanlines.
 * Thay vi to tung pixel, ve cac duong ngang doi xung.
 * Do phuc tap: O(radius) thay vi O(radius^2).
 */
static void scanlineFillCircle(int xc, int yc, int radius, int fillColor)
{
    int x, y, d;
    if (radius <= 0)
    {
        putpixel(xc, yc, fillColor);
        return;
    }

    x = 0;
    y = radius;
    d = 1 - radius;

    setcolor(fillColor);
    while (x <= y)
    {
        /* Ve 4 cap duong ngang doi xung */
        line(xc - y, yc - x, xc + y, yc - x);
        line(xc - y, yc + x, xc + y, yc + x);
        if (x != y)
        {
            line(xc - x, yc - y, xc + x, yc - y);
            line(xc - x, yc + y, xc + x, yc + y);
        }

        if (d < 0)
            d += 2 * x + 3;
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

/*
 * scanlineFillEllipse():
 * To ellipse bang cach tinh x theo phuong trinh ellipse cho tung y.
 * Do phuc tap: O(ry) duong ngang.
 */
static void scanlineFillEllipse(int xc, int yc, int rx, int ry, int fillColor)
{
    int dy;
    if (rx <= 0 || ry <= 0)
    {
        putpixel(xc, yc, fillColor);
        return;
    }

    setcolor(fillColor);
    for (dy = -ry; dy <= ry; dy++)
    {
        float t = 1.0f - (float)(dy * dy) / (float)(ry * ry);
        int dx;
        if (t < 0.0f)
            t = 0.0f;
        dx = (int)(rx * sqrtf(t));
        line(xc - dx, yc + dy, xc + dx, yc + dy);
    }
}

/*
 * scanlineFillTriangle():
 * To tam giac bang thuat toan scanline – sap xep 3 dinh theo Y,
 * noi suy X tren 2 canh, ve duong ngang tu left->right.
 * Do phuc tap: O(chieu cao tam giac) duong ngang.
 */
static void scanlineFillTriangle(int x0, int y0, int x1, int y1,
                                 int x2, int y2, int fillColor)
{
    int tmpx, tmpy, totalH, y;

    /* Sap xep 3 dinh theo Y tang dan */
    if (y0 > y1) { tmpx = x0; tmpy = y0; x0 = x1; y0 = y1; x1 = tmpx; y1 = tmpy; }
    if (y0 > y2) { tmpx = x0; tmpy = y0; x0 = x2; y0 = y2; x2 = tmpx; y2 = tmpy; }
    if (y1 > y2) { tmpx = x1; tmpy = y1; x1 = x2; y1 = y2; x2 = tmpx; y2 = tmpy; }

    totalH = y2 - y0;
    if (totalH == 0)
    {
        /* Tam giac suy bien (3 dinh thang hang) */
        int minX = x0 < x1 ? (x0 < x2 ? x0 : x2) : (x1 < x2 ? x1 : x2);
        int maxX = x0 > x1 ? (x0 > x2 ? x0 : x2) : (x1 > x2 ? x1 : x2);
        setcolor(fillColor);
        line(minX, y0, maxX, y0);
        return;
    }

    setcolor(fillColor);
    for (y = y0; y <= y2; y++)
    {
        /* Canh dai: luon tu v0 den v2 */
        int xa = x0 + (x2 - x0) * (y - y0) / totalH;
        int xb;

        /* Canh ngan: v0->v1 (nua tren) hoac v1->v2 (nua duoi) */
        if (y <= y1)
        {
            int segH = y1 - y0;
            xb = (segH > 0) ? x0 + (x1 - x0) * (y - y0) / segH : x0;
        }
        else
        {
            int segH = y2 - y1;
            xb = (segH > 0) ? x1 + (x2 - x1) * (y - y1) / segH : x1;
        }

        if (xa > xb)
        {
            int t = xa;
            xa = xb;
            xb = t;
        }
        line(xa, y, xb, y);
    }
}

/* ====================================================================== */
/*  PHAN 2: BOUNDARY FILL (Iterative) – Giu lai cho muc dich giao duc   */
/*  Dung stack tren heap thay vi de quy de tranh stack overflow.         */
/* ====================================================================== */

typedef struct
{
    int *data;
    int top;
    int capacity;
} FillStack;

static FillStack fsCreate(int maxPairs)
{
    FillStack s;
    s.capacity = maxPairs * 2;
    s.data = (int *)malloc(sizeof(int) * s.capacity);
    s.top = 0;
    return s;
}

static void fsDestroy(FillStack *s)
{
    if (s->data)
    {
        free(s->data);
        s->data = NULL;
    }
    s->top = 0;
    s->capacity = 0;
}

static int fsEmpty(const FillStack *s) { return s->top == 0; }

static void fsPush(FillStack *s, int x, int y)
{
    if (s->top + 2 <= s->capacity)
    {
        s->data[s->top++] = x;
        s->data[s->top++] = y;
    }
}

static void fsPop(FillStack *s, int *x, int *y)
{
    *y = s->data[--s->top];
    *x = s->data[--s->top];
}

static int rfInBounds(int x, int y, const RecursiveFillConfig *cfg)
{
    return (x >= cfg->minX && x <= cfg->maxX && y >= cfg->minY &&
            y <= cfg->maxY)
               ? 1
               : 0;
}

static int rfIsBoundaryColor(int pxColor, int boundaryColorA, int boundaryColorB,
                             int hasSecondBoundary)
{
    if (pxColor == boundaryColorA)
        return 1;
    if (hasSecondBoundary && pxColor == boundaryColorB)
        return 1;
    return 0;
}

static void iterativeBoundaryFill(int seedX, int seedY, int fillColor,
                                  int boundaryColorA, int boundaryColorB,
                                  int hasSecondBoundary, int eightWay,
                                  const RecursiveFillConfig *cfg,
                                  RecursiveFillResult *res)
{
    int maxPairs = cfg->maxCalls;
    FillStack stk;
    int x, y, cur;

    if (maxPairs > 30000)
        maxPairs = 30000;

    stk = fsCreate(maxPairs);

    res->calls = 0;
    res->aborted = 0;

    if (!rfInBounds(seedX, seedY, cfg))
    {
        fsDestroy(&stk);
        return;
    }
    cur = getpixel(seedX, seedY);
    if (cur == fillColor ||
        rfIsBoundaryColor(cur, boundaryColorA, boundaryColorB, hasSecondBoundary))
    {
        fsDestroy(&stk);
        return;
    }

    fsPush(&stk, seedX, seedY);

    while (!fsEmpty(&stk))
    {
        fsPop(&stk, &x, &y);

        if (!rfInBounds(x, y, cfg))
            continue;

        cur = getpixel(x, y);
        if (cur == fillColor)
            continue;
        if (rfIsBoundaryColor(cur, boundaryColorA, boundaryColorB,
                              hasSecondBoundary))
            continue;

        putpixel(x, y, fillColor);
        res->calls++;

        if (res->calls >= cfg->maxCalls)
        {
            res->aborted = 1;
            break;
        }

        fsPush(&stk, x - 1, y);
        fsPush(&stk, x + 1, y);
        fsPush(&stk, x, y - 1);
        fsPush(&stk, x, y + 1);

        if (eightWay)
        {
            fsPush(&stk, x - 1, y - 1);
            fsPush(&stk, x + 1, y - 1);
            fsPush(&stk, x - 1, y + 1);
            fsPush(&stk, x + 1, y + 1);
        }
    }

    fsDestroy(&stk);
}

RecursiveFillResult boundaryFillRecursive4(int seedX, int seedY, int fillColor,
                                           int boundaryColor,
                                           const RecursiveFillConfig *config)
{
    RecursiveFillConfig localCfg;
    const RecursiveFillConfig *cfg;
    RecursiveFillResult res;

    if (config)
        cfg = config;
    else
    {
        localCfg = makeDefaultRecursiveFillConfig();
        localCfg.useEightWay = 0;
        cfg = &localCfg;
    }

    res.calls = 0;
    res.aborted = 0;
    iterativeBoundaryFill(seedX, seedY, fillColor, boundaryColor, 0, 0,
                          0, cfg, &res);
    return res;
}

RecursiveFillResult boundaryFillRecursive8(int seedX, int seedY, int fillColor,
                                           int boundaryColor,
                                           const RecursiveFillConfig *config)
{
    RecursiveFillConfig localCfg;
    const RecursiveFillConfig *cfg;
    RecursiveFillResult res;

    if (config)
    {
        localCfg = *config;
        localCfg.useEightWay = 1;
        cfg = &localCfg;
    }
    else
    {
        localCfg = makeDefaultRecursiveFillConfig();
        localCfg.useEightWay = 1;
        cfg = &localCfg;
    }

    res.calls = 0;
    res.aborted = 0;
    iterativeBoundaryFill(seedX, seedY, fillColor, boundaryColor, 0, 0,
                          1, cfg, &res);
    return res;
}

RecursiveFillResult boundaryFillRecursiveDualBoundary(
    int seedX, int seedY, int fillColor, int boundaryColorA, int boundaryColorB,
    const RecursiveFillConfig *config)
{
    RecursiveFillConfig localCfg;
    const RecursiveFillConfig *cfg;
    RecursiveFillResult res;

    if (config)
        cfg = config;
    else
    {
        localCfg = makeDefaultRecursiveFillConfig();
        cfg = &localCfg;
    }

    res.calls = 0;
    res.aborted = 0;
    iterativeBoundaryFill(seedX, seedY, fillColor, boundaryColorA,
                          boundaryColorB, 1, cfg->useEightWay, cfg, &res);
    return res;
}

#endif
