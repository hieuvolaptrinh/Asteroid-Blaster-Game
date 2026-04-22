#ifndef COLOR_H
#define COLOR_H

/* ====================================================================== */
/*                    ASTEROID BLASTER – COLOR PALETTE                     */
/*                                                                         */
/*  He thong mau RGB thong nhat cho toan bo du an.                         */
/*  Su dung macro COLOR(r, g, b) thay cho cac mau mac dinh cua BGI.       */
/*  Chia theo nhom: Space, Stars, Asteroid, Ship, Effects, UI, Planets...  */
/* ====================================================================== */

/* ====================================================================== */
/*                         SPACE BACKGROUND                                */
/* ====================================================================== */
#define COL_SPACE_BG        COLOR(6, 6, 18)       /* Nen khong gian toi nhat       */
#define COL_SPACE_DEEP      COLOR(10, 10, 30)     /* Nen sau                       */
#define COL_SPACE_VOID      COLOR(15, 12, 40)     /* Tim dam (void)                */
#define COL_SPACE_MID       COLOR(20, 18, 50)     /* Trung gian                    */
#define COL_SPACE_DUST      COLOR(30, 28, 55)     /* Bui khong gian                */

/* ====================================================================== */
/*                         STARS (Ngoi sao nen)                            */
/* ====================================================================== */
#define COL_STAR_DIM        COLOR(70, 70, 90)     /* Sao toi nhat (nhap nhay pha 1)*/
#define COL_STAR_MED        COLOR(140, 145, 165)  /* Sao trung binh                */
#define COL_STAR_BRIGHT     COLOR(210, 215, 230)  /* Sao sang                      */
#define COL_STAR_GLOW       COLOR(245, 248, 255)  /* Sao sang nhat + glow          */
#define COL_STAR_WARM       COLOR(255, 230, 180)  /* Sao am (vang nhat)            */
#define COL_STAR_BLUE       COLOR(160, 200, 255)  /* Sao xanh                      */

/* ====================================================================== */
/*                    ASTEROID – SMALL (Ban kinh <= 22)                    */
/* ====================================================================== */
#define COL_AST_S_DARK      COLOR(75, 70, 65)     /* Lop toi (shadow body)         */
#define COL_AST_S_MID       COLOR(120, 115, 105)  /* Lop giua                      */
#define COL_AST_S_FILL      COLOR(165, 160, 150)  /* Lop sang (highlight zone)     */
#define COL_AST_S_LINE      COLOR(200, 195, 185)  /* Duong vien ngoai              */
#define COL_AST_S_HL        COLOR(230, 225, 215)  /* Diem sang nhat                */
#define COL_AST_S_CRATER    COLOR(55, 50, 48)     /* Mau mieng nui lua             */

/* ====================================================================== */
/*                    ASTEROID – MEDIUM (Ban kinh 23-35)                   */
/* ====================================================================== */
#define COL_AST_M_DARK      COLOR(130, 45, 25)    /* Lop toi (do tham)             */
#define COL_AST_M_MID       COLOR(180, 80, 40)    /* Lop giua                      */
#define COL_AST_M_FILL      COLOR(220, 120, 55)   /* Lop sang (cam)                */
#define COL_AST_M_LINE      COLOR(255, 175, 60)   /* Vien ngoai (vang cam)         */
#define COL_AST_M_HL        COLOR(255, 220, 120)  /* Diem sang nhat                */
#define COL_AST_M_CRATER    COLOR(100, 30, 15)    /* Mieng nui lua                 */

/* ====================================================================== */
/*                    ASTEROID – LARGE (Ban kinh > 35)                     */
/* ====================================================================== */
#define COL_AST_L_DARK      COLOR(110, 75, 20)    /* Lop toi (nau dam)             */
#define COL_AST_L_MID       COLOR(170, 135, 45)   /* Lop giua                      */
#define COL_AST_L_FILL      COLOR(215, 175, 55)   /* Lop sang (vang dat)           */
#define COL_AST_L_LINE      COLOR(255, 140, 65)   /* Vien ngoai                    */
#define COL_AST_L_HL        COLOR(255, 235, 195)  /* Diem sang nhat                */
#define COL_AST_L_CRATER    COLOR(85, 55, 12)     /* Mieng nui lua                 */

/* ====================================================================== */
/*                    ASTEROID – SHADOW & HP BAR                           */
/* ====================================================================== */
#define COL_AST_SHADOW      COLOR(25, 22, 30)     /* Bong do asteroid              */
#define COL_HP_HIGH         COLOR(50, 210, 80)    /* HP > 60%: xanh la             */
#define COL_HP_MED          COLOR(245, 200, 40)   /* HP 30-60%: vang               */
#define COL_HP_LOW          COLOR(215, 45, 40)    /* HP < 30%: do                  */
#define COL_HP_BORDER       COLOR(80, 80, 100)    /* Vien thanh HP                 */
#define COL_HP_BG           COLOR(30, 30, 45)     /* Nen thanh HP                  */

/* ====================================================================== */
/*                         SHIP (Phi thuyen)                               */
/* ====================================================================== */
#define COL_SHIP_BODY       COLOR(25, 65, 165)    /* Than chinh – xanh dam         */
#define COL_SHIP_HL1        COLOR(50, 140, 215)   /* Highlight lop 1 – xanh nhat   */
#define COL_SHIP_HL2        COLOR(110, 210, 255)  /* Highlight lop 2 – cyan sang   */
#define COL_SHIP_HL3        COLOR(180, 235, 255)  /* Highlight nho nhat            */
#define COL_SHIP_WING       COLOR(20, 50, 130)    /* Canh – xanh dam hon           */
#define COL_SHIP_WING_HL    COLOR(65, 135, 210)   /* Highlight canh                */
#define COL_SHIP_EDGE       COLOR(140, 210, 255)  /* Vien sang                     */
#define COL_SHIP_EDGE2      COLOR(200, 235, 255)  /* Vien sang nhat                */
#define COL_SHIP_SHADOW     COLOR(12, 20, 50)     /* Bong phi thuyen               */
#define COL_SHIP_COCKPIT    COLOR(100, 190, 255)  /* Kinh lai – xanh sang          */
#define COL_SHIP_COCKPIT_HL COLOR(220, 245, 255)  /* Anh sang kinh lai             */

/* ====================================================================== */
/*                    SHIP AURA (Khien bao ve)                             */
/* ====================================================================== */
#define COL_AURA_OUTER      COLOR(30, 60, 180)    /* Vong ngoai                    */
#define COL_AURA_INNER      COLOR(80, 200, 255)   /* Vong trong                    */
#define COL_AURA_ALT        COLOR(60, 180, 230)   /* Xen ke (nhap nhay)            */

/* ====================================================================== */
/*                    ENGINE FLAME (Lua dong co)                           */
/* ====================================================================== */
#define COL_FLAME_OUTER     COLOR(220, 50, 25)    /* Lop ngoai – do dam            */
#define COL_FLAME_OUTER_ALT COLOR(255, 80, 20)    /* Lop ngoai – cam do (xen ke)   */
#define COL_FLAME_MID       COLOR(255, 200, 50)   /* Lop giua – vang               */
#define COL_FLAME_CORE      COLOR(255, 250, 220)  /* Loi – trang am                */
#define COL_FLAME_SPARK     COLOR(255, 180, 80)   /* Tia lua                       */

/* ====================================================================== */
/*                         CROSSHAIR (Tam ngam)                            */
/* ====================================================================== */
#define COL_CROSS_OUTER     COLOR(30, 180, 60)    /* Vong ngoai – xanh la          */
#define COL_CROSS_INNER     COLOR(80, 240, 100)   /* Vong trong – xanh sang        */
#define COL_CROSS_DOT       COLOR(255, 255, 255)  /* Tam diem – trang              */
#define COL_AIM_LINE        COLOR(60, 60, 80)     /* Duong ngam – xam              */

/* ====================================================================== */
/*                    BULLET (Dan)                                         */
/* ====================================================================== */
#define COL_BULLET_GLOW     COLOR(255, 80, 30)    /* Hao quang – cam do            */
#define COL_BULLET_FILL     COLOR(255, 210, 60)   /* Than dan – vang               */
#define COL_BULLET_CORE     COLOR(255, 250, 230)  /* Loi – trang am                */
#define COL_BULLET_TRAIL    COLOR(255, 140, 40)   /* Vet di                        */

/* ====================================================================== */
/*                    EXPLOSION (Hieu ung no)                              */
/* ====================================================================== */
#define COL_EXPL_1          COLOR(255, 255, 240)  /* Pha 1: loi sang trang         */
#define COL_EXPL_2          COLOR(255, 220, 80)   /* Pha 2: vang sang              */
#define COL_EXPL_3          COLOR(255, 140, 40)   /* Pha 3: cam                    */
#define COL_EXPL_4          COLOR(200, 55, 20)    /* Pha 4: do dam                 */
#define COL_EXPL_5          COLOR(80, 70, 65)     /* Pha 5: khoi xam               */
#define COL_EXPL_GLOW       COLOR(255, 100, 30)   /* Vong glow ngoai cung          */
#define COL_EXPL_DEBRIS     COLOR(255, 180, 60)   /* Manh vo                       */
#define COL_EXPL_DEBRIS_DIM COLOR(120, 100, 80)   /* Manh vo pha cuoi              */
#define COL_EXPL_SPARK      COLOR(255, 255, 200)  /* Hat lua                       */

/* ====================================================================== */
/*                    ITEM / CRYSTAL (Tinh the nang cap)                   */
/* ====================================================================== */
#define COL_CRYSTAL_A       COLOR(80, 235, 255)   /* Mau A: cyan sang              */
#define COL_CRYSTAL_B       COLOR(240, 100, 255)  /* Mau B: tim hong               */
#define COL_CRYSTAL_EDGE    COLOR(230, 240, 255)  /* Vien – trang xanh             */
#define COL_CRYSTAL_BOLT    COLOR(255, 230, 80)   /* Bieu tuong set – vang         */
#define COL_CRYSTAL_SPARK_A COLOR(255, 255, 255)  /* Sparkle A – trang             */
#define COL_CRYSTAL_SPARK_B COLOR(255, 240, 100)  /* Sparkle B – vang              */
#define COL_CRYSTAL_GLOW    COLOR(160, 220, 255)  /* Hao quang xung quanh          */

/* ====================================================================== */
/*                    SKILL EFFECTS                                        */
/* ====================================================================== */
/* Bomb Burst */
#define COL_BOMB_HOT        COLOR(255, 220, 60)   /* Pha nong – vang               */
#define COL_BOMB_MID        COLOR(255, 100, 30)   /* Pha giua – cam do             */
#define COL_BOMB_COOL       COLOR(200, 40, 20)    /* Pha mat – do tham             */
#define COL_BOMB_RING       COLOR(255, 160, 40)   /* Vong song no                  */

/* Piercing Beam */
#define COL_BEAM_OUTER      COLOR(80, 220, 255)   /* Lop ngoai – cyan              */
#define COL_BEAM_INNER      COLOR(200, 245, 255)  /* Lop trong – trang xanh        */
#define COL_BEAM_GLOW       COLOR(40, 160, 255)   /* Hao quang                     */

/* Time Slow */
#define COL_SLOW_TEXT       COLOR(100, 180, 255)  /* Text thong bao                */
#define COL_SLOW_GLOW       COLOR(60, 140, 220)   /* Hieu ung xanh                 */

/* ====================================================================== */
/*                         UI – TEXT                                        */
/* ====================================================================== */
#define COL_TEXT_TITLE       COLOR(255, 225, 70)   /* Tieu de – vang am             */
#define COL_TEXT_SUBTITLE    COLOR(100, 215, 255)  /* Phu de – cyan                 */
#define COL_TEXT_BODY        COLOR(225, 230, 240)  /* Noi dung – trang xam          */
#define COL_TEXT_DIM         COLOR(90, 95, 115)    /* Text mo – xam                 */
#define COL_TEXT_ACCENT      COLOR(255, 180, 50)   /* Nhan manh – cam vang          */
#define COL_TEXT_DANGER      COLOR(255, 70, 60)    /* Nguy hiem – do                */
#define COL_TEXT_SUCCESS     COLOR(80, 230, 110)   /* Thanh cong – xanh la          */
#define COL_TEXT_WHITE       COLOR(245, 248, 255)  /* Trang tinh khiet              */

/* ====================================================================== */
/*                         UI – BUTTONS                                    */
/* ====================================================================== */
#define COL_BTN_NORMAL      COLOR(30, 35, 55)     /* Nen nut binh thuong           */
#define COL_BTN_BORDER      COLOR(80, 90, 120)    /* Vien nut                      */
#define COL_BTN_HOVER_GREEN COLOR(40, 155, 65)    /* Hover xanh la                 */
#define COL_BTN_HOVER_BLUE  COLOR(30, 80, 180)    /* Hover xanh duong              */
#define COL_BTN_HOVER_RED   COLOR(175, 40, 40)    /* Hover do                      */
#define COL_BTN_HOVER_CYAN  COLOR(30, 150, 180)   /* Hover cyan                    */
#define COL_BTN_TEXT         COLOR(230, 235, 245)  /* Text nut                      */
#define COL_BTN_TEXT_HOV     COLOR(255, 230, 80)   /* Text nut khi hover            */

/* ====================================================================== */
/*                    UI – PANEL / OVERLAY                                  */
/* ====================================================================== */
#define COL_PANEL_BG        COLOR(12, 14, 28)     /* Nen panel                     */
#define COL_PANEL_BORDER    COLOR(80, 180, 255)   /* Vien panel – xanh sang        */
#define COL_PANEL_BORDER2   COLOR(50, 120, 200)   /* Vien panel ngoai              */
#define COL_OVERLAY_DIM     COLOR(4, 4, 12)       /* Lop mo khi pause              */

/* ====================================================================== */
/*                    UI – GAME OVER                                       */
/* ====================================================================== */
#define COL_GAMEOVER_TITLE  COLOR(220, 40, 40)    /* Tieu de KET THUC – do         */
#define COL_GAMEOVER_BORDER COLOR(180, 30, 30)    /* Vien khung – do dam           */

/* ====================================================================== */
/*                    UI – LEVEL BANNER                                    */
/* ====================================================================== */
#define COL_BANNER_BG       COLOR(20, 40, 120)    /* Nen banner                    */
#define COL_BANNER_BORDER   COLOR(100, 200, 255)  /* Vien banner                   */

/* ====================================================================== */
/*                    UI – HUD (Menu button)                               */
/* ====================================================================== */
#define COL_MENU_BTN_BG     COLOR(20, 22, 35)     /* Nen nut menu                  */
#define COL_MENU_BTN_HOVER  COLOR(40, 45, 65)     /* Nen nut menu hover            */
#define COL_MENU_BTN_LINE   COLOR(180, 190, 210)  /* Gach menu – xam sang          */
#define COL_MENU_BTN_LINE_H COLOR(255, 225, 70)   /* Gach menu hover – vang        */

/* ====================================================================== */
/*              PLANETS & BACKGROUND OBJECTS                               */
/* ====================================================================== */
/* Shadow chung */
#define COL_SHADOW          COLOR(18, 16, 25)     /* Bong do chung                 */
#define COL_SHADOW_LIGHT    COLOR(40, 38, 50)     /* Bong nhe                      */

/* Mat Troi (Level 2) */
#define COL_SUN_CORONA_1    COLOR(60, 20, 10)     /* Corona ngoai cung             */
#define COL_SUN_CORONA_2    COLOR(180, 40, 15)    /* Corona giua                   */
#define COL_SUN_CORONA_3    COLOR(255, 100, 30)   /* Corona gần                    */
#define COL_SUN_CORONA_4    COLOR(255, 200, 60)   /* Corona sat                    */
#define COL_SUN_CORONA_5    COLOR(255, 245, 200)  /* Corona trong cung             */
#define COL_SUN_SURFACE_1   COLOR(200, 50, 15)    /* Be mat ngoai                  */
#define COL_SUN_SURFACE_2   COLOR(255, 100, 30)   /* Be mat giua                   */
#define COL_SUN_SURFACE_3   COLOR(255, 200, 60)   /* Be mat trong                  */
#define COL_SUN_CORE        COLOR(255, 248, 220)  /* Loi Mat Troi                  */
#define COL_SUN_FLARE_A     COLOR(255, 200, 60)   /* Tia sang A                    */
#define COL_SUN_FLARE_B     COLOR(255, 110, 40)   /* Tia sang B                    */

/* Hanh tinh chung */
#define COL_PLANET_GRAY     COLOR(160, 165, 170)  /* Hanh tinh xam                 */
#define COL_PLANET_GOLD     COLOR(230, 200, 70)   /* Hanh tinh vang                */
#define COL_PLANET_TEAL     COLOR(60, 200, 200)   /* Hanh tinh cyan                */
#define COL_PLANET_CORAL    COLOR(220, 100, 70)   /* Hanh tinh cam do              */
#define COL_PLANET_BROWN    COLOR(140, 100, 50)   /* Hanh tinh nau                 */
#define COL_PLANET_HL       COLOR(240, 245, 255)  /* Highlight 3D hanh tinh        */

/* Mat Trang */
#define COL_MOON_SURFACE    COLOR(175, 180, 185)  /* Be mat Mat Trang              */
#define COL_MOON_CRATER     COLOR(110, 112, 118)  /* Mieng nui lua                 */
#define COL_MOON_SHADOW     COLOR(60, 62, 70)     /* Bong Mat Trang                */

/* Trai Dat (Level 4) */
#define COL_EARTH_OCEAN     COLOR(30, 80, 180)    /* Dai duong                     */
#define COL_EARTH_LAND      COLOR(50, 160, 60)    /* Luc dia                       */
#define COL_EARTH_CLOUD     COLOR(235, 240, 255)  /* May                           */
#define COL_EARTH_SHADOW    COLOR(15, 30, 60)     /* Bong Trai Dat                 */

/* Sao Tho (Level 3) */
#define COL_SATURN_BODY_1   COLOR(160, 120, 50)   /* Than ngoai                    */
#define COL_SATURN_BODY_2   COLOR(220, 185, 70)   /* Than giua                     */
#define COL_SATURN_BODY_3   COLOR(240, 110, 60)   /* Vung nong                     */
#define COL_SATURN_HL       COLOR(255, 245, 210)  /* Highlight                     */
#define COL_SATURN_RING_DK  COLOR(120, 110, 90)   /* Vanh dai toi                  */
#define COL_SATURN_RING_LT  COLOR(200, 195, 175)  /* Vanh dai sang                 */
#define COL_SATURN_RING_BR  COLOR(240, 235, 220)  /* Vanh dai sang nhat            */

/* Sao Hoa (Level 5) */
#define COL_MARS_BODY_1     COLOR(150, 90, 30)    /* Than ngoai                    */
#define COL_MARS_BODY_2     COLOR(220, 160, 50)   /* Than giua                     */
#define COL_MARS_BODY_3     COLOR(240, 100, 50)   /* Vung nong                     */
#define COL_MARS_HL         COLOR(255, 240, 210)  /* Highlight                     */

/* UFO */
#define COL_UFO_BODY        COLOR(170, 180, 195)  /* Than UFO                      */
#define COL_UFO_CABIN       COLOR(70, 210, 220)   /* Cabin UFO                     */
#define COL_UFO_CABIN_HL    COLOR(150, 240, 255)  /* Cabin highlight               */
#define COL_UFO_LIGHT       COLOR(255, 220, 60)   /* Den UFO                       */
#define COL_UFO_BEAM        COLOR(120, 230, 255)  /* Tia sang UFO                  */
#define COL_UFO_WARNING     COLOR(255, 50, 40)    /* Den canh bao                  */

/* Tram Vu Tru (Level 4) */
#define COL_STATION_BODY    COLOR(100, 105, 115)  /* Than tram                     */
#define COL_STATION_PANEL   COLOR(60, 190, 200)   /* Tam nang luong                */
#define COL_STATION_BORDER  COLOR(160, 170, 185)  /* Vien tram                     */
#define COL_STATION_LIGHT   COLOR(255, 220, 60)   /* Den tram                      */

/* ====================================================================== */
/*                    NEBULA (Tinh van)                                    */
/* ====================================================================== */
#define COL_NEBULA_PURPLE   COLOR(140, 50, 180)   /* Tim                           */
#define COL_NEBULA_CYAN     COLOR(50, 180, 200)   /* Xanh cyan                     */
#define COL_NEBULA_BLUE     COLOR(40, 60, 180)    /* Xanh dam                      */
#define COL_NEBULA_PINK     COLOR(220, 100, 200)  /* Hong                          */
#define COL_NEBULA_LTCYAN   COLOR(130, 220, 255)  /* Cyan nhat                     */
#define COL_NEBULA_LTBLUE   COLOR(100, 140, 240)  /* Xanh nhat                     */
#define COL_NEBULA_DIM      COLOR(50, 45, 65)     /* Tinh van mo                   */

/* ====================================================================== */
/*                    BLACK HOLE (Lo den – Level 6)                        */
/* ====================================================================== */
#define COL_BH_EVENT        COLOR(30, 28, 40)     /* Event horizon                 */
#define COL_BH_DISK_1       COLOR(35, 50, 170)    /* Dia tich tu – xanh dam        */
#define COL_BH_DISK_2       COLOR(50, 175, 200)   /* Dia – cyan                    */
#define COL_BH_DISK_3       COLOR(120, 220, 255)  /* Dia – cyan sang               */
#define COL_BH_DISK_4       COLOR(160, 60, 180)   /* Dia – tim                     */
#define COL_BH_DISK_5       COLOR(220, 90, 200)   /* Dia – hong tim                */
#define COL_BH_DISK_6       COLOR(200, 50, 30)    /* Dia – do                      */
#define COL_BH_DISK_7       COLOR(60, 55, 70)     /* Dia – toi                     */
#define COL_BH_HALO         COLOR(45, 42, 60)     /* Hao quang bao quanh           */

/* ====================================================================== */
/*                    GALAXY SPIRAL (Giai ngan ha – Level 5)               */
/* ====================================================================== */
#define COL_GALAXY_CORE     COLOR(240, 245, 255)  /* Tam giai ngan ha              */
#define COL_GALAXY_INNER    COLOR(220, 235, 255)  /* Vong trong                    */
#define COL_GALAXY_MID      COLOR(140, 210, 255)  /* Vong giua                     */
#define COL_GALAXY_OUTER    COLOR(70, 160, 220)   /* Vong ngoai                    */

/* ====================================================================== */
/*                    SHOOTING STAR (Sao bang)                             */
/* ====================================================================== */
#define COL_SHOOTING_HEAD   COLOR(240, 245, 255)  /* Dau sao bang – trang          */
#define COL_SHOOTING_MID    COLOR(255, 225, 100)  /* Than – vang                   */
#define COL_SHOOTING_TAIL   COLOR(100, 200, 255)  /* Duoi – xanh                   */

/* ====================================================================== */
/*               ORBIT / MISC LINES                                       */
/* ====================================================================== */
#define COL_ORBIT_LINE      COLOR(45, 45, 65)     /* Duong quy dao – xam toi       */
#define COL_INFINITY_OUTER  COLOR(60, 190, 210)   /* Vo cuc – vien ngoai           */
#define COL_INFINITY_INNER  COLOR(120, 225, 255)  /* Vo cuc – vien trong           */

/* ====================================================================== */
/*           GRADIENT HELPERS – Pre-computed intermediate colors           */
/* ====================================================================== */
/* Fire gradient (8 buoc: trang -> vang -> cam -> do -> toi) */
#define COL_FIRE_0          COLOR(255, 255, 245)  /* Trang nong                    */
#define COL_FIRE_1          COLOR(255, 240, 160)  /* Vang nhat                     */
#define COL_FIRE_2          COLOR(255, 210, 80)   /* Vang dam                      */
#define COL_FIRE_3          COLOR(255, 170, 40)   /* Cam vang                      */
#define COL_FIRE_4          COLOR(255, 120, 25)   /* Cam do                        */
#define COL_FIRE_5          COLOR(230, 70, 15)    /* Do cam                        */
#define COL_FIRE_6          COLOR(180, 35, 10)    /* Do tham                       */
#define COL_FIRE_7          COLOR(100, 20, 8)     /* Do toi                        */

/* Cool gradient (4 buoc: trang -> cyan -> xanh) */
#define COL_COOL_0          COLOR(230, 245, 255)  /* Trang xanh                    */
#define COL_COOL_1          COLOR(140, 215, 255)  /* Cyan nhat                     */
#define COL_COOL_2          COLOR(60, 160, 230)   /* Xanh trung                    */
#define COL_COOL_3          COLOR(25, 80, 170)    /* Xanh dam                      */

/* Smoke gradient (4 buoc) */
#define COL_SMOKE_0         COLOR(140, 135, 125)  /* Khoi sang                     */
#define COL_SMOKE_1         COLOR(100, 95, 90)    /* Khoi trung                    */
#define COL_SMOKE_2         COLOR(65, 60, 55)     /* Khoi toi                      */
#define COL_SMOKE_3         COLOR(35, 32, 30)     /* Khoi den                      */

/* ====================================================================== */
/*               UTILITY: Blend (Lerp) hai mau tai runtime                 */
/* ====================================================================== */
/*
 * colorLerp(): Tinh mau trung gian giua c1 va c2 theo he so t (0.0 - 1.0).
 *   t=0.0 -> tra ve c1, t=1.0 -> tra ve c2.
 *   Hoat dong voi gia tri COLOR(r,g,b) tu graphics.h
 */
static int colorLerp(int c1, int c2, float t)
{
  int r1 = (c1 >> 16) & 0xFF, g1 = (c1 >> 8) & 0xFF, b1 = c1 & 0xFF;
  int r2 = (c2 >> 16) & 0xFF, g2 = (c2 >> 8) & 0xFF, b2 = c2 & 0xFF;
  int r = r1 + (int)((r2 - r1) * t);
  int g = g1 + (int)((g2 - g1) * t);
  int b = b1 + (int)((b2 - b1) * t);
  if (r < 0) r = 0; if (r > 255) r = 255;
  if (g < 0) g = 0; if (g > 255) g = 255;
  if (b < 0) b = 0; if (b > 255) b = 255;
  return COLOR(r, g, b);
}

#endif
