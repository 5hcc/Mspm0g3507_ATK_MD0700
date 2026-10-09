#ifndef __LCD_TOUCH_IIC_H__
#define __LCD_TOUCH_IIC_H__

#include "lcd.h"

#if (LCD_USING_TOUCH != 0)

/* 引脚定义 */
#define LCD_TOUCH_IIC_PORT                      GPIOA
#define LCD_TOUCH_IIC_SCL_GPIO_PIN              TOUCH_SCL_PIN
#define LCD_TOUCH_IIC_SDA_GPIO_PIN              TOUCH_SDA_PIN

/* IO操作 */
#define LCD_TOUCH_IIC_SCL(x)                 do{ x ?                                                                                                         \
                                                        DL_GPIO_setPins(LCD_TOUCH_IIC_PORT, LCD_TOUCH_IIC_SCL_GPIO_PIN) :    \
                                                        DL_GPIO_clearPins(LCD_TOUCH_IIC_PORT, LCD_TOUCH_IIC_SCL_GPIO_PIN);   \
                                                    }while(0)

#define LCD_TOUCH_IIC_SDA(x)                 do{ x ?                                                                                                         \
                                                        DL_GPIO_setPins(LCD_TOUCH_IIC_PORT, LCD_TOUCH_IIC_SDA_GPIO_PIN) :    \
                                                        DL_GPIO_clearPins(LCD_TOUCH_IIC_PORT, LCD_TOUCH_IIC_SDA_GPIO_PIN);   \
                                                    }while(0)

#define LCD_TOUCH_IIC_READ_SDA()             DL_GPIO_readPins(LCD_TOUCH_IIC_PORT, LCD_TOUCH_IIC_SDA_GPIO_PIN)

/* 控制IIC通讯是读操作还是写操作 */
#define ATK_MD0700_TOUCH_IIC_WRITE                  0
#define ATK_MD0700_TOUCH_IIC_READ                   1

/* 控制IIC通讯是读操作还是写操作 */
#define ATK_MD0430_TOUCH_IIC_WRITE                  0
#define ATK_MD0430_TOUCH_IIC_READ                   1

/* 错误代码 */
#define ATK_MD0700_TOUCH_IIC_EOK                    0   /* 没有错误 */
#define ATK_MD0700_TOUCH_IIC_ERROR                  1   /* 错误 */

/* 错误代码 */
#define ATK_MD0430_TOUCH_IIC_EOK    0   /* 没有错误 */
#define ATK_MD0430_TOUCH_IIC_ERROR  1   /* 错误 */

/* 操作函数 */
void atk_md0430_touch_iic_init(uint8_t iic_addr);                                   /* 初始化IIC接口 */
uint8_t atk_md0430_touch_iic_write_reg(uint16_t reg, uint8_t *buf, uint8_t len);    /* 写ATK-MD0430模块触摸寄存器 */
void atk_md0430_touch_iic_read_reg(uint16_t reg, uint8_t *buf, uint8_t len);        /* 读ATK-MD0430模块触摸寄存器 */
void atk_md0430_touch_iic_start(void);
void atk_md0430_touch_iic_stop(void);
uint8_t atk_md0430_touch_iic_wait_ack(void);
void atk_md0430_touch_iic_send_byte(uint8_t dat);
uint8_t atk_md0430_touch_iic_recv_byte(uint8_t ack);

#endif /* ATK_MD0430_USING_TOUCH */

#endif
