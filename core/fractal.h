#ifndef FRACTAL_H
#define FRACTAL_H

#include <graphics.h>
#include <math.h>

/* ====================================================================== */
/*                    FRACTAL CURVES LIBRARY                              */
/*                    ASTEROID BLASTER v3.0                               */
/*                                                                        */
/*  File này chứa các hàm đệ quy để vẽ đường cong Fractal:               */
/*  - Koch Curve: Đường cong tuyết (snowflake pattern)                   */
/*  - Dragon Curve: Đường cong rồng (lightning pattern)                  */
/*  - C-Curve (Levy C Curve): Đường cong C (smooth spiral pattern)       */
/*                                                                        */
/*  Tất cả hàm đều có ghi chú chi tiết về thuật toán và cách sử dụng.    */
/* ====================================================================== */

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ====================================================================== */
/*                    UTILITY FUNCTIONS (Hàm tiện ích)                    */
/* ====================================================================== */

/**
 * rotatePoint(): Xoay điểm (x, y) quanh tâm (cx, cy) một góc angle (radian)
 * 
 * @param x, y: Tọa độ điểm cần xoay
 * @param cx, cy: Tọa độ tâm xoay
 * @param angle: Góc xoay (radian)
 * @param newX, newY: Con trỏ lưu tọa độ mới sau khi xoay
 * 
 * Công thức xoay:
 *   newX = cx + (x - cx) * cos(angle) - (y - cy) * sin(angle)
 *   newY = cy + (x - cx) * sin(angle) + (y - cy) * cos(angle)
 */
void rotatePoint(float x, float y, float cx, float cy, float angle, float *newX, float *newY)
{
  float dx = x - cx;
  float dy = y - cy;
  *newX = cx + dx * cosf(angle) - dy * sinf(angle);
  *newY = cy + dx * sinf(angle) + dy * cosf(angle);
}

/**
 * colorGradient(): Tạo màu gradient giữa 2 màu
 * 
 * @param color1: Màu bắt đầu (WinBGIm color)
 * @param color2: Màu kết thúc (WinBGIm color)
 * @param t: Tỷ lệ (0.0 = color1, 1.0 = color2)
 * @return: Màu gradient (hiện tại trả về color1 hoặc color2)
 * 
 * Lưu ý: WinBGIm không hỗ trợ RGB trực tiếp, nên hàm này
 * chỉ chọn màu gần nhất dựa trên t
 */
int colorGradient(int color1, int color2, float t)
{
  /* Đơn giản hóa: chọn màu gần nhất */
  if (t < 0.5f)
    return color1;
  else
    return color2;
}

/* ====================================================================== */
/*                    KOCH CURVE (Đường Cong Koch)                        */
/* ====================================================================== */

/**
 * drawKochCurve(): Vẽ đường cong Koch đệ quy
 * 
 * Thuật toán:
 *   1. Nếu depth == 0: Vẽ đoạn thẳng từ (x1, y1) đến (x2, y2)
 *   2. Ngược lại:
 *      a. Chia đoạn thẳng thành 3 phần bằng nhau
 *      b. Tính điểm A (1/3 đoạn), điểm B (2/3 đoạn)
 *      c. Tính điểm C (đỉnh tam giác đều phía trên AB)
 *      d. Đệ quy vẽ 4 đoạn: (x1,y1)->A, A->C, C->B, B->(x2,y2)
 * 
 * @param x1, y1: Điểm bắt đầu
 * @param x2, y2: Điểm kết thúc
 * @param depth: Độ sâu đệ quy (0-5 khuyến nghị)
 * @param color: Màu vẽ (WinBGIm color constant)
 * 
 * Độ phức tạp: O(4^depth)
 * Khuyến nghị: depth <= 5 để tránh lag
 */
void drawKochCurve(float x1, float y1, float x2, float y2, int depth, int color)
{
  /* Điều kiện dừng: depth = 0, vẽ đoạn thẳng */
  if (depth == 0)
  {
    setcolor(color);
    line((int)x1, (int)y1, (int)x2, (int)y2);
    return;
  }

  /* Bước 1: Tính điểm A (1/3 đoạn) */
  float Ax = x1 + (x2 - x1) / 3.0f;
  float Ay = y1 + (y2 - y1) / 3.0f;

  /* Bước 2: Tính điểm B (2/3 đoạn) */
  float Bx = x1 + 2.0f * (x2 - x1) / 3.0f;
  float By = y1 + 2.0f * (y2 - y1) / 3.0f;

  /* Bước 3: Tính điểm C (đỉnh tam giác đều)
   * - Tính góc của vector AB
   * - Xoay 60 độ (-PI/3) để tạo đỉnh tam giác đều
   * - Tính độ dài AB
   * - Tính tọa độ C */
  float angle = atan2f(By - Ay, Bx - Ax) - (float)M_PI / 3.0f;
  float length = sqrtf((Bx - Ax) * (Bx - Ax) + (By - Ay) * (By - Ay));
  float Cx = Ax + length * cosf(angle);
  float Cy = Ay + length * sinf(angle);

  /* Bước 4: Đệ quy vẽ 4 đoạn con */
  drawKochCurve(x1, y1, Ax, Ay, depth - 1, color); /* Đoạn 1: (x1,y1) -> A */
  drawKochCurve(Ax, Ay, Cx, Cy, depth - 1, color); /* Đoạn 2: A -> C */
  drawKochCurve(Cx, Cy, Bx, By, depth - 1, color); /* Đoạn 3: C -> B */
  drawKochCurve(Bx, By, x2, y2, depth - 1, color); /* Đoạn 4: B -> (x2,y2) */
}

/**
 * drawKochRectangle(): Vẽ hình chữ nhật với viền Koch Curve
 * 
 * Ứng dụng: Viền nút bấm, viền khung trong menu
 * 
 * @param x1, y1: Góc trên trái
 * @param x2, y2: Góc dưới phải
 * @param depth: Độ sâu Koch Curve
 * @param color: Màu viền
 * 
 * Vẽ 4 cạnh Koch Curve:
 *   - Cạnh trên: (x1, y1) -> (x2, y1)
 *   - Cạnh phải: (x2, y1) -> (x2, y2)
 *   - Cạnh dưới: (x2, y2) -> (x1, y2)
 *   - Cạnh trái: (x1, y2) -> (x1, y1)
 */
void drawKochRectangle(int x1, int y1, int x2, int y2, int depth, int color)
{
  /* Vẽ 4 cạnh Koch Curve */
  drawKochCurve((float)x1, (float)y1, (float)x2, (float)y1, depth, color); /* Cạnh trên */
  drawKochCurve((float)x2, (float)y1, (float)x2, (float)y2, depth, color); /* Cạnh phải */
  drawKochCurve((float)x2, (float)y2, (float)x1, (float)y2, depth, color); /* Cạnh dưới */
  drawKochCurve((float)x1, (float)y2, (float)x1, (float)y1, depth, color); /* Cạnh trái */
}

/**
 * drawKochCircle(): Vẽ vòng tròn Koch Curve (xấp xỉ)
 * 
 * Ứng dụng: Khí quyển Trái Đất, hiệu ứng hào quang
 * 
 * @param cx, cy: Tâm vòng tròn
 * @param radius: Bán kính
 * @param depth: Độ sâu Koch Curve
 * @param color: Màu vẽ
 * 
 * Thuật toán:
 *   - Chia vòng tròn thành 12 đoạn (mỗi đoạn 30 độ)
 *   - Vẽ Koch Curve giữa các điểm liên tiếp
 */
void drawKochCircle(int cx, int cy, int radius, int depth, int color)
{
  int segments = 12; /* Số đoạn chia vòng tròn */
  float angleStep = 2.0f * (float)M_PI / segments;

  /* Vẽ Koch Curve giữa các điểm trên vòng tròn */
  for (int i = 0; i < segments; i++)
  {
    float angle1 = i * angleStep;
    float angle2 = (i + 1) * angleStep;

    float x1 = cx + radius * cosf(angle1);
    float y1 = cy + radius * sinf(angle1);
    float x2 = cx + radius * cosf(angle2);
    float y2 = cy + radius * sinf(angle2);

    drawKochCurve(x1, y1, x2, y2, depth, color);
  }
}

/**
 * drawKochSnowflake(): Vẽ bông tuyết Koch (Koch Snowflake)
 * 
 * Ứng dụng: Băng tuyết trên Mặt Trăng, hiệu ứng tuyết rơi
 * 
 * @param cx, cy: Tâm bông tuyết
 * @param size: Kích thước (bán kính tam giác ngoại tiếp)
 * @param depth: Độ sâu Koch Curve
 * @param color: Màu vẽ
 * 
 * Thuật toán:
 *   - Tạo tam giác đều với 3 đỉnh
 *   - Vẽ Koch Curve trên 3 cạnh tam giác
 */
void drawKochSnowflake(int cx, int cy, int size, int depth, int color)
{
  /* Tính 3 đỉnh tam giác đều */
  float angle1 = -(float)M_PI / 2.0f;                 /* Đỉnh trên */
  float angle2 = angle1 + 2.0f * (float)M_PI / 3.0f; /* Đỉnh dưới phải */
  float angle3 = angle2 + 2.0f * (float)M_PI / 3.0f; /* Đỉnh dưới trái */

  float x1 = cx + size * cosf(angle1);
  float y1 = cy + size * sinf(angle1);
  float x2 = cx + size * cosf(angle2);
  float y2 = cy + size * sinf(angle2);
  float x3 = cx + size * cosf(angle3);
  float y3 = cy + size * sinf(angle3);

  /* Vẽ Koch Curve trên 3 cạnh */
  drawKochCurve(x1, y1, x2, y2, depth, color);
  drawKochCurve(x2, y2, x3, y3, depth, color);
  drawKochCurve(x3, y3, x1, y1, depth, color);
}

/* ====================================================================== */
/*                    DRAGON CURVE (Đường Cong Rồng)                      */
/* ====================================================================== */

/**
 * drawDragonCurve(): Vẽ đường cong Dragon đệ quy
 * 
 * Thuật toán:
 *   1. Nếu depth == 0: Vẽ đoạn thẳng từ (x1, y1) đến (x2, y2)
 *   2. Ngược lại:
 *      a. Tính điểm giữa M = ((x1+x2)/2, (y1+y2)/2)
 *      b. Xoay vector 90 độ (hoặc -90 độ) để tạo điểm mới
 *      c. Đệ quy vẽ 2 nhánh với hướng xoay đối ngược
 * 
 * @param x1, y1: Điểm bắt đầu
 * @param x2, y2: Điểm kết thúc
 * @param depth: Độ sâu đệ quy (0-10 khuyến nghị)
 * @param turn: Hướng xoay (1 = trái, -1 = phải)
 * @param color: Màu vẽ
 * 
 * Độ phức tạp: O(2^depth)
 * Khuyến nghị: depth <= 10 để tránh lag
 */
void drawDragonCurve(float x1, float y1, float x2, float y2, int depth, int turn, int color)
{
  /* Điều kiện dừng: depth = 0, vẽ đoạn thẳng */
  if (depth == 0)
  {
    setcolor(color);
    line((int)x1, (int)y1, (int)x2, (int)y2);
    return;
  }

  /* Bước 1: Tính điểm giữa M */
  float Mx = (x1 + x2) / 2.0f;
  float My = (y1 + y2) / 2.0f;

  /* Bước 2: Xoay vector 90 độ (hoặc -90 độ)
   * - Vector từ M đến (x2, y2): (dx, dy)
   * - Xoay 90 độ: (dx, dy) -> (-dy, dx)
   * - Xoay -90 độ: (dx, dy) -> (dy, -dx)
   * - Điểm mới = M + vector xoay / 2 */
  float dx = x2 - Mx;
  float dy = y2 - My;
  float newX = Mx + turn * (-dy);
  float newY = My + turn * dx;

  /* Bước 3: Đệ quy vẽ 2 nhánh với hướng xoay đối ngược */
  drawDragonCurve(x1, y1, newX, newY, depth - 1, 1, color);  /* Nhánh 1: xoay trái */
  drawDragonCurve(newX, newY, x2, y2, depth - 1, -1, color); /* Nhánh 2: xoay phải */
}

/**
 * drawDragonCurveAnimated(): Vẽ Dragon Curve với animation
 * 
 * Ứng dụng: Tia Mặt Trời, Skill 9 (Beam), hiệu ứng sét đánh
 * 
 * @param frame: Frame hiện tại (dùng để tạo animation)
 * 
 * Animation: Thay đổi hướng xoay dựa trên frame
 */
void drawDragonCurveAnimated(float x1, float y1, float x2, float y2, int depth, int turn, int color, int frame)
{
  /* Thay đổi hướng xoay dựa trên frame để tạo animation "rung" */
  int animatedTurn = (frame % 4 < 2) ? turn : -turn;
  drawDragonCurve(x1, y1, x2, y2, depth, animatedTurn, color);
}

/* ====================================================================== */
/*                    C-CURVE (Đường Cong C / Levy C Curve)               */
/* ====================================================================== */

/**
 * drawCCurve(): Vẽ đường cong C đệ quy
 * 
 * Thuật toán:
 *   1. Nếu depth == 0: Vẽ đoạn thẳng từ (x1, y1) đến (x2, y2)
 *   2. Ngược lại:
 *      a. Tính điểm giữa M
 *      b. Xoay vector 45 độ để tạo điểm C
 *      c. Đệ quy vẽ 2 nhánh: (x1, y1) -> C và C -> (x2, y2)
 * 
 * @param x1, y1: Điểm bắt đầu
 * @param x2, y2: Điểm kết thúc
 * @param depth: Độ sâu đệ quy (0-10 khuyến nghị)
 * @param color: Màu vẽ
 * 
 * Độ phức tạp: O(2^depth)
 * Khuyến nghị: depth <= 10 để tránh lag
 */
void drawCCurve(float x1, float y1, float x2, float y2, int depth, int color)
{
  /* Điều kiện dừng: depth = 0, vẽ đoạn thẳng */
  if (depth == 0)
  {
    setcolor(color);
    line((int)x1, (int)y1, (int)x2, (int)y2);
    return;
  }

  /* Bước 1: Tính vector từ (x1, y1) đến (x2, y2) */
  float dx = x2 - x1;
  float dy = y2 - y1;

  /* Bước 2: Xoay vector 45 độ và chia đôi độ dài
   * - Góc hiện tại: atan2(dy, dx)
   * - Góc mới: góc hiện tại + 45 độ (PI/4)
   * - Độ dài mới: độ dài cũ / sqrt(2) */
  float angle = atan2f(dy, dx) + (float)M_PI / 4.0f;
  float length = sqrtf(dx * dx + dy * dy) / sqrtf(2.0f);

  /* Bước 3: Tính điểm C */
  float Cx = x1 + length * cosf(angle);
  float Cy = y1 + length * sinf(angle);

  /* Bước 4: Đệ quy vẽ 2 nhánh */
  drawCCurve(x1, y1, Cx, Cy, depth - 1, color); /* Nhánh 1: (x1, y1) -> C */
  drawCCurve(Cx, Cy, x2, y2, depth - 1, color); /* Nhánh 2: C -> (x2, y2) */
}

/**
 * drawCCurveEllipse(): Vẽ ellipse với C-Curve (xấp xỉ)
 * 
 * Ứng dụng: Vành Sao Thổ, vành đai phức tạp
 * 
 * @param cx, cy: Tâm ellipse
 * @param rx, ry: Bán kính trục x và y
 * @param depth: Độ sâu C-Curve
 * @param color: Màu vẽ
 * 
 * Thuật toán:
 *   - Chia ellipse thành 16 đoạn
 *   - Vẽ C-Curve giữa các điểm liên tiếp
 */
void drawCCurveEllipse(int cx, int cy, int rx, int ry, int depth, int color)
{
  int segments = 16; /* Số đoạn chia ellipse */
  float angleStep = 2.0f * (float)M_PI / segments;

  /* Vẽ C-Curve giữa các điểm trên ellipse */
  for (int i = 0; i < segments; i++)
  {
    float angle1 = i * angleStep;
    float angle2 = (i + 1) * angleStep;

    float x1 = cx + rx * cosf(angle1);
    float y1 = cy + ry * sinf(angle1);
    float x2 = cx + rx * cosf(angle2);
    float y2 = cy + ry * sinf(angle2);

    drawCCurve(x1, y1, x2, y2, depth, color);
  }
}

/**
 * drawCCurveSpiral(): Vẽ xoắn ốc C-Curve
 * 
 * Ứng dụng: Giải Ngân Hà, xoắn ốc phức tạp
 * 
 * @param cx, cy: Tâm xoắn ốc
 * @param radius: Bán kính
 * @param depth: Độ sâu C-Curve
 * @param color: Màu vẽ
 * 
 * Thuật toán:
 *   - Tạo xoắn ốc bằng cách tăng dần bán kính theo góc
 *   - Vẽ C-Curve giữa các điểm liên tiếp
 */
void drawCCurveSpiral(int cx, int cy, int radius, int depth, int color)
{
  int segments = 24; /* Số đoạn trong xoắn ốc */
  float angleStep = 2.0f * (float)M_PI / segments;

  /* Vẽ C-Curve theo xoắn ốc */
  for (int i = 0; i < segments; i++)
  {
    float angle1 = i * angleStep;
    float angle2 = (i + 1) * angleStep;

    /* Bán kính tăng dần theo góc để tạo xoắn ốc */
    float r1 = radius * (1.0f - (float)i / segments);
    float r2 = radius * (1.0f - (float)(i + 1) / segments);

    float x1 = cx + r1 * cosf(angle1);
    float y1 = cy + r1 * sinf(angle1);
    float x2 = cx + r2 * cosf(angle2);
    float y2 = cy + r2 * sinf(angle2);

    drawCCurve(x1, y1, x2, y2, depth, color);
  }
}

#endif /* FRACTAL_H */
