#include "lcd_touch.h"
#include "LCD_touch_iic.h"
#include <string.h>

#if (LCD_USING_TOUCH != 0)

/* ATK-MD0430模块触摸屏PID */
#define ATK_MD0430_TOUCH_PID1    "911"
#define ATK_MD0430_TOUCH_PID2    "1158"

/* ATK-MD0430模块最大触摸点数 */
#define ATK_MD0430_TOUCH_TP_MAX 5

/* ATK-MD0430模块触摸屏寄存器地址定义 */
#define ATK_MD0430_TOUCH_REG_CTRL           0x8040  /* 控制寄存器 */
#define ATK_MD0430_TOUCH_REG_PID            0x8140  /* PID寄存器 */
#define ATK_MD0430_TOUCH_REG_TPINFO         0x814E  /* 触摸状态寄存器 */
#define ATK_MD0430_TOUCH_REG_TP1            0x8150  /* 触摸点1数据寄存器 */
#define ATK_MD0430_TOUCH_REG_TP2            0x8158  /* 触摸点2数据寄存器 */
#define ATK_MD0430_TOUCH_REG_TP3            0x8160  /* 触摸点3数据寄存器 */
#define ATK_MD0430_TOUCH_REG_TP4            0x8168  /* 触摸点4数据寄存器 */
#define ATK_MD0430_TOUCH_REG_TP5            0x8170  /* 触摸点5数据寄存器 */

/* 触摸状态寄存器位定义 */
#define ATK_MD0430_TOUCH_TPINFO_MASK_STA    0x80
#define ATK_MD0430_TOUCH_TPINFO_MASK_CNT    0x0F

/* ATK-MD0430模块触摸点数据寄存器 */
static const uint16_t g_atk_md0430_touch_tp_reg[ATK_MD0430_TOUCH_TP_MAX] = {
    ATK_MD0430_TOUCH_REG_TP1,
    ATK_MD0430_TOUCH_REG_TP2,
    ATK_MD0430_TOUCH_REG_TP3,
    ATK_MD0430_TOUCH_REG_TP4,
    ATK_MD0430_TOUCH_REG_TP5,
};

/**
 * @brief       ATK-MD0430模块触摸硬件复位
 * @param       addr: 复位时使用的IIC通讯地址
 * @retval      无
 */
static void atk_md0430_touch_hw_reset(atk_md0430_touch_iic_addr_t addr)
{

        DL_GPIO_initDigitalOutputFeatures(TOUCH_PEN_IOMUX,
		DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);

    DL_GPIO_enableOutput(GPIOA , LCD_TOUCH_PEN_GPIO_PIN);

    switch (addr)
    {
        case ATK_MD0430_TOUCH_IIC_ADDR_14:
        {
            LCD_TOUCH_TCS(0);
            LCD_TOUCH_PEN(0);
            LCD_TOUCH_PEN(1);
            delay_ms(1);
            LCD_TOUCH_TCS(1);
            delay_ms(10);
            break;
        }
        case ATK_MD0430_TOUCH_IIC_ADDR_5D:
        {
            LCD_TOUCH_TCS(0);
            LCD_TOUCH_PEN(0);
            delay_ms(1);
            LCD_TOUCH_TCS(1);
            delay_ms(10);
            break;
        }
        default:
        {
            break;
        }
    }
    DL_GPIO_disableOutput(GPIOA , LCD_TOUCH_PEN_GPIO_PIN);

   DL_GPIO_initDigitalInputFeatures(TOUCH_PEN_IOMUX,
   DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
   DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
}

/**
 * @brief       ATK-MD0430模块触摸软件复位
 * @param       无
 * @retval      无
 */
static void atk_md0430_touch_sw_reset(void)
{
    uint8_t dat;
    
    dat = 0x02;
    atk_md0430_touch_iic_write_reg(ATK_MD0430_TOUCH_REG_CTRL, &dat, 1);
    delay_ms(10);
    
    dat = 0x00;
    atk_md0430_touch_iic_write_reg(ATK_MD0430_TOUCH_REG_CTRL, &dat, 1);
}

/**
 * @brief       获取ATK-MD0430模块触摸屏PID
 * @param       pid: 获取到的PID(ASCII格式)
 * @retval      无
 */
static void atk_md0430_touch_get_pid(char *pid)
{
    atk_md0430_touch_iic_read_reg(ATK_MD0430_TOUCH_REG_PID, (uint8_t *)pid, 4);
    pid[4] = '\0';
}

/**
 * @brief       ATK-MD0430模块触摸初始化
 * @param       无
 * @retval      ATK_MD0430_TOUCH_EOK  : ATK-MD0430模块触摸初始化成功
 *              ATK_MD0430_TOUCH_ERROR: ATK-MD0430模块触摸初始化失败
 */
uint8_t atk_md0430_touch_init(void)
{
    char pid[5];
    
    atk_md0430_touch_hw_reset(ATK_MD0430_TOUCH_IIC_ADDR_14);
    atk_md0430_touch_iic_init(ATK_MD0430_TOUCH_IIC_ADDR_14);
    delay_ms(100);
    atk_md0430_touch_get_pid(pid);
    if ((strcmp(pid, ATK_MD0430_TOUCH_PID1) != 0) && (strcmp(pid, ATK_MD0430_TOUCH_PID2) != 0))
    {
        return ATK_MD0430_TOUCH_ERROR;
    }
    atk_md0430_touch_sw_reset();
    
    return ATK_MD0430_TOUCH_EOK;
}

/**
 * @brief       ATK-MD0430模块触摸扫描
 * @note        建议调用间隔大于10ms
 * @param       point: 扫描到的触摸点信息
 *              cnt  : 需要扫描的触摸点数量(1~ATK_MD0430_TOUCH_TP_MAX)
 * @retval      0   : 没有扫描到触摸点
 *              其他: 实际获取到的触摸点信息数量
 */
uint8_t atk_md0430_touch_scan(atk_md0430_touch_point_t *point, uint8_t cnt)
{
    uint8_t tp_info;
    uint8_t tp_cnt;
    uint8_t point_index;
    atk_md0430_lcd_disp_dir_t dir;
    uint8_t tpn_info[6];
    atk_md0430_touch_point_t point_raw;
    
    if ((cnt == 0) || (cnt > ATK_MD0430_TOUCH_TP_MAX))
    {
        return 0;
    }
    
    for (point_index=0; point_index<cnt; point_index++)
    {
        if (&point[point_index] == NULL)
        {
            return 0;
        }
    }
    
    atk_md0430_touch_iic_read_reg(ATK_MD0430_TOUCH_REG_TPINFO, &tp_info, sizeof(tp_info));
    if ((tp_info & ATK_MD0430_TOUCH_TPINFO_MASK_STA) == ATK_MD0430_TOUCH_TPINFO_MASK_STA)
    {
        tp_cnt = tp_info & ATK_MD0430_TOUCH_TPINFO_MASK_CNT;
        tp_cnt = (cnt < tp_cnt) ? cnt : tp_cnt;
        
        for (point_index=0; point_index<tp_cnt; point_index++)
        {
            atk_md0430_touch_iic_read_reg(g_atk_md0430_touch_tp_reg[point_index], tpn_info, sizeof(tpn_info));
            point_raw.x = (uint16_t)(tpn_info[1] << 8) | tpn_info[0];
            point_raw.y = (uint16_t)(tpn_info[3] << 8) | tpn_info[2];
            point_raw.size = (uint16_t)(tpn_info[5] << 8) | tpn_info[4];
            
            dir = lcd_get_disp_dir();
            switch (dir)
            {
                case LCD_DISP_DIR_0:
                {
                    point[point_index].x = point_raw.x;
                    point[point_index].y = point_raw.y;
                    break;
                }
                case LCD_DISP_DIR_90:
                {
                    point[point_index].x = point_raw.y;
                    point[point_index].y = lcd_get_height() - point_raw.x;
                    break;
                }
                case LCD_DISP_DIR_180:
                {
                    point[point_index].x = lcd_get_width() - point_raw.x;
                    point[point_index].y = lcd_get_height() - point_raw.y;
                    break;
                }
                case LCD_DISP_DIR_270:
                {
                    point[point_index].x = lcd_get_width() - point_raw.y;
                    point[point_index].y = point_raw.x;
                    break;
                }
            }
            
            point[point_index].size = point_raw.size;
        }
        
        tp_info = 0;
        atk_md0430_touch_iic_write_reg(ATK_MD0430_TOUCH_REG_TPINFO, &tp_info, sizeof(tp_info));
        
        return tp_cnt;
    }
    else
    {
        return 0;
    }
}

#endif /* ATK_MD0430_USING_TOUCH */