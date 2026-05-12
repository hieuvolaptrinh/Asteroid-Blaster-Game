#ifndef SNOW_DOWN_H
#define SNOW_DOWN_H

/* ====================================================================== */
/*                    SNOW SYSTEM MANAGER                                 */
/*                    ASTEROID BLASTER v3.0                               */
/*                                                                        */
/*  Hệ thống quản lý tuyết rơi toàn cục                                  */
/*  - Quản lý mảng bông tuyết                                            */
/*  - Cập nhật vị trí, xoay, fade, tan                                   */
/*  - Render tất cả bông tuyết                                           */
/*  - Hiển thị trên toàn bộ app                                          */
/* ====================================================================== */

#define MAX_SNOWFLAKES 80 /* Số bông tuyết tối đa (tăng lên) */

/* Mảng toàn cục lưu các bông tuyết */
static SnowParticle snowflakes[MAX_SNOWFLAKES];
static int snowInitialized = 0;

/**
 * initSnowSystem(): Khởi tạo hệ thống tuyết rơi
 * 
 * Tạo MAX_SNOWFLAKES bông tuyết với:
 * - Vị trí ngẫu nhiên trên màn hình
 * - Kích thước ngẫu nhiên (5-20 pixel) - TO HƠN
 * - Tốc độ rơi ngẫu nhiên - NHANH HƠN
 * - Depth Koch Curve ngẫu nhiên (1-2)
 * - Màu sắc đa dạng
 * - Một số bông có hiệu ứng tan
 */
void initSnowSystem(void)
{
  int i;
  for (i = 0; i < MAX_SNOWFLAKES; i++)
  {
    snowflakes[i].x = (float)(rand() % WIDTH);
    snowflakes[i].y = (float)(-(rand() % HEIGHT)); /* Bắt đầu từ trên */
    snowflakes[i].vx = ((rand() % 20) - 10) / 10.0f; /* Gió: -1.0 đến 1.0 */
    snowflakes[i].vy = ((rand() % 30) + 20) / 10.0f; /* Rơi NHANH HƠN: 2.0 đến 5.0 */
    snowflakes[i].size = (rand() % 16) + 5;          /* Kích thước TO HƠN: 5-20 */
    snowflakes[i].depth = (rand() % 2) + 1;          /* Depth: 1-2 (tối ưu) */
    snowflakes[i].rotation = 0.0f;
    snowflakes[i].rotSpeed = ((rand() % 20) - 10) / 100.0f; /* Xoay: -0.1 đến 0.1 */
    snowflakes[i].alpha = 255; /* Bắt đầu hoàn toàn hiển thị */
    snowflakes[i].fadeSpeed = (rand() % 3) + 1;
    
    /* Màu sắc đa dạng */
    snowflakes[i].color = getSnowColor(rand() % 5);
    
    /* 20% bông tuyết có hiệu ứng tan */
    if (rand() % 5 == 0)
    {
      snowflakes[i].melting = 1;
      snowflakes[i].meltScale = 1.0f;
    }
    else
    {
      snowflakes[i].melting = 0;
      snowflakes[i].meltScale = 1.0f;
    }
    
    snowflakes[i].active = 1;
  }
  snowInitialized = 1;
}

/**
 * updateSnowflakes(): Cập nhật vị trí và trạng thái các bông tuyết
 * 
 * - Di chuyển bông tuyết theo vận tốc
 * - Xoay bông tuyết
 * - Fade out khi gần đáy
 * - Hiệu ứng tan (thu nhỏ dần)
 * - Reset bông tuyết khi ra khỏi màn hình
 */
void updateSnowflakes(void)
{
  int i;
  for (i = 0; i < MAX_SNOWFLAKES; i++)
  {
    if (!snowflakes[i].active)
      continue;

    /* Cập nhật vị trí */
    snowflakes[i].x += snowflakes[i].vx;
    snowflakes[i].y += snowflakes[i].vy;

    /* Cập nhật góc xoay */
    snowflakes[i].rotation += snowflakes[i].rotSpeed;
    if (snowflakes[i].rotation > 2.0f * (float)M_PI)
      snowflakes[i].rotation -= 2.0f * (float)M_PI;
    if (snowflakes[i].rotation < 0.0f)
      snowflakes[i].rotation += 2.0f * (float)M_PI;

    /* Hiệu ứng tan: Thu nhỏ dần */
    if (snowflakes[i].melting)
    {
      snowflakes[i].meltScale -= 0.01f; /* Thu nhỏ dần */
      snowflakes[i].alpha -= 3; /* Fade nhanh hơn */
      
      if (snowflakes[i].meltScale <= 0.0f || snowflakes[i].alpha <= 0)
      {
        /* Reset bông tuyết mới */
        snowflakes[i].x = (float)(rand() % WIDTH);
        snowflakes[i].y = -50.0f;
        snowflakes[i].vx = ((rand() % 20) - 10) / 10.0f;
        snowflakes[i].vy = ((rand() % 30) + 20) / 10.0f;
        snowflakes[i].size = (rand() % 16) + 5;
        snowflakes[i].depth = (rand() % 2) + 1;
        snowflakes[i].rotSpeed = ((rand() % 20) - 10) / 100.0f;
        snowflakes[i].alpha = 255;
        snowflakes[i].color = getSnowColor(rand() % 5);
        snowflakes[i].meltScale = 1.0f;
        
        /* 20% bông tuyết mới có hiệu ứng tan */
        snowflakes[i].melting = (rand() % 5 == 0) ? 1 : 0;
      }
    }
    else
    {
      /* Hiệu ứng fade out khi gần đáy (không tan) */
      if (snowflakes[i].y > HEIGHT - 150)
      {
        snowflakes[i].alpha -= snowflakes[i].fadeSpeed * 2;
        if (snowflakes[i].alpha < 0)
          snowflakes[i].alpha = 0;
      }
    }

    /* Reset bông tuyết khi ra khỏi màn hình */
    if (snowflakes[i].y > HEIGHT + 50 || (snowflakes[i].alpha <= 0 && !snowflakes[i].melting))
    {
      /* Xuất hiện lại ở trên */
      snowflakes[i].x = (float)(rand() % WIDTH);
      snowflakes[i].y = -50.0f;
      snowflakes[i].vx = ((rand() % 20) - 10) / 10.0f;
      snowflakes[i].vy = ((rand() % 30) + 20) / 10.0f;
      snowflakes[i].size = (rand() % 16) + 5;
      snowflakes[i].depth = (rand() % 2) + 1;
      snowflakes[i].rotSpeed = ((rand() % 20) - 10) / 100.0f;
      snowflakes[i].alpha = 255;
      snowflakes[i].color = getSnowColor(rand() % 5);
      snowflakes[i].meltScale = 1.0f;
      snowflakes[i].melting = (rand() % 5 == 0) ? 1 : 0;
    }
    else if (snowflakes[i].x < -50 || snowflakes[i].x > WIDTH + 50)
    {
      /* Ra khỏi màn hình theo chiều ngang */
      snowflakes[i].x = (float)(rand() % WIDTH);
      snowflakes[i].y = -50.0f;
      snowflakes[i].alpha = 255;
      snowflakes[i].meltScale = 1.0f;
    }
  }
}

/**
 * drawSnowflakes(): Vẽ tất cả bông tuyết
 * 
 * - Vẽ theo thứ tự: xa (nhỏ) trước, gần (lớn) sau
 * - Tạo hiệu ứng chiều sâu
 */
void drawSnowflakes(void)
{
  int i;
  
  /* Pass 1: Vẽ bông tuyết nhỏ (xa) */
  for (i = 0; i < MAX_SNOWFLAKES; i++)
  {
    if (!snowflakes[i].active || snowflakes[i].alpha <= 0)
      continue;

    if (snowflakes[i].size >= 12)
      continue; /* Bỏ qua bông lớn */

    drawSnowWithShadow((int)snowflakes[i].x, (int)snowflakes[i].y,
                       snowflakes[i].size, snowflakes[i].depth,
                       snowflakes[i].rotation, snowflakes[i].alpha,
                       snowflakes[i].color, snowflakes[i].meltScale);
  }

  /* Pass 2: Vẹ bông tuyết lớn (gần) */
  for (i = 0; i < MAX_SNOWFLAKES; i++)
  {
    if (!snowflakes[i].active || snowflakes[i].alpha <= 0)
      continue;

    if (snowflakes[i].size < 12)
      continue; /* Bỏ qua bông nhỏ */

    drawSnowWithShadow((int)snowflakes[i].x, (int)snowflakes[i].y,
                       snowflakes[i].size, snowflakes[i].depth,
                       snowflakes[i].rotation, snowflakes[i].alpha,
                       snowflakes[i].color, snowflakes[i].meltScale);
  }
}

/**
 * renderGlobalSnowEffect(): Hàm chính render tuyết toàn cục
 * 
 * Gọi hàm này trong TOÀN BỘ game loop (menu, game, pause, game over)
 * 
 * @param enable: 1 = bật tuyết rơi, 0 = tắt
 */
void renderGlobalSnowEffect(int enable)
{
  if (!enable)
    return;

  /* Khởi tạo lần đầu */
  if (!snowInitialized)
    initSnowSystem();

  /* Cập nhật và vẽ */
  updateSnowflakes();
  drawSnowflakes();
}

/**
 * resetSnowSystem(): Reset hệ thống tuyết (dùng khi cần)
 */
void resetSnowSystem(void)
{
  snowInitialized = 0;
}

#endif /* SNOW_DOWN_H */
