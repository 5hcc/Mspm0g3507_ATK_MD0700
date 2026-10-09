#include "i8080ctrl.h"



/**
 * @brief       ATK-MD0430模块初始化
 * @param       无
 * @retval      无
 */
void LCD_GPIO_init()
{
   LCD_CS(1);
   LCD_RS(0);
   LCD_WR(1);
   LCD_RD(1);
   LCD_RST(0);
   delay_ms(20);
   LCD_RST(1);
   delay_ms(100);
}

/**
 * @brief       ATK-MD0430模块通过GPIO接口写命令
 * @param       cmd: 待写命令
 * @retval      无
 */
void lcd_wr_cmd(uint16_t cmd)
{
    LCD_RS(0);
    LCD_CS(0);
    LCD_GPIO_WR_DATA(cmd);
    LCD_WR(0);
    LCD_WR(1);
    LCD_CS(1);
}

/**
 * @brief       ATK-MD0430模块通过GPIO接口写数据
 * @param       data: 待写数据
 * @retval      无
 */
void lcd_wr_data(uint16_t data)
{
    LCD_RS(1);
    LCD_CS(0);
    LCD_GPIO_WR_DATA(data);
    LCD_WR(0);
    LCD_WR(1);
    LCD_CS(1);
}

/**
 * @brief       ATK-MD0430模块通过GPIO接口写寄存器
 * @param       reg: 待操作寄存器
 *              dat: 待写数据
 * @retval      无
 */
void lcd_gpio_write_reg(uint16_t reg, uint16_t dat)
{
    lcd_wr_cmd(reg);
    lcd_wr_data(dat);
}

/**
 * @brief       ATK-MD0430模块通过GPIO接口读数据
 * @param       无
 * @retval      读取到的数据
 */
uint16_t lcd_gpio_read_dat(void)
{
    uint16_t dat;
    DL_GPIO_disableOutput(LCD_Data_PORT,    LCD_Data_D0_PIN |
                                            LCD_Data_D1_PIN |
                                            LCD_Data_D2_PIN |
                                            LCD_Data_D3_PIN | 
                                            LCD_Data_D4_PIN | 
                                            LCD_Data_D5_PIN | 
                                            LCD_Data_D6_PIN | 
                                            LCD_Data_D7_PIN | 
                                            LCD_Data_D8_PIN | 
                                            LCD_Data_D9_PIN | 
                                            LCD_Data_D10_PIN | 
                                            LCD_Data_D11_PIN | 
                                            LCD_Data_D12_PIN | 
                                            LCD_Data_D13_PIN | 
                                            LCD_Data_D14_PIN | 
                                            LCD_Data_D15_PIN); 

    DL_GPIO_initDigitalInputFeatures(LCD_Data_D0_IOMUX, 
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		    DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(LCD_Data_D1_IOMUX, 
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(LCD_Data_D2_IOMUX, 
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(LCD_Data_D3_IOMUX, 
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(LCD_Data_D4_IOMUX, 
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(LCD_Data_D5_IOMUX, 
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(LCD_Data_D6_IOMUX, 
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(LCD_Data_D7_IOMUX, 
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(LCD_Data_D8_IOMUX, 
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(LCD_Data_D9_IOMUX, 
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(LCD_Data_D10_IOMUX, 
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(LCD_Data_D11_IOMUX, 
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(LCD_Data_D12_IOMUX, 
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(LCD_Data_D13_IOMUX, 
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(LCD_Data_D14_IOMUX, 
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(LCD_Data_D15_IOMUX, 
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);


    LCD_RS(1);
    LCD_CS(0);
    LCD_RD(0);
    __nop();
    __nop();
    dat = LCD_GPIO_RD_DATA();
    LCD_RD(1);
    LCD_CS(1);
    

   DL_GPIO_enableOutput(LCD_Data_PORT, LCD_Data_D0_PIN |
                                       LCD_Data_D1_PIN |
                                       LCD_Data_D2_PIN |
                                       LCD_Data_D3_PIN | 
                                       LCD_Data_D4_PIN | 
                                       LCD_Data_D5_PIN | 
                                       LCD_Data_D6_PIN | 
                                       LCD_Data_D7_PIN | 
                                       LCD_Data_D8_PIN | 
                                       LCD_Data_D9_PIN | 
                                       LCD_Data_D10_PIN | 
                                       LCD_Data_D11_PIN | 
                                       LCD_Data_D12_PIN | 
                                       LCD_Data_D13_PIN | 
                                       LCD_Data_D14_PIN | 
                                       LCD_Data_D15_PIN); 

    return dat;
}
