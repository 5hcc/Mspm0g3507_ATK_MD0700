#ifndef __LCD_TOUCH_H__
#define __LCD_TOUCH_H__

#include "lcd.h"

#if (LCD_USING_TOUCH != 0)

/* 引脚定义 */
#define LCD_TOUCH_PEN_GPIO_PORT                 GPIOA
#define LCD_TOUCH_PEN_GPIO_PIN                  TOUCH_PEN_PIN
#define LCD_TOUCH_TCS_GPIO_PIN                  TOUCH_TCS_PIN

/* IO操作 */
#define LCD_TOUCH_READ_PEN()             DL_GPIO_readPins(LCD_TOUCH_PEN_GPIO_PORT, LCD_TOUCH_PEN_GPIO_PIN)
#define LCD_TOUCH_PEN(x)                 do{ x ?                                                                                                 \
                                                        DL_GPIO_setPins(LCD_TOUCH_PEN_GPIO_PORT, LCD_TOUCH_PEN_GPIO_PIN) :    \
                                                        DL_GPIO_clearPins(LCD_TOUCH_PEN_GPIO_PORT, LCD_TOUCH_PEN_GPIO_PIN);   \
                                            }while(0)
#define LCD_TOUCH_TCS(x)                 do{ x ?                                                                                                 \
                                                        DL_GPIO_setPins(LCD_TOUCH_PEN_GPIO_PORT, LCD_TOUCH_TCS_GPIO_PIN) :    \
                                                        DL_GPIO_clearPins(LCD_TOUCH_PEN_GPIO_PORT, LCD_TOUCH_TCS_GPIO_PIN);   \
                                            }while(0)

/* ATK-MD0430触摸IIC通讯地址枚举 */
typedef enum
{
    ATK_MD0430_TOUCH_IIC_ADDR_14 = 0x14,    /* 0x14 */
    ATK_MD0430_TOUCH_IIC_ADDR_5D = 0x5D,    /* 0x5D */
} atk_md0430_touch_iic_addr_t;

/* 触摸IIC通讯地址枚举 */
typedef enum
{
    ATK_MD0700_TOUCH_IIC_ADDR_14 = 0x14,    /* 0x14 */
    ATK_MD0700_TOUCH_IIC_ADDR_5D = 0x5D,    /* 0x5D */
    ATK_MD0700_TOUCH_IIC_ADDR_38 = 0x38,    /* 0x38 */
} atk_md0700_touch_iic_addr_t;

/* 触摸点坐标数据结构 */
typedef struct
{
    uint16_t x;     /* 触摸点X坐标 */
    uint16_t y;     /* 触摸点Y坐标 */
    uint16_t size;  /* 触摸点大小 */
} atk_md0430_touch_point_t;

/* 触摸点坐标数据结构 */
typedef struct
{
    uint16_t x;     /* 触摸点X坐标 */
    uint16_t y;     /* 触摸点Y坐标 */
    uint16_t size;  /* 触摸点大小 */
} atk_md0700_touch_point_t;

/* 错误代码 */
#define ATK_MD0430_TOUCH_EOK                    0   /* 没有错误 */
#define ATK_MD0430_TOUCH_ERROR                  1   /* 错误 */

/* 错误代码 */
#define ATK_MD0700_TOUCH_EOK                    0   /* 没有错误 */
#define ATK_MD0700_TOUCH_ERROR                  1   /* 错误 */


/* 操作函数 */
uint8_t atk_md0700_touch_init(void);                                            /* ATK-MD0700模块触摸初始化 */
uint8_t atk_md0700_touch_scan(atk_md0700_touch_point_t *point, uint8_t cnt);    /* ATK-MD0700模块触摸扫描 */

/* 操作函数 */
uint8_t atk_md0430_touch_init(void);                                            /* ATK-MD0430模块触摸初始化 */
uint8_t atk_md0430_touch_scan(atk_md0430_touch_point_t *point, uint8_t cnt);    /* ATK-MD0430模块触摸扫描 */

#endif /* ATK_MD0430_USING_TOUCH */

#endif
