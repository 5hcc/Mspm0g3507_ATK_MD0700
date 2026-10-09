#include "lcd_touch_iic.h"

#if (LCD_USING_TOUCH != 0)

/* ATK-MD0430触摸IIC数据结构体 */
static struct
{
    uint8_t iic_addr;               /* IIC通讯地址 */
} g_atk_md0430_touch_iic_sta = {0};


/**
 * @brief       产生IIC起始信号
 * @param       无
 * @retval      无
 */
void atk_md0430_touch_iic_start(void)
{
    LCD_TOUCH_IIC_SDA(1);
    LCD_TOUCH_IIC_SCL(1);
    delay_us(2);
    LCD_TOUCH_IIC_SDA(0);
    delay_us(2);
    LCD_TOUCH_IIC_SCL(0);
    delay_us(2);
}

/**
 * @brief       产生IIC停止信号
 * @param       无
 * @retval      无
 */
void atk_md0430_touch_iic_stop(void)
{
    LCD_TOUCH_IIC_SDA(0);
    delay_us(2);
    LCD_TOUCH_IIC_SCL(1);
    delay_us(2);
    LCD_TOUCH_IIC_SDA(1);
    delay_us(2);
}

/**
 * @brief       等待IIC应答信号
 * @param       无
 * @retval      0: 应答信号接收成功
 *              1: 应答信号接收失败
 */
uint8_t atk_md0430_touch_iic_wait_ack(void)
{
    uint16_t waittime = 0;
    uint8_t rack = 0;
    
    LCD_TOUCH_IIC_SDA(1);
    DL_GPIO_disableOutput(GPIOA , LCD_TOUCH_IIC_SDA_GPIO_PIN);
     
    DL_GPIO_initDigitalInputFeatures(TOUCH_SDA_IOMUX,
    DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
    DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    delay_us(2);
    LCD_TOUCH_IIC_SCL(1);
     
    while (LCD_TOUCH_IIC_READ_SDA())
    {
        waittime++;
        
        if (waittime > 250)
        {
            DL_GPIO_enableOutput(GPIOA , LCD_TOUCH_IIC_SDA_GPIO_PIN);
            atk_md0430_touch_iic_stop();
            rack = 1;
            break;
        }
    }

    DL_GPIO_enableOutput(GPIOA , LCD_TOUCH_IIC_SDA_GPIO_PIN);
    
    DL_GPIO_clearPins(GPIOA , LCD_TOUCH_IIC_SDA_GPIO_PIN);

    LCD_TOUCH_IIC_SCL(0);
    delay_us(2);
    
    return rack;
}

/**
 * @brief       产生ACK应答信号
 * @param       无
 * @retval      无
 */
static void atk_md0430_touch_iic_ack(void)
{
    LCD_TOUCH_IIC_SDA(0);
    delay_us(2);
    LCD_TOUCH_IIC_SCL(1);
    delay_us(2);
    LCD_TOUCH_IIC_SCL(0);
    delay_us(2);
    LCD_TOUCH_IIC_SDA(1);
    delay_us(2);
}

/**
 * @brief       不产生ACK应答信号
 * @param       无
 * @retval      无
 */
static void atk_md0430_touch_iic_nack(void)
{
    LCD_TOUCH_IIC_SDA(1);
    delay_us(2);
    LCD_TOUCH_IIC_SCL(1);
    delay_us(2);
    LCD_TOUCH_IIC_SCL(0);
    delay_us(2);
}

/**
 * @brief       IIC发送一个字节
 * @param       dat: 要发送的数据
 * @retval      无
 */
void atk_md0430_touch_iic_send_byte(uint8_t dat)
{
    uint8_t t;
    
    for (t=0; t<8; t++)
    {
        LCD_TOUCH_IIC_SDA((dat & 0x80) >> 7);
        delay_us(2);
        LCD_TOUCH_IIC_SCL(1);
        delay_us(2);
        LCD_TOUCH_IIC_SCL(0);
        dat <<= 1;
    }
    LCD_TOUCH_IIC_SDA(1);
}

/**
 * @brief       IIC接收一个字节
 * @param       ack: ack=1时，发送ack; ack=0时，发送nack
 * @retval      接收到的数据
 */
uint8_t atk_md0430_touch_iic_recv_byte(uint8_t ack)
{
    uint8_t i;
    uint8_t dat = 0;

    DL_GPIO_disableOutput(GPIOA , LCD_TOUCH_IIC_SDA_GPIO_PIN);

    DL_GPIO_initDigitalInputFeatures(TOUCH_SDA_IOMUX,
    DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
    DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    for (i = 0; i < 8; i++ )
    {
        dat <<= 1;
        LCD_TOUCH_IIC_SCL(1);
        delay_us(2);
        
        if (LCD_TOUCH_IIC_READ_SDA())
        {
            dat++;
        }
        
        LCD_TOUCH_IIC_SCL(0);
        delay_us(2);
    }

    DL_GPIO_enableOutput(GPIOA , LCD_TOUCH_IIC_SDA_GPIO_PIN);

    if (ack == 0)
    {
        atk_md0430_touch_iic_nack();
    }
    else
    {
        atk_md0430_touch_iic_ack();
    }

    return dat;
}

/**
 * @brief       初始化IIC接口
 * @param       无
 * @retval      无
 */
void atk_md0430_touch_iic_init(uint8_t iic_addr)
{        
    g_atk_md0430_touch_iic_sta.iic_addr = iic_addr;
}

/**
 * @brief       写ATK-MD0430模块触摸寄存器
 * @param       reg: 待写寄存器地址
 *              buf: 待写入的数据
 *              len: 待写入数据的长度
 * @retval      ATK_MD0430_TOUCH_IIC_EOK  : 写ATK-MD0430模块触摸寄存器成功
 *              ATK_MD0430_TOUCH_IIC_ERROR: 写ATK-MD0430模块触摸寄存器失败
 */
uint8_t atk_md0430_touch_iic_write_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t buf_index;
    uint8_t ret;
    
    atk_md0430_touch_iic_start();
    atk_md0430_touch_iic_send_byte((g_atk_md0430_touch_iic_sta.iic_addr << 1) | ATK_MD0430_TOUCH_IIC_WRITE);
    atk_md0430_touch_iic_wait_ack();
    atk_md0430_touch_iic_send_byte((uint8_t)(reg >> 8) & 0xFF);
    atk_md0430_touch_iic_wait_ack();
    atk_md0430_touch_iic_send_byte((uint8_t)reg & 0xFF);
    atk_md0430_touch_iic_wait_ack();
    
    for (buf_index=0; buf_index<len; buf_index++)
    {
        atk_md0430_touch_iic_send_byte(buf[buf_index]);
        ret = atk_md0430_touch_iic_wait_ack();
        if (ret != 0)
        {
            break;
        }
    }
    
    atk_md0430_touch_iic_stop();
    
    if (ret != 0)
    {
        return ATK_MD0430_TOUCH_IIC_ERROR;
    }
    
    return ATK_MD0430_TOUCH_IIC_EOK;
}

/**
 * @brief       读ATK-MD0430模块触摸寄存器
 * @param       reg: 待读寄存器地址
 *              buf: 读取的数据
 *              len: 待读取数据的长度
 * @retval      ATK_MD0430_TOUCH_IIC_EOK  : 读ATK-MD0430模块触摸寄存器成功
 *              ATK_MD0430_TOUCH_IIC_ERROR: 读ATK-MD0430模块触摸寄存器失败
 */
void atk_md0430_touch_iic_read_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t buf_index;
    
    atk_md0430_touch_iic_start();
    atk_md0430_touch_iic_send_byte((g_atk_md0430_touch_iic_sta.iic_addr << 1) | ATK_MD0430_TOUCH_IIC_WRITE);
    atk_md0430_touch_iic_wait_ack();
    atk_md0430_touch_iic_send_byte((uint8_t)(reg >> 8) & 0xFF);
    atk_md0430_touch_iic_wait_ack();
    atk_md0430_touch_iic_send_byte((uint8_t)reg & 0xFF);
    atk_md0430_touch_iic_wait_ack();
    atk_md0430_touch_iic_start();
    atk_md0430_touch_iic_send_byte((g_atk_md0430_touch_iic_sta.iic_addr << 1) | ATK_MD0430_TOUCH_IIC_READ);
    atk_md0430_touch_iic_wait_ack();
    
    for (buf_index=0; buf_index<len - 1; buf_index++)
    {
        buf[buf_index] = atk_md0430_touch_iic_recv_byte(1);
    }
    
    buf[buf_index] = atk_md0430_touch_iic_recv_byte(0);
    
    atk_md0430_touch_iic_stop();
}

#endif /* ATK_MD0430_USING_TOUCH */
