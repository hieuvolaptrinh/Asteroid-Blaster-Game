#ifndef FILL_COLOR_H
#define FILL_COLOR_H

#include <graphics.h>

/* ====================================================================== */
/*              RECURSIVE BOUNDARY-FILL / FLOOD-FILL HELPERS             */
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

/* Default config theo kich thuoc man hinh hien tai. */
RecursiveFillConfig makeDefaultRecursiveFillConfig(void)
{
    RecursiveFillConfig cfg;
    int w = getmaxx();
    int h = getmaxy();
    int area = (w + 1) * (h + 1);

    cfg.minX = 0;
    cfg.minY = 0;
    cfg.maxX = w;
    cfg.maxY = h;
    cfg.maxCalls = area;
    cfg.maxDepth = area;
    cfg.useEightWay = 0;
    return cfg;
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

static void recursiveBoundaryFillInternal(int x, int y, int fillColor,
                                          int boundaryColorA,
                                          int boundaryColorB,
                                          int hasSecondBoundary,
                                          const RecursiveFillConfig *cfg,
                                          RecursiveFillResult *res, int depth)
{
    int cur;

    if (res->aborted)
        return;

    if (depth > cfg->maxDepth || res->calls >= cfg->maxCalls)
    {
        res->aborted = 1;
        return;
    }

    if (!rfInBounds(x, y, cfg))
        return;

    cur = getpixel(x, y);
    if (cur == fillColor)
        return;
    if (rfIsBoundaryColor(cur, boundaryColorA, boundaryColorB, hasSecondBoundary))
        return;

    putpixel(x, y, fillColor);
    res->calls++;

    /* 4-way adjacency: trai, phai, tren, duoi */
    recursiveBoundaryFillInternal(x - 1, y, fillColor, boundaryColorA,
                                  boundaryColorB, hasSecondBoundary, cfg, res,
                                  depth + 1);
    recursiveBoundaryFillInternal(x + 1, y, fillColor, boundaryColorA,
                                  boundaryColorB, hasSecondBoundary, cfg, res,
                                  depth + 1);
    recursiveBoundaryFillInternal(x, y - 1, fillColor, boundaryColorA,
                                  boundaryColorB, hasSecondBoundary, cfg, res,
                                  depth + 1);
    recursiveBoundaryFillInternal(x, y + 1, fillColor, boundaryColorA,
                                  boundaryColorB, hasSecondBoundary, cfg, res,
                                  depth + 1);

    if (!cfg->useEightWay)
        return;

    /* 8-way adjacency: them 4 duong cheo cho vung co canh xien */
    recursiveBoundaryFillInternal(x - 1, y - 1, fillColor, boundaryColorA,
                                  boundaryColorB, hasSecondBoundary, cfg, res,
                                  depth + 1);
    recursiveBoundaryFillInternal(x + 1, y - 1, fillColor, boundaryColorA,
                                  boundaryColorB, hasSecondBoundary, cfg, res,
                                  depth + 1);
    recursiveBoundaryFillInternal(x - 1, y + 1, fillColor, boundaryColorA,
                                  boundaryColorB, hasSecondBoundary, cfg, res,
                                  depth + 1);
    recursiveBoundaryFillInternal(x + 1, y + 1, fillColor, boundaryColorA,
                                  boundaryColorB, hasSecondBoundary, cfg, res,
                                  depth + 1);
}

/*
 * boundaryFillRecursive4():
 * To mau de quy theo bien (4 huong), chinh xac cho vung dong co vien ro rang.
 */
RecursiveFillResult boundaryFillRecursive4(int seedX, int seedY, int fillColor,
                                           int boundaryColor,
                                           const RecursiveFillConfig *config)
{
    RecursiveFillConfig localCfg;
    const RecursiveFillConfig *cfg;
    RecursiveFillResult res;

    if (config)
    {
        cfg = config;
    }
    else
    {
        localCfg = makeDefaultRecursiveFillConfig();
        localCfg.useEightWay = 0;
        cfg = &localCfg;
    }

    res.calls = 0;
    res.aborted = 0;

    recursiveBoundaryFillInternal(seedX, seedY, fillColor, boundaryColor, 0, 0,
                                  cfg, &res, 0);
    return res;
}

/*
 * boundaryFillRecursive8():
 * Ban 8 huong de bao sat bien xien/cheo tot hon.
 */
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

    recursiveBoundaryFillInternal(seedX, seedY, fillColor, boundaryColor, 0, 0,
                                  cfg, &res, 0);
    return res;
}

/*
 * boundaryFillRecursiveDualBoundary():
 * Ho tro 2 mau bien (vien ngoai + vien trong) de tranh tran mau qua nhieu lop.
 */
RecursiveFillResult boundaryFillRecursiveDualBoundary(
    int seedX, int seedY, int fillColor, int boundaryColorA, int boundaryColorB,
    const RecursiveFillConfig *config)
{
    RecursiveFillConfig localCfg;
    const RecursiveFillConfig *cfg;
    RecursiveFillResult res;

    if (config)
    {
        cfg = config;
    }
    else
    {
        localCfg = makeDefaultRecursiveFillConfig();
        cfg = &localCfg;
    }

    res.calls = 0;
    res.aborted = 0;

    recursiveBoundaryFillInternal(seedX, seedY, fillColor, boundaryColorA,
                                  boundaryColorB, 1, cfg, &res, 0);
    return res;
}

#endif
