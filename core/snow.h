#ifndef SNOW_H
#define SNOW_H

/* ====================================================================== */
/*                    SNOW RENDERING SYSTEM - UI LAYER                    */
/*                    ASTEROID BLASTER v3.0                               */
/*                                                                        */
/*  Module vẽ UI cho bông tuyết Koch Snowflake                           */
/*  - Vẽ bông tuyết với màu sắc và đổ bóng                               */
/*  - Hỗ trợ hiệu ứng tan (thu nhỏ)                                      */
/*  - Tối ưu render với depth thấp                                       */
/* ====================================================================== */

/* Cấu trúc bông tuyết (dùng chung với snow_down.h) */
typedef struct
{
  float x, y;       /* Vị trí */
  float vx, vy;     /* Vận tốc */
  int size;         /* Kích thước (5-20 pixel) */
  int depth;        /* Độ sâu Koch (1-2 để tối ưu) */
  float rotation;   /* Góc xoay */
  float rotSpeed;   /* Tốc độ xoay */
  int alpha;        /* Độ trong suốt (0-255) */
  int fadeSpeed;    /* Tốc độ fade */
  int melting;      /* Đang tan (1) hay không (0) */
  float meltScale;  /* Tỷ lệ thu nhỏ khi tan (1.0 -> 0.0) */
  int color;        /* Màu sắc bông tuyết */
  int active;       /* Trạng thái */
} SnowParticle;

/**
 * getSnowColor(): Lấy màu sắc cho bông tuyết
 * 
 * @param colorType: Loại màu (0-4)
 * @return: Mã màu
 */
int getSnowColor(int colorType)
{
  switch (colorType)
  {
    case 0: return COL_TEXT_WHITE;    /* Trắng tinh khiết */
    case 1: return COL_STAR_BRIGHT;   /* Trắng sáng */
    case 2: return COL_STAR_BLUE;     /* Xanh nhạt */
    case 3: return COLOR(200, 220, 255); /* Xanh trắng */
    case 4: return COLOR(220, 240, 255); /* Xanh rất nhạt */
    default: return COL_TEXT_WHITE;
  }
}

/**
 * drawSnowWithShadow(): Vẽ bông tuyết với đổ bóng và màu sắc
 * 
 * @param cx, cy: Tâm bông tuyết
 * @param size: Kích thước
 * @param depth: Độ sâu Koch
 * @param rotation: Góc xoay
 * @param alpha: Độ trong suốt (0-255)
 * @param color: Màu sắc bông tuyết
 * @param meltScale: Tỷ lệ thu nhỏ (1.0 = bình thường, 0.0 = tan hết)
 */
void drawSnowWithShadow(int cx, int cy, int size, int depth, float rotation, 
                        int alpha, int color, float meltScale)
{
  /* Áp dụng hiệu ứng tan */
  int actualSize = (int)(size * meltScale);
  if (actualSize < 1) return;

  /* Tính 3 đỉnh tam giác đều */
  float angle1 = -(float)M_PI / 2.0f + rotation;
  float angle2 = angle1 + 2.0f * (float)M_PI / 3.0f;
  float angle3 = angle2 + 2.0f * (float)M_PI / 3.0f;

  float x1 = cx + actualSize * cosf(angle1);
  float y1 = cy + actualSize * sinf(angle1);
  float x2 = cx + actualSize * cosf(angle2);
  float y2 = cy + actualSize * sinf(angle2);
  float x3 = cx + actualSize * cosf(angle3);
  float y3 = cy + actualSize * sinf(angle3);

  /* Vẽ bóng (offset xuống dưới và sang phải một chút) */
  if (alpha > 100 && meltScale > 0.5f) /* Chỉ vẽ bóng khi alpha đủ cao và chưa tan nhiều */
  {
    int shadowOffset = 2;
    int shadowColor = COL_STAR_DIM;
    
    /* Vẽ Koch Curve cho bóng */
    drawKochCurve(x1 + shadowOffset, y1 + shadowOffset, 
                  x2 + shadowOffset, y2 + shadowOffset, 
                  depth, shadowColor);
    drawKochCurve(x2 + shadowOffset, y2 + shadowOffset, 
                  x3 + shadowOffset, y3 + shadowOffset, 
                  depth, shadowColor);
    drawKochCurve(x3 + shadowOffset, y3 + shadowOffset, 
                  x1 + shadowOffset, y1 + shadowOffset, 
                  depth, shadowColor);
  }

  /* Chọn màu dựa trên alpha và màu gốc */
  int snowColor = color;
  if (alpha < 100)
    snowColor = COL_STAR_DIM; /* Fade out */
  else if (alpha < 150)
    snowColor = COL_STAR_MED;

  /* Vẽ Koch Snowflake */
  drawKochCurve(x1, y1, x2, y2, depth, snowColor);
  drawKochCurve(x2, y2, x3, y3, depth, snowColor);
  drawKochCurve(x3, y3, x1, y1, depth, snowColor);

  /* Thêm điểm sáng ở tâm (highlight) */
  if (alpha > 150 && actualSize >= 5 && meltScale > 0.7f)
  {
    setcolor(COL_TEXT_WHITE);
    putpixel(cx, cy, COL_TEXT_WHITE);
    if (actualSize >= 8)
    {
      putpixel(cx + 1, cy, COL_TEXT_WHITE);
      putpixel(cx - 1, cy, COL_TEXT_WHITE);
      putpixel(cx, cy + 1, COL_TEXT_WHITE);
      putpixel(cx, cy - 1, COL_TEXT_WHITE);
    }
  }
}

#endif /* SNOW_H */
