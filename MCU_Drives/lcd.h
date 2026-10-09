#ifndef __lcd_H__
#define __lcd_H__

#include "i8080ctrl.h"
#include <string.h>

/* 定义ATK-MD0430模块LCD尺寸 */
#define LCD_WIDTH            480
#define LCD_HEIGHT           800


/* 定义是否使用ATK-MD0430模块触摸屏 */
#define LCD_USING_TOUCH          1

/* 定义ATK-MD0430模块字体显示配置 */
#define ATK_MD0430_FONT_12              1
#define ATK_MD0430_FONT_16              1
#define ATK_MD0430_FONT_24              1
#define ATK_MD0430_FONT_32              1

/* 默认触摸屏配置 */
#ifndef LCD_USING_TOUCH
#define LCD_USING_TOUCH 1
#endif

/******************************************************************************************/
/* SSD1963相关配置参数(一般不用改) */

/* LCD分辨率设置 */ 
#define SSD_HOR_RESOLUTION      800     /* LCD水平分辨率 */ 
#define SSD_VER_RESOLUTION      480     /* LCD垂直分辨率 */ 

/* LCD驱动参数设置 */ 
#define SSD_HOR_PULSE_WIDTH     1       /* 水平脉宽 */ 
#define SSD_HOR_BACK_PORCH      46      /* 水平前廊 */ 
#define SSD_HOR_FRONT_PORCH     210     /* 水平后廊 */ 

#define SSD_VER_PULSE_WIDTH     1       /* 垂直脉宽 */ 
#define SSD_VER_BACK_PORCH      23      /* 垂直前廊 */ 
#define SSD_VER_FRONT_PORCH     22      /* 垂直前廊 */ 

/* 如下几个参数，自动计算 */ 
#define SSD_HT          (SSD_HOR_RESOLUTION + SSD_HOR_BACK_PORCH + SSD_HOR_FRONT_PORCH)
#define SSD_HPS         (SSD_HOR_BACK_PORCH)
#define SSD_VT          (SSD_VER_RESOLUTION + SSD_VER_BACK_PORCH + SSD_VER_FRONT_PORCH)
#define SSD_VPS         (SSD_VER_BACK_PORCH)
   
/******************************************************************************************/

/* 默认使用12点阵字体 */
#if ((ATK_MD0430_FONT_12 == 0) && (ATK_MD0430_FONT_16 == 0) && (ATK_MD0430_FONT_24 == 0) && (ATK_MD0430_FONT_32 == 0))
#undef ATK_MD0430_FONT_12
#define ATK_MD0430_FONT_12 1
#endif

/* 条件包含头文件 */
#if (LCD_USING_TOUCH != 0)
#include "lcd_touch.h"
#endif

/* 背光引脚 */
// #define ATK_MD0430_BL_GPIO_PORT         GPIOC
// #define ATK_MD0430_BL_GPIO_PIN          GPIO_PIN_10
// #define ATK_MD0430_BL_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)

/* IO操作 */
// #define ATK_MD0430_BL(x)                do{ x ?                                                                                 \
//                                             DL_GPIO_setPins(ATK_MD0430_BL_GPIO_PORT, ATK_MD0430_BL_GPIO_PIN) :  \
//                                             DL_GPIO_resetPins(ATK_MD0430_BL_GPIO_PORT, ATK_MD0430_BL_GPIO_PIN); \
//                                         }while(0)

/* ATK-MD0430模块LCD扫描方向枚举 */
typedef enum
{
    LCD_SCAN_DIR_L2R_U2D = 0x0000,   /* 从左到右，从上到下 */
    LCD_SCAN_DIR_L2R_D2U = 0x0080,   /* 从左到右，从下到上 */
    LCD_SCAN_DIR_R2L_U2D = 0x0040,   /* 从右到左，从上到下 */
    LCD_SCAN_DIR_R2L_D2U = 0x00C0,   /* 从右到左，从下到上 */
    LCD_SCAN_DIR_U2D_L2R = 0x0020,   /* 从上到下，从左到右 */
    LCD_SCAN_DIR_U2D_R2L = 0x0060,   /* 从上到下，从右到左 */
    LCD_SCAN_DIR_D2U_L2R = 0x00A0,   /* 从下到上，从左到右 */
    LCD_SCAN_DIR_D2U_R2L = 0x00E0,   /* 从下到上，从右到左 */
} atk_md0430_lcd_scan_dir_t;

/* ATK-MD0430模块LCD显示方向枚举 */
typedef enum
{
    LCD_DISP_DIR_0 = 0x00,           /* LCD顺时针旋转0度显示方向 */
    LCD_DISP_DIR_90,                 /* LCD顺时针旋转90度显示方向 */
    LCD_DISP_DIR_180,                /* LCD顺时针旋转180度显示方向 */
    LCD_DISP_DIR_270,                /* LCD顺时针旋转270度显示方向 */
} atk_md0430_lcd_disp_dir_t;

/* ATK-MD0430模块LCD显示字体枚举 */
typedef enum
{
#if (ATK_MD0430_FONT_12 != 0)
    ATK_MD0430_LCD_FONT_12,             /* 12点阵字体 */
#endif
#if (ATK_MD0430_FONT_16 != 0)
    ATK_MD0430_LCD_FONT_16,             /* 16点阵字体 */
#endif
#if (ATK_MD0430_FONT_24 != 0)
    ATK_MD0430_LCD_FONT_24,             /* 24点阵字体 */
#endif
#if (ATK_MD0430_FONT_32 != 0)
    ATK_MD0430_LCD_FONT_32,             /* 32点阵字体 */
#endif
} atk_md0430_lcd_font_t;

/* ATK-MD0430模块LCD数字显示模式枚举 */
typedef enum
{
    ATK_MD0430_NUM_SHOW_NOZERO = 0x00,  /* 数字高位为0不显示 */
    ATK_MD0430_NUM_SHOW_ZERO,           /* 数字高位为0显示 */
} atk_md0430_num_mode_t;

/* 常用颜色定义（RGB565格式） */
#define ATK_MD0430_WHITE        0xFFFF
#define ATK_MD0430_BLACK        0x0000
#define ATK_MD0430_BLUE         0x001F
#define ATK_MD0430_BRED         0XF81F
#define ATK_MD0430_GRED         0XFFE0
#define ATK_MD0430_GBLUE        0X07FF
#define ATK_MD0430_RED          0xF800
#define ATK_MD0430_MAGENTA      0xF81F
#define ATK_MD0430_GREEN        0x07E0
#define ATK_MD0430_CYAN         0x7FFF
#define ATK_MD0430_YELLOW       0xFFE0
#define ATK_MD0430_BROWN        0XBC40
#define ATK_MD0430_BRRED        0XFC07
#define ATK_MD0430_GRAY         0X8430

/* 错误码定义 */
#define LCD_EOK                 0   /* 没有错误 */
#define LCD_ERROR               1   /* 错误 */
#define LCD_EINVAL              2   /* 非法参数 */

extern uint8_t device;

/* 函数声明 */
uint8_t lcd_init(void);                                                                                                                          /* ATK-MD0430模块初始化 */
uint16_t lcd_get_width(void);                                                                                                                /* 获取ATK-MD0430模块LCD宽度 */
uint16_t lcd_get_height(void);                                                                                                               /* 获取ATK-MD0430模块LCD高度 */
void lcd_backlight_on(void);                                                                                                                     /* 开启ATK-MD0430模块LCD背光 */
void lcd_backlight_off(void);                                                                                                                    /* 关闭ATK-MD0430模块LCD背光 */
void lcd_display_on(void);                                                                                                                       /* 开启ATK-MD0430模块LCD显示 */
void lcd_display_off(void);                                                                                                                      /* 关闭ATK-MD0430模块LCD显示 */
uint8_t lcd_set_scan_dir(atk_md0430_lcd_scan_dir_t scan_dir);                                                                                    /* 设置ATK-MD0430模块LCD扫描方向 */
uint8_t lcd_set_disp_dir(atk_md0430_lcd_disp_dir_t disp_dir);                                                                                    /* 设置ATK-MD0430模块LCD显示方向 */
atk_md0430_lcd_scan_dir_t lcd_get_scan_dir(void);                                                                                                /* 获取ATK-MD0430模块LCD扫描方向 */
atk_md0430_lcd_disp_dir_t lcd_get_disp_dir(void);                                                                                                /* 获取ATK-MD0430模块LCD显示方向 */
void lcd_fill(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color);                                                                      /* ATK-MD0430模块LCD填充矩形 */
void lcd_clear(uint16_t color);                                                                                                                  /* ATK-MD0430模块LCD清屏 */
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color);                                                                                     /* ATK-MD0430模块LCD画点 */
uint16_t lcd_read_point(uint16_t x, uint16_t y);                                                                                                 /* ATK-MD0430模块LCD读点 */
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);                                                          /* ATK-MD0430模块LCD画线 */
void lcd_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);                                                          /* ATK-MD0430模块LCD画矩形框 */
void lcd_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);                                                                        /* ATK-MD0430模块LCD画圆框 */
void lcd_show_char(uint16_t x, uint16_t y, char ch, atk_md0430_lcd_font_t font, uint16_t color);                                                 /* ATK-MD0430模块LCD显示1个字符 */
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *str, atk_md0430_lcd_font_t font, uint16_t color);            /* ATK-MD0430模块LCD显示字符串 */
void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, atk_md0430_num_mode_t mode, atk_md0430_lcd_font_t font, uint16_t color);   /* ATK-MD0430模块LCD显示数字，可选择是否显示高位0 */
void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, atk_md0430_lcd_font_t font, uint16_t color);                                /* ATK-MD0430模块LCD显示数字，不显示高位0 */
void lcd_show_pic(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *pic);                                                        /* ATK-MD0430模块LCD显示图片 */

void show_string(uint16_t x, uint16_t y,char *str, atk_md0430_lcd_font_t font, uint16_t color);
#endif