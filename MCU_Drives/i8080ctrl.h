#ifndef __I8080CTRL_H__
#define __I8080CTRL_H__

#include "ti_msp_dl_config.h"

#define delay_us(us) delay_cycles(CPUCLK_FREQ / 1000000 * (us))
#define delay_ms(ms) delay_cycles(CPUCLK_FREQ / 1000 * (ms))

#define PAout(n,m) DL_GPIO_writePinsVal(GPIOA, n , (m == 1)? n : 0x00)
#define PBout(n,m) DL_GPIO_writePinsVal(GPIOB, n , (m == 1)? n : 0x00)

#define LCD_CS(m)  PAout(LCD_GPIO_CS_PIN,m)
#define LCD_RS(m)  PAout(LCD_GPIO_RS_PIN,m)
#define LCD_WR(m)  PAout(LCD_GPIO_WR_PIN,m)
#define LCD_RD(m)  PAout(LCD_GPIO_RD_PIN,m)
#define LCD_RST(m)  PAout(LCD_GPIO_RST_PIN,m)

#define LCD_DATA_GPIO_PORT          GPIOB
#define LCD_GPIO_WR_DATA(x)         do{ LCD_DATA_GPIO_PORT->DOUT31_0 = (uint16_t)x; }while(0)
#define LCD_GPIO_RD_DATA()            LCD_DATA_GPIO_PORT->DIN31_0

void LCD_GPIO_init();
void lcd_wr_cmd(uint16_t cmd);
void lcd_wr_data(uint16_t data);
void lcd_gpio_write_reg(uint16_t reg, uint16_t dat);
uint16_t lcd_gpio_read_dat(void);

#endif