# 🎨 HƯỚNG DẪN TRIỂN KHAI FRACTAL CURVES - ASTEROID BLASTER v3.0

## 📋 TỔNG QUAN

Tài liệu này mô tả chi tiết cách tích hợp các đường cong fractal (Koch Curve, Dragon Curve, C-Curve) vào game Asteroid Blaster để tăng tính thẩm mỹ và hiệu ứng hình ảnh.

---

## 🔧 CẤU TRÚC FILE MỚI

Tạo file header mới: `core/fractal.h`

File này sẽ chứa:

- Thuật toán vẽ Koch Curve
- Thuật toán vẽ Dragon Curve
- Thuật toán vẽ C-Curve (Levy C Curve)
- Các hàm tiện ích để tính toán điểm, góc, và biến đổi

---

## 📐 1. KOCH CURVE (Đường Cong Koch)

### **Thuật toán cơ bản**:

```
Đầu vào: 2 điểm (x1, y1) và (x2, y2), độ sâu đệ quy depth
Nếu depth == 0:
    Vẽ đoạn thẳng từ (x1, y1) đến (x2, y2)
Ngược lại:
    1. Chia đoạn thẳng thành 3 phần bằng nhau
    2. Tính điểm A (1/3 đoạn), điểm B (2/3 đoạn)
    3. Tính điểm C (đỉnh tam giác đều phía trên AB)
    4. Đệ quy vẽ 4 đoạn: (x1,y1)->A, A->C, C->B, B->(x2,y2)
```

### **Công thức tính toán**:

```cpp
// Điểm A (1/3 đoạn)
Ax = x1 + (x2 - x1) / 3.0
Ay = y1 + (y2 - y1) / 3.0

// Điểm B (2/3 đoạn)
Bx = x1 + 2 * (x2 - x1) / 3.0
By = y1 + 2 * (y2 - y1) / 3.0

// Điểm C (đỉnh tam giác đều)
// Xoay vector AB 60 độ quanh A
angle = atan2(By - Ay, Bx - Ax) - PI/3
length = sqrt((Bx-Ax)^2 + (By-Ay)^2)
Cx = Ax + length * cos(angle)
Cy = Ay + length * sin(angle)
```

### **Ứng dụng trong game**:

#### **A. Menu chính - Viền tiêu đề & nút**

**Vị trí**: `ui/menu.h` - Hàm `drawMainMenu()`

**Mô tả chi tiết**:

- **Viền tiêu đề "ASTEROID BLASTER"**:
  - Thay thế viền chữ nhật đơn giản bằng Koch Curve depth 2-3
  - Tạo 4 cạnh Koch Curve bao quanh text
  - Màu sắc: Gradient từ xanh lam nhạt → trắng (hiệu ứng băng giá)
  - Animation: Các đỉnh Koch nhấp nháy như tinh thể tuyết

- **Viền các nút (BẮT ĐẦU, HƯỚNG DẪN, THOÁT)**:
  - Thay `rectangle()` bằng Koch Curve depth 1-2
  - Khi hover: Koch Curve sáng lên + animation "rung" nhẹ
  - Màu sắc:
    - Normal: COL_BTN_NORMAL với viền Koch xám nhạt
    - Hover: COL*BTN_HOVER*\* với viền Koch sáng

**Code mẫu**:

```cpp
// Thay thế:
// rectangle(MENU_START_X1, MENU_START_Y1, MENU_START_X2, MENU_START_Y2);

// Bằng:
drawKochRectangle(MENU_START_X1, MENU_START_Y1,
                  MENU_START_X2, MENU_START_Y2,
                  2, // depth
                  startHov ? COL_BTN_HOVER_GREEN : COL_BTN_NORMAL);
```

#### **B. Background Level 4 - Trái Đất**

**Vị trí**: `ui/background_level.h` - Hàm `drawLevel4Background()`

**Mô tả chi tiết**:

- **Khí quyển Trái Đất**:
  - Vẽ vòng tròn Koch Curve (Koch Snowflake) xung quanh Trái Đất
  - Depth 2-3 để tạo cảm giác khí quyển phức tạp
  - Màu sắc: Gradient xanh dương nhạt → trong suốt
  - Bán kính: 200-220 pixel (lớn hơn Trái Đất một chút)

- **Băng tuyết trên Mặt Trăng**:
  - Vẽ các Koch Snowflake nhỏ (depth 1-2) trên bề mặt Mặt Trăng
  - Vị trí: Ngẫu nhiên xung quanh các miệng núi lửa
  - Màu sắc: Trắng sáng (COL_TEXT_WHITE)
  - Kích thước: 5-15 pixel

**Code mẫu**:

```cpp
// Khí quyển Trái Đất
drawKochCircle(ex, ey, 200, 3, COL_EARTH_CLOUD);

// Băng tuyết Mặt Trăng
for (int i = 0; i < 10; i++) {
    int sx = mx + (rand() % 100) - 50;
    int sy = my + (rand() % 100) - 50;
    drawKochSnowflake(sx, sy, 8, 1, COL_TEXT_WHITE);
}
```

#### **C. Game Over - Viền khung**

**Vị trí**: `ui/game_over.h` - Hàm `drawGameOverScreen()`

**Mô tả chi tiết**:

- **Viền khung "KẾT THÚC"**:
  - Thay 2 lớp `rectangle()` bằng Koch Curve depth 2-3
  - Tạo cảm giác **vỡ vụn, tan rã** như kính vỡ
  - Màu sắc: Đỏ sẫm (COL_GAMEOVER_BORDER) với hiệu ứng nhấp nháy
  - Animation: Các đỉnh Koch "rung" ngẫu nhiên

**Code mẫu**:

```cpp
// Thay thế:
// rectangle(WIDTH/2 - 280, HEIGHT/2 - 140, WIDTH/2 + 280, HEIGHT/2 + 190);

// Bằng:
drawKochRectangle(WIDTH/2 - 280, HEIGHT/2 - 140,
                  WIDTH/2 + 280, HEIGHT/2 + 190,
                  3, // depth cao để tạo cảm giác vỡ vụn
                  COL_GAMEOVER_BORDER);
```

---

## 🐉 2. DRAGON CURVE (Đường Cong Rồng)

### **Thuật toán cơ bản**:

```
Đầu vào: 2 điểm (x1, y1) và (x2, y2), độ sâu depth, hướng turn (1 hoặc -1)
Nếu depth == 0:
    Vẽ đoạn thẳng từ (x1, y1) đến (x2, y2)
Ngược lại:
    1. Tính điểm giữa M
    2. Xoay vector từ M đến (x2, y2) 90 độ theo hướng turn
    3. Đệ quy vẽ 2 nhánh:
       - (x1, y1) -> M với turn = 1
       - M -> (x2, y2) với turn = -1
```

### **Công thức tính toán**:

```cpp
// Điểm giữa M
Mx = (x1 + x2) / 2.0
My = (y1 + y2) / 2.0

// Xoay 90 độ (hoặc -90 độ)
dx = x2 - x1
dy = y2 - y1
// Xoay 90 độ: (dx, dy) -> (-dy, dx)
// Xoay -90 độ: (dx, dy) -> (dy, -dx)
newX = Mx + turn * (-dy) / 2.0
newY = My + turn * dx / 2.0
```

### **Ứng dụng trong game**:

#### **A. Background Level 2 - Tia Mặt Trời**

**Vị trí**: `ui/background_level.h` - Hàm `drawLevel2Background()`

**Mô tả chi tiết**:

- **Tia sáng Mặt Trời (Solar Flares)**:
  - Thay thế 12 đoạn thẳng `line()` bằng Dragon Curve depth 3-4
  - Mỗi tia bắt đầu từ bán kính 210px, kết thúc ở 280px
  - Màu sắc: Gradient từ vàng rực (COL_SUN_FLARE_A) → cam (COL_SUN_FLARE_B)
  - Animation: Các tia "rung" và thay đổi hình dạng theo frame

**Code mẫu**:

```cpp
// Thay thế:
// for (i = 0; i < 12; i++) {
//     line(x1, y1, x2, y2);
// }

// Bằng:
for (i = 0; i < 12; i++) {
    float angle = i * 30.0f * 3.14159f / 180.0f;
    int x1 = (int)(cosf(angle) * 210);
    int y1 = (int)(sinf(angle) * 210);
    int x2 = (int)(cosf(angle) * 280);
    int y2 = (int)(sinf(angle) * 280);

    int color = (i % 2) ? COL_SUN_FLARE_A : COL_SUN_FLARE_B;
    drawDragonCurve(x1, y1, x2, y2, 4, 1, color);
}
```

#### **B. Background Level 6 - Lỗ Đen**

**Vị trí**: `ui/background_level.h` - Hàm `drawLevel6Background()`

**Mô tả chi tiết**:

- **Dòng năng lượng bị hút vào Lỗ Đen**:
  - Thay thế 15 đoạn thẳng `line(sx, sy, bx, by)` bằng Dragon Curve depth 4-5
  - Tạo quỹ đạo uốn lượn, xoắn ốc phức tạp
  - Màu sắc: Gradient từ sáng (COL_STAR_GLOW) → tối (COL_BH_HALO)
  - Animation: Các đường cong "chảy" về phía Lỗ Đen

**Code mẫu**:

```cpp
// Thay thế:
// line(sx, sy, bx, by);

// Bằng:
for (i = 0; i < 15; i++) {
    float angle = i * 24.0f * 3.14159f / 180.0f;
    int dist = 60 + (i * 13) % 80;
    int sx = bx + (int)(cosf(angle) * dist);
    int sy = by + (int)(sinf(angle) * dist);

    int color = (i % 2) ? COL_STAR_WARM : COL_STAR_GLOW;
    drawDragonCurve(sx, sy, bx, by, 5, 1, color);
}
```

#### **C. Skill 9 - Tia Xuyên (Piercing Beam)**

**Vị trí**: `core/draw_effects.h` - Hàm `drawSkillEffects()`

**Mô tả chi tiết**:

- **Tia laser phức tạp**:
  - Thay thế đoạn thẳng `line()` bằng Dragon Curve depth 3-4
  - Tạo hiệu ứng **sét đánh, năng lượng bùng nổ**
  - Màu sắc: Gradient từ xanh lam sáng (COL_BEAM_OUTER) → trắng (COL_BEAM_CORE)
  - Animation: Dragon Curve "rung" và thay đổi hình dạng mỗi frame

**Code mẫu**:

```cpp
// Trong drawSkillEffects(), phần Beam:
if (sk->beamActive) {
    // Thay thế:
    // line((int)sk->beamX, (int)sk->beamY, (int)sk->beamEndX, (int)sk->beamEndY);

    // Bằng:
    drawDragonCurve((int)sk->beamX, (int)sk->beamY,
                    (int)sk->beamEndX, (int)sk->beamEndY,
                    4, 1, COL_BEAM_OUTER);

    // Vẽ thêm lớp trong sáng hơn
    drawDragonCurve((int)sk->beamX, (int)sk->beamY,
                    (int)sk->beamEndX, (int)sk->beamEndY,
                    3, -1, COL_BEAM_CORE);
}
```

---

## 🌀 3. C-CURVE (Đường Cong C / Levy C Curve)

### **Thuật toán cơ bản**:

```
Đầu vào: 2 điểm (x1, y1) và (x2, y2), độ sâu depth
Nếu depth == 0:
    Vẽ đoạn thẳng từ (x1, y1) đến (x2, y2)
Ngược lại:
    1. Tính điểm giữa M
    2. Xoay vector 45 độ để tạo điểm C
    3. Đệ quy vẽ 2 nhánh: (x1, y1) -> C và C -> (x2, y2)
```

### **Công thức tính toán**:

```cpp
// Điểm giữa M
Mx = (x1 + x2) / 2.0
My = (y1 + y2) / 2.0

// Xoay 45 độ
dx = x2 - x1
dy = y2 - y1
angle = atan2(dy, dx) + PI/4
length = sqrt(dx*dx + dy*dy) / sqrt(2)
Cx = x1 + length * cos(angle)
Cy = y1 + length * sin(angle)
```

### **Ứng dụng trong game**:

#### **A. Background Level 3 - Vành Sao Thổ**

**Vị trí**: `ui/background_level.h` - Hàm `drawLevel3Background()`

**Mô tả chi tiết**:

- **Vành đai Sao Thổ (Saturn Rings)**:
  - Thay thế các `ellipse()` đơn giản bằng C-Curve depth 2-3
  - Tạo cấu trúc vành đai phức tạp, chi tiết hơn
  - Màu sắc: Gradient từ tối (COL_SATURN_RING_DK) → sáng (COL_SATURN_RING_LT)
  - Vẽ nhiều lớp C-Curve chồng lên nhau

**Code mẫu**:

```cpp
// Thay thế:
// for (r = 140; r < 200; r += 3) {
//     ellipse(sx, sy, 20, 160, r, r / 3);
// }

// Bằng:
for (r = 140; r < 200; r += 5) {
    drawCCurveEllipse(sx, sy, r, r / 3, 2, COL_SATURN_RING_DK);
}
```

#### **B. Background Level 5 - Giải Ngân Hà Spiral**

**Vị trí**: `ui/background_level.h` - Hàm `drawLevel5Background()`

**Mô tả chi tiết**:

- **Xoắn ốc Giải Ngân Hà**:
  - Thay thế các `ellipse()` bằng C-Curve depth 3-4
  - Tạo cấu trúc xoắn ốc tự nhiên, phức tạp hơn
  - Màu sắc: Gradient từ sáng (COL_GALAXY_CORE) → tối (COL_GALAXY_OUTER)
  - Animation: Xoắn ốc quay chậm theo thời gian

**Code mẫu**:

```cpp
// Thay thế:
// for (r = 25; r < 120; r += 15) {
//     ellipse(gx, gy, 0, 360, r, r / 2);
// }

// Bằng:
for (r = 25; r < 120; r += 15) {
    int c = (r < 50) ? COL_GALAXY_INNER :
            (r < 80) ? COL_GALAXY_MID : COL_GALAXY_OUTER;
    drawCCurveSpiral(gx, gy, r, 3, c);
}
```

---

## 🎨 4. HÀM TIỆN ÍCH CẦN IMPLEMENT

### **File: `core/fractal.h`**

```cpp
#ifndef FRACTAL_H
#define FRACTAL_H

/* ====================================================================== */
/*                    FRACTAL CURVES LIBRARY                              */
/* ====================================================================== */

/* --- Koch Curve --- */
void drawKochCurve(int x1, int y1, int x2, int y2, int depth, int color);
void drawKochRectangle(int x1, int y1, int x2, int y2, int depth, int color);
void drawKochCircle(int cx, int cy, int radius, int depth, int color);
void drawKochSnowflake(int cx, int cy, int size, int depth, int color);

/* --- Dragon Curve --- */
void drawDragonCurve(int x1, int y1, int x2, int y2, int depth, int turn, int color);
void drawDragonCurveAnimated(int x1, int y1, int x2, int y2, int depth, int turn, int color, int frame);

/* --- C-Curve (Levy C Curve) --- */
void drawCCurve(int x1, int y1, int x2, int y2, int depth, int color);
void drawCCurveEllipse(int cx, int cy, int rx, int ry, int depth, int color);
void drawCCurveSpiral(int cx, int cy, int radius, int depth, int color);

/* --- Utility Functions --- */
void rotatePoint(int x, int y, int cx, int cy, float angle, int *newX, int *newY);
int colorGradient(int color1, int color2, float t);

#endif
```

---

## 🎯 5. HƯỚNG DẪN TRIỂN KHAI

### **Bước 1: Tạo file `core/fractal.h`**

- Implement các hàm vẽ Koch Curve, Dragon Curve, C-Curve
- Sử dụng đệ quy với giới hạn depth để tránh tràn stack
- Tối ưu: Cache các điểm đã tính toán

### **Bước 2: Tích hợp vào UI**

- **Menu chính**: Thay `rectangle()` bằng `drawKochRectangle()`
- **Background Level 2**: Thay `line()` bằng `drawDragonCurve()`
- **Background Level 3**: Thay `ellipse()` bằng `drawCCurveEllipse()`
- **Background Level 4**: Thêm `drawKochCircle()` và `drawKochSnowflake()`
- **Background Level 5**: Thay `ellipse()` bằng `drawCCurveSpiral()`
- **Background Level 6**: Thay `line()` bằng `drawDragonCurve()`
- **Game Over**: Thay `rectangle()` bằng `drawKochRectangle()`
- **Skill 9**: Thay `line()` bằng `drawDragonCurve()`

### **Bước 3: Thêm Animation**

- Sử dụng `frameCount` để tạo hiệu ứng động
- Koch Curve: Nhấp nháy các đỉnh
- Dragon Curve: Rung và thay đổi hình dạng
- C-Curve: Xoay chậm

### **Bước 4: Tối ưu hiệu suất**

- Giới hạn depth: 1-5 (depth càng cao càng chậm)
- Cache: Lưu các điểm đã tính toán
- Chỉ vẽ khi cần thiết (không vẽ lại mỗi frame nếu không cần)

---

## 📊 6. BẢNG THAM SỐ ĐỀ XUẤT

| **Vị trí**          | **Fractal** | **Depth** | **Màu sắc**        | **Animation**   |
| ------------------- | ----------- | --------- | ------------------ | --------------- |
| Menu - Tiêu đề      | Koch        | 2-3       | Xanh lam → Trắng   | Nhấp nháy       |
| Menu - Nút          | Koch        | 1-2       | Xám → Sáng (hover) | Rung nhẹ        |
| Level 4 - Trái Đất  | Koch        | 2-3       | Xanh dương nhạt    | Không           |
| Level 4 - Mặt Trăng | Koch        | 1-2       | Trắng sáng         | Không           |
| Game Over           | Koch        | 3         | Đỏ sẫm             | Rung ngẫu nhiên |
| Level 2 - Mặt Trời  | Dragon      | 3-4       | Vàng → Cam         | Rung            |
| Level 6 - Lỗ Đen    | Dragon      | 4-5       | Sáng → Tối         | Chảy            |
| Skill 9 - Beam      | Dragon      | 3-4       | Xanh → Trắng       | Rung            |
| Level 3 - Sao Thổ   | C-Curve     | 2-3       | Tối → Sáng         | Không           |
| Level 5 - Ngân Hà   | C-Curve     | 3-4       | Sáng → Tối         | Xoay chậm       |

---

## 🚀 7. KẾT QUẢ MONG ĐỢI

Sau khi triển khai, game sẽ có:

- ✅ Giao diện menu đẹp hơn với viền Koch Curve băng giá
- ✅ Background phức tạp, chi tiết hơn với Dragon Curve và C-Curve
- ✅ Hiệu ứng skill ấn tượng hơn với Dragon Curve
- ✅ Cảm giác chuyên nghiệp, nghệ thuật hơn
- ✅ Tăng tính thẩm mỹ mà không ảnh hưởng gameplay

---

## 📝 8. GHI CHÚ QUAN TRỌNG

### **Hiệu suất**:

- Depth > 5: Rất chậm, tránh sử dụng
- Depth 3-4: Cân bằng giữa đẹp và hiệu suất
- Depth 1-2: Nhanh, phù hợp cho animation

### **Màu sắc**:

- Sử dụng các constants từ `core/color.h`
- Tạo gradient bằng hàm `colorGradient()`
- Tránh dùng màu quá sáng (gây chói mắt)

### **Animation**:

- Sử dụng `frameCount % N` để tạo chu kỳ
- Giới hạn tốc độ animation (không quá nhanh)
- Chỉ animate khi cần thiết (tiết kiệm CPU)

---

**Tác giả**: Kiro AI Assistant  
**Phiên bản**: 1.0  
**Ngày**: 2026-05-12

---

## 🎓 9. TIÊU CHÍ ĐÁNH GIÁ VÀ HƯỚNG DẪN CHI TIẾT

### **✅ Tiêu chí 1: Cài đặt chính xác hàm đệ quy**

**Yêu cầu**:

- Hàm đệ quy phải có điều kiện dừng rõ ràng (depth == 0)
- Công thức toán học chính xác (chia 3, xoay 60°, 90°, 45°)
- Ghi chú đầy đủ cho từng bước thuật toán

**Kiểm tra**:

```cpp
// File: core/fractal.h đã implement đầy đủ:
// ✅ drawKochCurve() - Có ghi chú chi tiết từng bước
// ✅ drawDragonCurve() - Có ghi chú chi tiết từng bước
// ✅ drawCCurve() - Có ghi chú chi tiết từng bước
```

**Ví dụ ghi chú tốt** (đã có trong `core/fractal.h`):

```cpp
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
```

---

### **✅ Tiêu chí 2: Lồng ghép sáng tạo vào game**

**Yêu cầu**:

- Fractal phải là chi tiết tự nhiên trong game (không rời rạc)
- Phối hợp hài hòa với bối cảnh (màu sắc, kích thước)
- Tạo giá trị thẩm mỹ (không chỉ để "có Fractal")

**Các vị trí lồng ghép sáng tạo**:

#### **A. Koch Curve - Hiệu ứng băng giá, tuyết rơi**

1. **Menu chính - Viền tiêu đề**:

   ```cpp
   // Trong ui/menu.h - drawMainMenu()
   // Thay thế viền chữ nhật đơn giản:

   // CŨ:
   // setcolor(COL_TEXT_TITLE);
   // rectangle(WIDTH/2 - 250, HEIGHT/2 - 220, WIDTH/2 + 250, HEIGHT/2 - 100);

   // MỚI: Viền Koch Curve băng giá
   drawKochRectangle(WIDTH/2 - 250, HEIGHT/2 - 220,
                     WIDTH/2 + 250, HEIGHT/2 - 100,
                     2, COL_TEXT_TITLE);
   ```

   **Giải thích**: Tạo cảm giác tiêu đề như được khắc trên băng giá

2. **Level 4 - Khí quyển Trái Đất**:

   ```cpp
   // Trong ui/background_level.h - drawLevel4Background()
   // Sau khi vẽ Trái Đất, thêm:

   /* Khí quyển phức tạp với Koch Curve */
   drawKochCircle(ex, ey, 200, 2, COL_EARTH_CLOUD);
   drawKochCircle(ex, ey, 210, 2, COL_EARTH_CLOUD);
   ```

   **Giải thích**: Tạo lớp khí quyển phức tạp, tự nhiên hơn ellipse đơn giản

3. **Level 4 - Băng tuyết Mặt Trăng**:
   ```cpp
   /* Băng tuyết trên Mặt Trăng */
   for (int i = 0; i < 8; i++) {
       int sx = mx + (rand() % 80) - 40;
       int sy = my + (rand() % 80) - 40;
       int size = 5 + (rand() % 10);
       drawKochSnowflake(sx, sy, size, 1, COL_TEXT_WHITE);
   }
   ```
   **Giải thích**: Tạo chi tiết băng tuyết tự nhiên trên bề mặt Mặt Trăng

#### **B. Dragon Curve - Hiệu ứng năng lượng, sét đánh**

1. **Level 2 - Tia Mặt Trời**:

   ```cpp
   // Trong ui/background_level.h - drawLevel2Background()
   // Thay thế tia thẳng:

   /* Tia sang Mat Troi (Solar Flares) - DRAGON CURVE */
   for (i = 0; i < 12; i++) {
       float angle = i * 30.0f * 3.14159f / 180.0f;
       int x1 = (int)(cosf(angle) * 210);
       int y1 = (int)(sinf(angle) * 210);
       int x2 = (int)(cosf(angle) * 280);
       int y2 = (int)(sinf(angle) * 280);

       int color = (i % 2) ? COL_SUN_FLARE_A : COL_SUN_FLARE_B;
       drawDragonCurve((float)x1, (float)y1, (float)x2, (float)y2,
                       4, 1, color);
   }
   ```

   **Giải thích**: Tia Mặt Trời trở nên uốn lượn, năng động như tia lửa thật

2. **Level 6 - Dòng năng lượng Lỗ Đen**:

   ```cpp
   // Trong ui/background_level.h - drawLevel6Background()
   // Thay thế đường thẳng:

   /* Cac ngoi sao bi hut vao - DRAGON CURVE */
   for (i = 0; i < 15; i++) {
       float angle = i * 24.0f * 3.14159f / 180.0f;
       int dist = 60 + (i * 13) % 80;
       int sx = bx + (int)(cosf(angle) * dist);
       int sy = by + (int)(sinf(angle) * dist);

       int color = (i % 2) ? COL_STAR_WARM : COL_STAR_GLOW;
       drawDragonCurve((float)sx, (float)sy, (float)bx, (float)by,
                       5, 1, color);
   }
   ```

   **Giải thích**: Dòng năng lượng xoắn ốc phức tạp, tạo cảm giác bị hút mạnh mẽ

3. **Skill 9 - Tia Xuyên (Piercing Beam)**:

   ```cpp
   // Trong core/draw_effects.h - drawSkillEffects()
   // Phần Beam:

   if (sk->beamActive) {
       /* Lớp ngoài - Dragon Curve depth 4 */
       drawDragonCurve(sk->beamX, sk->beamY,
                       sk->beamEndX, sk->beamEndY,
                       4, 1, COL_BEAM_OUTER);

       /* Lớp trong sáng hơn - Dragon Curve depth 3 */
       drawDragonCurve(sk->beamX, sk->beamY,
                       sk->beamEndX, sk->beamEndY,
                       3, -1, COL_BEAM_CORE);
   }
   ```

   **Giải thích**: Tia laser trở nên phức tạp như sét đánh, ấn tượng hơn

#### **C. C-Curve - Hiệu ứng xoắn ốc, vành đai**

1. **Level 3 - Vành Sao Thổ**:

   ```cpp
   // Trong ui/background_level.h - drawLevel3Background()
   // Thay thế ellipse đơn giản:

   /* Vanh dai (Rings) - C-CURVE */
   setcolor(COL_SATURN_RING_DK);
   for (r = 140; r < 200; r += 5) {
       drawCCurveEllipse(sx, sy, r, r / 3, 2, COL_SATURN_RING_DK);
   }
   setcolor(COL_SATURN_RING_LT);
   for (r = 140; r < 200; r += 5) {
       drawCCurveEllipse(sx, sy, r, r / 3, 2, COL_SATURN_RING_LT);
   }
   ```

   **Giải thích**: Vành đai phức tạp, chi tiết hơn, giống vành đai thật

2. **Level 5 - Giải Ngân Hà**:

   ```cpp
   // Trong ui/background_level.h - drawLevel5Background()
   // Thay thế ellipse:

   /* Cac vong xoay oc - C-CURVE SPIRAL */
   for (r = 25; r < 120; r += 15) {
       int c = (r < 50) ? COL_GALAXY_INNER :
               (r < 80) ? COL_GALAXY_MID : COL_GALAXY_OUTER;
       drawCCurveSpiral(gx, gy, r, 3, c);
   }
   ```

   **Giải thích**: Xoắn ốc tự nhiên, phức tạp như giải ngân hà thật

---

### **✅ Tiêu chí 3: Hình ảnh sắc nét, màu sắc hài hòa**

**Yêu cầu**:

- Fractal hiển thị rõ ràng, không bị vỡ
- Màu sắc phối hợp với bối cảnh game
- Tạo hiệu ứng thị giác ấn tượng

**Hướng dẫn chọn màu**:

#### **A. Koch Curve - Màu lạnh (xanh, trắng)**

```cpp
/* Menu - Hiệu ứng băng giá */
drawKochRectangle(x1, y1, x2, y2, 2, COL_TEXT_SUBTITLE); // Xanh lam nhạt

/* Level 4 - Khí quyển */
drawKochCircle(ex, ey, 200, 2, COL_EARTH_CLOUD); // Xanh dương nhạt

/* Level 4 - Băng tuyết */
drawKochSnowflake(sx, sy, 8, 1, COL_TEXT_WHITE); // Trắng sáng

/* Game Over - Vỡ vụn */
drawKochRectangle(x1, y1, x2, y2, 3, COL_GAMEOVER_BORDER); // Đỏ sẫm
```

#### **B. Dragon Curve - Màu nóng (vàng, cam, đỏ)**

```cpp
/* Level 2 - Tia Mặt Trời */
int color = (i % 2) ? COL_SUN_FLARE_A : COL_SUN_FLARE_B; // Vàng/Cam
drawDragonCurve(x1, y1, x2, y2, 4, 1, color);

/* Level 6 - Lỗ Đen */
int color = (i % 2) ? COL_STAR_WARM : COL_STAR_GLOW; // Vàng/Trắng
drawDragonCurve(sx, sy, bx, by, 5, 1, color);

/* Skill 9 - Beam */
drawDragonCurve(x1, y1, x2, y2, 4, 1, COL_BEAM_OUTER); // Xanh lam sáng
drawDragonCurve(x1, y1, x2, y2, 3, -1, COL_BEAM_CORE); // Trắng
```

#### **C. C-Curve - Màu trung tính (xám, tím, xanh)**

```cpp
/* Level 3 - Vành Sao Thổ */
drawCCurveEllipse(sx, sy, r, r/3, 2, COL_SATURN_RING_DK); // Xám tối
drawCCurveEllipse(sx, sy, r, r/3, 2, COL_SATURN_RING_LT); // Xám sáng

/* Level 5 - Giải Ngân Hà */
int c = (r < 50) ? COL_GALAXY_INNER :  // Trắng sáng
        (r < 80) ? COL_GALAXY_MID :    // Xanh nhạt
                   COL_GALAXY_OUTER;   // Tím nhạt
drawCCurveSpiral(gx, gy, r, 3, c);
```

**Bảng phối màu hài hòa**:

| **Bối cảnh**       | **Fractal** | **Màu chính** | **Màu phụ** | **Hiệu ứng**      |
| ------------------ | ----------- | ------------- | ----------- | ----------------- |
| Menu (nền tối)     | Koch        | Xanh lam nhạt | Trắng       | Băng giá sáng     |
| Level 2 (Mặt Trời) | Dragon      | Vàng rực      | Cam         | Tia lửa nóng      |
| Level 3 (Sao Thổ)  | C-Curve     | Xám tối       | Xám sáng    | Vành đai kim loại |
| Level 4 (Trái Đất) | Koch        | Xanh dương    | Trắng       | Khí quyển + tuyết |
| Level 5 (Ngân Hà)  | C-Curve     | Trắng sáng    | Tím nhạt    | Xoắn ốc vũ trụ    |
| Level 6 (Lỗ Đen)   | Dragon      | Vàng          | Trắng       | Năng lượng bị hút |
| Skill 9 (Beam)     | Dragon      | Xanh lam      | Trắng       | Sét đánh          |
| Game Over          | Koch        | Đỏ sẫm        | Đỏ sáng     | Vỡ vụn            |

---

## 🎯 10. CHECKLIST TRIỂN KHAI

### **Bước 1: Tạo file `core/fractal.h`** ✅

- [x] Copy code từ phần 4 vào file
- [x] Kiểm tra compile (không lỗi syntax)
- [x] Test từng hàm riêng lẻ

### **Bước 2: Include vào `AsteroidBlaster.cpp`**

```cpp
/* --- Core headers --- */
#include "core/types.h"
#include "core/fractal.h"  // ← THÊM DÒNG NÀY
```

### **Bước 3: Tích hợp vào UI**

- [ ] **Menu chính** (`ui/menu.h`):
  - [ ] Viền tiêu đề Koch Curve
  - [ ] Viền nút Koch Curve
- [ ] **Level 2** (`ui/background_level.h`):
  - [ ] Tia Mặt Trời Dragon Curve
- [ ] **Level 3** (`ui/background_level.h`):
  - [ ] Vành Sao Thổ C-Curve
- [ ] **Level 4** (`ui/background_level.h`):
  - [ ] Khí quyển Trái Đất Koch Circle
  - [ ] Băng tuyết Mặt Trăng Koch Snowflake
- [ ] **Level 5** (`ui/background_level.h`):
  - [ ] Giải Ngân Hà C-Curve Spiral
- [ ] **Level 6** (`ui/background_level.h`):
  - [ ] Dòng năng lượng Lỗ Đen Dragon Curve
- [ ] **Game Over** (`ui/game_over.h`):
  - [ ] Viền khung Koch Curve
- [ ] **Skill 9** (`core/draw_effects.h`):
  - [ ] Tia Xuyên Dragon Curve

### **Bước 4: Test và tối ưu**

- [ ] Test từng màn hình
- [ ] Kiểm tra FPS (phải >= 30 FPS)
- [ ] Điều chỉnh depth nếu lag
- [ ] Kiểm tra màu sắc hài hòa

---

## 📸 11. HÌNH ẢNH MINH HỌA (Mô tả)

### **Koch Curve - Menu chính**:

```
┌─────────────────────────────────────┐
│  ╱╲╱╲╱╲  ASTEROID  ╱╲╱╲╱╲          │
│  ╲╱╲╱╲╱  BLASTER   ╲╱╲╱╲╱          │
│                                     │
│  ╱╲╱╲╱╲  BẮT ĐẦU   ╱╲╱╲╱╲          │
│  ╱╲╱╲╱╲  HƯỚNG DẪN ╱╲╱╲╱╲          │
└─────────────────────────────────────┘
```

**Mô tả**: Viền Koch Curve tạo cảm giác băng giá, sắc nét

### **Dragon Curve - Tia Mặt Trời**:

```
        ╱╲╱╲
       ╱    ╲╱╲
      ╱  ☀️  ╲╱╲
     ╱╲      ╱╲╱
    ╱  ╲╱╲╱╲╱
```

**Mô tả**: Tia uốn lượn như lửa, năng động

### **C-Curve - Vành Sao Thổ**:

```
      ╱╲╱╲╱╲╱╲
     ╱  🪐    ╲
    ╱╲╱╲╱╲╱╲╱╲
```

**Mô tả**: Vành đai phức tạp, chi tiết

---

## 🚀 12. KẾT LUẬN

Sau khi hoàn thành tài liệu này, bạn sẽ có:

✅ **File `core/fractal.h`** với:

- Hàm đệ quy chính xác, có ghi chú đầy đủ
- 3 loại Fractal: Koch, Dragon, C-Curve
- Các hàm wrapper tiện ích

✅ **Fractal lồng ghép sáng tạo** vào:

- Menu chính (viền băng giá)
- 5 background levels (chi tiết phức tạp)
- Skill 9 (hiệu ứng ấn tượng)
- Game Over (cảm giác vỡ vụn)

✅ **Hình ảnh sắc nét, màu sắc hài hòa**:

- Phối màu theo bối cảnh
- Depth phù hợp (không lag)
- Hiệu ứng thị giác ấn tượng

**Chúc bạn thành công!** 🎉

---

**Tác giả**: Kiro AI Assistant  
**Phiên bản**: 2.0 (Cập nhật chi tiết)  
**Ngày**: 2026-05-12
