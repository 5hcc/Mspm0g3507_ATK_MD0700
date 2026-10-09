#include "lcd.h"
#include "atk_md0430_font.h"

/* ATK-MD0430模块LCD驱动器ID */
#define ATK_MD0430_CHIP_ID1          0x5510
#define ATK_MD0430_CHIP_ID2          0x9806
#define ATK_MD0430_CHIP_ID3          0x1963
/* ATK-MD0430模块LCD扫描方向 */
#define ATK_MD0430_SCAN_DIR_L2R_U2D (0x0000)
#define ATK_MD0430_SCAN_DIR_L2R_D2U (0x0080)
#define ATK_MD0430_SCAN_DIR_R2L_U2D (0x0040)
#define ATK_MD0430_SCAN_DIR_R2L_D2U (0x00C0)
#define ATK_MD0430_SCAN_DIR_U2D_L2R (0x0020)
#define ATK_MD0430_SCAN_DIR_U2D_R2L (0x0060)
#define ATK_MD0430_SCAN_DIR_D2U_L2R (0x00A0)
#define ATK_MD0430_SCAN_DIR_D2U_R2L (0x00E0)


uint8_t device = 0;

/* ATK-MD0430模块状态数据结构体 */
static struct
{
    uint16_t chip_id;                   /* 驱动器ID */
    uint16_t width;                     /* LCD宽度 */
    uint16_t height;                    /* LCD高度 */
    uint16_t wramcmd;   /* 开始写gram指令 */
    uint16_t setxcmd;   /* 设置x坐标指令 */
    uint16_t setycmd;   /* 设置y坐标指令 */
    atk_md0430_lcd_scan_dir_t scan_dir; /* LCD扫描方向 */
    atk_md0430_lcd_disp_dir_t disp_dir; /* LCD显示方向 */
} g_atk_md0430_sta = {0};


/**
 * @brief       获取ATK-MD0430模块驱动器ID
 * @param       无
 * @retval      无
 */
static inline uint16_t lcd_get_chip_id(void)
{
    uint16_t chip_id;

    lcd_wr_cmd(0xA1);
    chip_id = lcd_gpio_read_dat();
    chip_id = lcd_gpio_read_dat();  /* 读回0x57 */
    chip_id <<= 8;
    chip_id |= lcd_gpio_read_dat(); /* 读回0x61 */

    if (chip_id == 0x5761) chip_id = 0x1963; /* SSD1963读回的ID是5761H,为方便区分,我们强制设置为1963 */

    if(chip_id != ATK_MD0430_CHIP_ID3)
    {
    lcd_wr_cmd(0XD3);
    chip_id = lcd_gpio_read_dat();
    chip_id = lcd_gpio_read_dat();
    chip_id = lcd_gpio_read_dat() << 8;
    chip_id |= lcd_gpio_read_dat();
    }
    
    return chip_id;
}

void lcd_ssd_backlight_set(uint8_t pwm)
{
    lcd_wr_cmd(0xBE);         /* 配置PWM输出 */
    lcd_wr_data(0x05);          /* 1设置PWM频率 */
    lcd_wr_data(pwm * 2.55);    /* 2设置PWM占空比 */
    lcd_wr_data(0x01);          /* 3设置C */
    lcd_wr_data(0xFF);          /* 4设置D */
    lcd_wr_data(0x00);          /* 5设置E */
    lcd_wr_data(0x00);          /* 6设置F */
}

/**
 * @brief       ATK-MD0430模块寄存器初始化
 * @param       无
 * @retval      无
 */
static void atk_md0430_reg_init(void)
{
    if (g_atk_md0430_sta.chip_id == ATK_MD0430_CHIP_ID1)
    {
        lcd_wr_cmd(0x1100);
        delay_ms(120);

        lcd_wr_cmd(0x3600);     // MADCTL - 内存访问控制
        lcd_wr_data(0x00);      // MX=0, MY=0, MV=0, ML=0, RGB=0, MH=0
        
        lcd_wr_cmd(0x3A00);     // COLMOD - 像素格式
        lcd_wr_data(0x55);      // 16-bit RGB 5-6-5

        lcd_gpio_write_reg(0xF000, 0x55);
        lcd_gpio_write_reg(0xF001, 0xAA);
        lcd_gpio_write_reg(0xF002, 0x52);
        lcd_gpio_write_reg(0xF003, 0x08);
        lcd_gpio_write_reg(0xF004, 0x01);
        lcd_gpio_write_reg(0xB000, 0x0D);
        lcd_gpio_write_reg(0xB001, 0x0D);
        lcd_gpio_write_reg(0xB002, 0x0D);
        lcd_gpio_write_reg(0xB600, 0x34);
        lcd_gpio_write_reg(0xB601, 0x34);
        lcd_gpio_write_reg(0xB602, 0x34);
        lcd_gpio_write_reg(0xB100, 0x0D);
        lcd_gpio_write_reg(0xB101, 0x0D);
        lcd_gpio_write_reg(0xB102, 0x0D);
        lcd_gpio_write_reg(0xB700, 0x34);
        lcd_gpio_write_reg(0xB701, 0x34);
        lcd_gpio_write_reg(0xB702, 0x34);
        lcd_gpio_write_reg(0xB200, 0x00);
        lcd_gpio_write_reg(0xB201, 0x00);
        lcd_gpio_write_reg(0xB202, 0x00);
        lcd_gpio_write_reg(0xB800, 0x24);
        lcd_gpio_write_reg(0xB801, 0x24);
        lcd_gpio_write_reg(0xB802, 0x24);
        lcd_gpio_write_reg(0xBF00, 0x01);
        lcd_gpio_write_reg(0xB300, 0x0F);
        lcd_gpio_write_reg(0xB301, 0x0F);
        lcd_gpio_write_reg(0xB302, 0x0F);
        lcd_gpio_write_reg(0xB900, 0x34);
        lcd_gpio_write_reg(0xB901, 0x34);
        lcd_gpio_write_reg(0xB902, 0x34);
        lcd_gpio_write_reg(0xB500, 0x08);
        lcd_gpio_write_reg(0xB501, 0x08);
        lcd_gpio_write_reg(0xB502, 0x08);
        lcd_gpio_write_reg(0xC200, 0x03);
        lcd_gpio_write_reg(0xBA00, 0x24);
        lcd_gpio_write_reg(0xBA01, 0x24);
        lcd_gpio_write_reg(0xBA02, 0x24);
        lcd_gpio_write_reg(0xBC00, 0x00);
        lcd_gpio_write_reg(0xBC01, 0x78);
        lcd_gpio_write_reg(0xBC02, 0x00);
        lcd_gpio_write_reg(0xBD00, 0x00);
        lcd_gpio_write_reg(0xBD01, 0x78);
        lcd_gpio_write_reg(0xBD02, 0x00);
        lcd_gpio_write_reg(0xBE00, 0x00);
        lcd_gpio_write_reg(0xBE01, 0x64);
        lcd_gpio_write_reg(0xD100, 0x00);
        lcd_gpio_write_reg(0xD101, 0x33);
        lcd_gpio_write_reg(0xD102, 0x00);
        lcd_gpio_write_reg(0xD103, 0x34);
        lcd_gpio_write_reg(0xD104, 0x00);
        lcd_gpio_write_reg(0xD105, 0x3A);
        lcd_gpio_write_reg(0xD106, 0x00);
        lcd_gpio_write_reg(0xD107, 0x4A);
        lcd_gpio_write_reg(0xD108, 0x00);
        lcd_gpio_write_reg(0xD109, 0x5C);
        lcd_gpio_write_reg(0xD10A, 0x00);
        lcd_gpio_write_reg(0xD10B, 0x81);
        lcd_gpio_write_reg(0xD10C, 0x00);
        lcd_gpio_write_reg(0xD10D, 0xA6);
        lcd_gpio_write_reg(0xD10E, 0x00);
        lcd_gpio_write_reg(0xD10F, 0xE5);
        lcd_gpio_write_reg(0xD110, 0x01);
        lcd_gpio_write_reg(0xD111, 0x13);
        lcd_gpio_write_reg(0xD112, 0x01);
        lcd_gpio_write_reg(0xD113, 0x54);
        lcd_gpio_write_reg(0xD114, 0x01);
        lcd_gpio_write_reg(0xD115, 0x82);
        lcd_gpio_write_reg(0xD116, 0x01);
        lcd_gpio_write_reg(0xD117, 0xCA);
        lcd_gpio_write_reg(0xD118, 0x02);
        lcd_gpio_write_reg(0xD119, 0x00);
        lcd_gpio_write_reg(0xD11A, 0x02);
        lcd_gpio_write_reg(0xD11B, 0x01);
        lcd_gpio_write_reg(0xD11C, 0x02);
        lcd_gpio_write_reg(0xD11D, 0x34);
        lcd_gpio_write_reg(0xD11E, 0x02);
        lcd_gpio_write_reg(0xD11F, 0x67);
        lcd_gpio_write_reg(0xD120, 0x02);
        lcd_gpio_write_reg(0xD121, 0x84);
        lcd_gpio_write_reg(0xD122, 0x02);
        lcd_gpio_write_reg(0xD123, 0xA4);
        lcd_gpio_write_reg(0xD124, 0x02);
        lcd_gpio_write_reg(0xD125, 0xB7);
        lcd_gpio_write_reg(0xD126, 0x02);
        lcd_gpio_write_reg(0xD127, 0xCF);
        lcd_gpio_write_reg(0xD128, 0x02);
        lcd_gpio_write_reg(0xD129, 0xDE);
        lcd_gpio_write_reg(0xD12A, 0x02);
        lcd_gpio_write_reg(0xD12B, 0xF2);
        lcd_gpio_write_reg(0xD12C, 0x02);
        lcd_gpio_write_reg(0xD12D, 0xFE);
        lcd_gpio_write_reg(0xD12E, 0x03);
        lcd_gpio_write_reg(0xD12F, 0x10);
        lcd_gpio_write_reg(0xD130, 0x03);
        lcd_gpio_write_reg(0xD131, 0x33);
        lcd_gpio_write_reg(0xD132, 0x03);
        lcd_gpio_write_reg(0xD133, 0x6D);
        lcd_gpio_write_reg(0xD200, 0x00);
        lcd_gpio_write_reg(0xD201, 0x33);
        lcd_gpio_write_reg(0xD202, 0x00);
        lcd_gpio_write_reg(0xD203, 0x34);
        lcd_gpio_write_reg(0xD204, 0x00);
        lcd_gpio_write_reg(0xD205, 0x3A);
        lcd_gpio_write_reg(0xD206, 0x00);
        lcd_gpio_write_reg(0xD207, 0x4A);
        lcd_gpio_write_reg(0xD208, 0x00);
        lcd_gpio_write_reg(0xD209, 0x5C);
        lcd_gpio_write_reg(0xD20A, 0x00);
        lcd_gpio_write_reg(0xD20B, 0x81);
        lcd_gpio_write_reg(0xD20C, 0x00);
        lcd_gpio_write_reg(0xD20D, 0xA6);
        lcd_gpio_write_reg(0xD20E, 0x00);
        lcd_gpio_write_reg(0xD20F, 0xE5);
        lcd_gpio_write_reg(0xD210, 0x01);
        lcd_gpio_write_reg(0xD211, 0x13);
        lcd_gpio_write_reg(0xD212, 0x01);
        lcd_gpio_write_reg(0xD213, 0x54);
        lcd_gpio_write_reg(0xD214, 0x01);
        lcd_gpio_write_reg(0xD215, 0x82);
        lcd_gpio_write_reg(0xD216, 0x01);
        lcd_gpio_write_reg(0xD217, 0xCA);
        lcd_gpio_write_reg(0xD218, 0x02);
        lcd_gpio_write_reg(0xD219, 0x00);
        lcd_gpio_write_reg(0xD21A, 0x02);
        lcd_gpio_write_reg(0xD21B, 0x01);
        lcd_gpio_write_reg(0xD21C, 0x02);
        lcd_gpio_write_reg(0xD21D, 0x34);
        lcd_gpio_write_reg(0xD21E, 0x02);
        lcd_gpio_write_reg(0xD21F, 0x67);
        lcd_gpio_write_reg(0xD220, 0x02);
        lcd_gpio_write_reg(0xD221, 0x84);
        lcd_gpio_write_reg(0xD222, 0x02);
        lcd_gpio_write_reg(0xD223, 0xA4);
        lcd_gpio_write_reg(0xD224, 0x02);
        lcd_gpio_write_reg(0xD225, 0xB7);
        lcd_gpio_write_reg(0xD226, 0x02);
        lcd_gpio_write_reg(0xD227, 0xCF);
        lcd_gpio_write_reg(0xD228, 0x02);
        lcd_gpio_write_reg(0xD229, 0xDE);
        lcd_gpio_write_reg(0xD22A, 0x02);
        lcd_gpio_write_reg(0xD22B, 0xF2);
        lcd_gpio_write_reg(0xD22C, 0x02);
        lcd_gpio_write_reg(0xD22D, 0xFE);
        lcd_gpio_write_reg(0xD22E, 0x03);
        lcd_gpio_write_reg(0xD22F, 0x10);
        lcd_gpio_write_reg(0xD230, 0x03);
        lcd_gpio_write_reg(0xD231, 0x33);
        lcd_gpio_write_reg(0xD232, 0x03);
        lcd_gpio_write_reg(0xD233, 0x6D);
        lcd_gpio_write_reg(0xD300, 0x00);
        lcd_gpio_write_reg(0xD301, 0x33);
        lcd_gpio_write_reg(0xD302, 0x00);
        lcd_gpio_write_reg(0xD303, 0x34);
        lcd_gpio_write_reg(0xD304, 0x00);
        lcd_gpio_write_reg(0xD305, 0x3A);
        lcd_gpio_write_reg(0xD306, 0x00);
        lcd_gpio_write_reg(0xD307, 0x4A);
        lcd_gpio_write_reg(0xD308, 0x00);
        lcd_gpio_write_reg(0xD309, 0x5C);
        lcd_gpio_write_reg(0xD30A, 0x00);
        lcd_gpio_write_reg(0xD30B, 0x81);
        lcd_gpio_write_reg(0xD30C, 0x00);
        lcd_gpio_write_reg(0xD30D, 0xA6);
        lcd_gpio_write_reg(0xD30E, 0x00);
        lcd_gpio_write_reg(0xD30F, 0xE5);
        lcd_gpio_write_reg(0xD310, 0x01);
        lcd_gpio_write_reg(0xD311, 0x13);
        lcd_gpio_write_reg(0xD312, 0x01);
        lcd_gpio_write_reg(0xD313, 0x54);
        lcd_gpio_write_reg(0xD314, 0x01);
        lcd_gpio_write_reg(0xD315, 0x82);
        lcd_gpio_write_reg(0xD316, 0x01);
        lcd_gpio_write_reg(0xD317, 0xCA);
        lcd_gpio_write_reg(0xD318, 0x02);
        lcd_gpio_write_reg(0xD319, 0x00);
        lcd_gpio_write_reg(0xD31A, 0x02);
        lcd_gpio_write_reg(0xD31B, 0x01);
        lcd_gpio_write_reg(0xD31C, 0x02);
        lcd_gpio_write_reg(0xD31D, 0x34);
        lcd_gpio_write_reg(0xD31E, 0x02);
        lcd_gpio_write_reg(0xD31F, 0x67);
        lcd_gpio_write_reg(0xD320, 0x02);
        lcd_gpio_write_reg(0xD321, 0x84);
        lcd_gpio_write_reg(0xD322, 0x02);
        lcd_gpio_write_reg(0xD323, 0xA4);
        lcd_gpio_write_reg(0xD324, 0x02);
        lcd_gpio_write_reg(0xD325, 0xB7);
        lcd_gpio_write_reg(0xD326, 0x02);
        lcd_gpio_write_reg(0xD327, 0xCF);
        lcd_gpio_write_reg(0xD328, 0x02);
        lcd_gpio_write_reg(0xD329, 0xDE);
        lcd_gpio_write_reg(0xD32A, 0x02);
        lcd_gpio_write_reg(0xD32B, 0xF2);
        lcd_gpio_write_reg(0xD32C, 0x02);
        lcd_gpio_write_reg(0xD32D, 0xFE);
        lcd_gpio_write_reg(0xD32E, 0x03);
        lcd_gpio_write_reg(0xD32F, 0x10);
        lcd_gpio_write_reg(0xD330, 0x03);
        lcd_gpio_write_reg(0xD331, 0x33);
        lcd_gpio_write_reg(0xD332, 0x03);
        lcd_gpio_write_reg(0xD333, 0x6D);
        lcd_gpio_write_reg(0xD400, 0x00);
        lcd_gpio_write_reg(0xD401, 0x33);
        lcd_gpio_write_reg(0xD402, 0x00);
        lcd_gpio_write_reg(0xD403, 0x34);
        lcd_gpio_write_reg(0xD404, 0x00);
        lcd_gpio_write_reg(0xD405, 0x3A);
        lcd_gpio_write_reg(0xD406, 0x00);
        lcd_gpio_write_reg(0xD407, 0x4A);
        lcd_gpio_write_reg(0xD408, 0x00);
        lcd_gpio_write_reg(0xD409, 0x5C);
        lcd_gpio_write_reg(0xD40A, 0x00);
        lcd_gpio_write_reg(0xD40B, 0x81);
        lcd_gpio_write_reg(0xD40C, 0x00);
        lcd_gpio_write_reg(0xD40D, 0xA6);
        lcd_gpio_write_reg(0xD40E, 0x00);
        lcd_gpio_write_reg(0xD40F, 0xE5);
        lcd_gpio_write_reg(0xD410, 0x01);
        lcd_gpio_write_reg(0xD411, 0x13);
        lcd_gpio_write_reg(0xD412, 0x01);
        lcd_gpio_write_reg(0xD413, 0x54);
        lcd_gpio_write_reg(0xD414, 0x01);
        lcd_gpio_write_reg(0xD415, 0x82);
        lcd_gpio_write_reg(0xD416, 0x01);
        lcd_gpio_write_reg(0xD417, 0xCA);
        lcd_gpio_write_reg(0xD418, 0x02);
        lcd_gpio_write_reg(0xD419, 0x00);
        lcd_gpio_write_reg(0xD41A, 0x02);
        lcd_gpio_write_reg(0xD41B, 0x01);
        lcd_gpio_write_reg(0xD41C, 0x02);
        lcd_gpio_write_reg(0xD41D, 0x34);
        lcd_gpio_write_reg(0xD41E, 0x02);
        lcd_gpio_write_reg(0xD41F, 0x67);
        lcd_gpio_write_reg(0xD420, 0x02);
        lcd_gpio_write_reg(0xD421, 0x84);
        lcd_gpio_write_reg(0xD422, 0x02);
        lcd_gpio_write_reg(0xD423, 0xA4);
        lcd_gpio_write_reg(0xD424, 0x02);
        lcd_gpio_write_reg(0xD425, 0xB7);
        lcd_gpio_write_reg(0xD426, 0x02);
        lcd_gpio_write_reg(0xD427, 0xCF);
        lcd_gpio_write_reg(0xD428, 0x02);
        lcd_gpio_write_reg(0xD429, 0xDE);
        lcd_gpio_write_reg(0xD42A, 0x02);
        lcd_gpio_write_reg(0xD42B, 0xF2);
        lcd_gpio_write_reg(0xD42C, 0x02);
        lcd_gpio_write_reg(0xD42D, 0xFE);
        lcd_gpio_write_reg(0xD42E, 0x03);
        lcd_gpio_write_reg(0xD42F, 0x10);
        lcd_gpio_write_reg(0xD430, 0x03);
        lcd_gpio_write_reg(0xD431, 0x33);
        lcd_gpio_write_reg(0xD432, 0x03);
        lcd_gpio_write_reg(0xD433, 0x6D);
        lcd_gpio_write_reg(0xD500, 0x00);
        lcd_gpio_write_reg(0xD501, 0x33);
        lcd_gpio_write_reg(0xD502, 0x00);
        lcd_gpio_write_reg(0xD503, 0x34);
        lcd_gpio_write_reg(0xD504, 0x00);
        lcd_gpio_write_reg(0xD505, 0x3A);
        lcd_gpio_write_reg(0xD506, 0x00);
        lcd_gpio_write_reg(0xD507, 0x4A);
        lcd_gpio_write_reg(0xD508, 0x00);
        lcd_gpio_write_reg(0xD509, 0x5C);
        lcd_gpio_write_reg(0xD50A, 0x00);
        lcd_gpio_write_reg(0xD50B, 0x81);
        lcd_gpio_write_reg(0xD50C, 0x00);
        lcd_gpio_write_reg(0xD50D, 0xA6);
        lcd_gpio_write_reg(0xD50E, 0x00);
        lcd_gpio_write_reg(0xD50F, 0xE5);
        lcd_gpio_write_reg(0xD510, 0x01);
        lcd_gpio_write_reg(0xD511, 0x13);
        lcd_gpio_write_reg(0xD512, 0x01);
        lcd_gpio_write_reg(0xD513, 0x54);
        lcd_gpio_write_reg(0xD514, 0x01);
        lcd_gpio_write_reg(0xD515, 0x82);
        lcd_gpio_write_reg(0xD516, 0x01);
        lcd_gpio_write_reg(0xD517, 0xCA);
        lcd_gpio_write_reg(0xD518, 0x02);
        lcd_gpio_write_reg(0xD519, 0x00);
        lcd_gpio_write_reg(0xD51A, 0x02);
        lcd_gpio_write_reg(0xD51B, 0x01);
        lcd_gpio_write_reg(0xD51C, 0x02);
        lcd_gpio_write_reg(0xD51D, 0x34);
        lcd_gpio_write_reg(0xD51E, 0x02);
        lcd_gpio_write_reg(0xD51F, 0x67);
        lcd_gpio_write_reg(0xD520, 0x02);
        lcd_gpio_write_reg(0xD521, 0x84);
        lcd_gpio_write_reg(0xD522, 0x02);
        lcd_gpio_write_reg(0xD523, 0xA4);
        lcd_gpio_write_reg(0xD524, 0x02);
        lcd_gpio_write_reg(0xD525, 0xB7);
        lcd_gpio_write_reg(0xD526, 0x02);
        lcd_gpio_write_reg(0xD527, 0xCF);
        lcd_gpio_write_reg(0xD528, 0x02);
        lcd_gpio_write_reg(0xD529, 0xDE);
        lcd_gpio_write_reg(0xD52A, 0x02);
        lcd_gpio_write_reg(0xD52B, 0xF2);
        lcd_gpio_write_reg(0xD52C, 0x02);
        lcd_gpio_write_reg(0xD52D, 0xFE);
        lcd_gpio_write_reg(0xD52E, 0x03);
        lcd_gpio_write_reg(0xD52F, 0x10);
        lcd_gpio_write_reg(0xD530, 0x03);
        lcd_gpio_write_reg(0xD531, 0x33);
        lcd_gpio_write_reg(0xD532, 0x03);
        lcd_gpio_write_reg(0xD533, 0x6D);
        lcd_gpio_write_reg(0xD600, 0x00);
        lcd_gpio_write_reg(0xD601, 0x33);
        lcd_gpio_write_reg(0xD602, 0x00);
        lcd_gpio_write_reg(0xD603, 0x34);
        lcd_gpio_write_reg(0xD604, 0x00);
        lcd_gpio_write_reg(0xD605, 0x3A);
        lcd_gpio_write_reg(0xD606, 0x00);
        lcd_gpio_write_reg(0xD607, 0x4A);
        lcd_gpio_write_reg(0xD608, 0x00);
        lcd_gpio_write_reg(0xD609, 0x5C);
        lcd_gpio_write_reg(0xD60A, 0x00);
        lcd_gpio_write_reg(0xD60B, 0x81);
        lcd_gpio_write_reg(0xD60C, 0x00);
        lcd_gpio_write_reg(0xD60D, 0xA6);
        lcd_gpio_write_reg(0xD60E, 0x00);
        lcd_gpio_write_reg(0xD60F, 0xE5);
        lcd_gpio_write_reg(0xD610, 0x01);
        lcd_gpio_write_reg(0xD611, 0x13);
        lcd_gpio_write_reg(0xD612, 0x01);
        lcd_gpio_write_reg(0xD613, 0x54);
        lcd_gpio_write_reg(0xD614, 0x01);
        lcd_gpio_write_reg(0xD615, 0x82);
        lcd_gpio_write_reg(0xD616, 0x01);
        lcd_gpio_write_reg(0xD617, 0xCA);
        lcd_gpio_write_reg(0xD618, 0x02);
        lcd_gpio_write_reg(0xD619, 0x00);
        lcd_gpio_write_reg(0xD61A, 0x02);
        lcd_gpio_write_reg(0xD61B, 0x01);
        lcd_gpio_write_reg(0xD61C, 0x02);
        lcd_gpio_write_reg(0xD61D, 0x34);
        lcd_gpio_write_reg(0xD61E, 0x02);
        lcd_gpio_write_reg(0xD61F, 0x67);
        lcd_gpio_write_reg(0xD620, 0x02);
        lcd_gpio_write_reg(0xD621, 0x84);
        lcd_gpio_write_reg(0xD622, 0x02);
        lcd_gpio_write_reg(0xD623, 0xA4);
        lcd_gpio_write_reg(0xD624, 0x02);
        lcd_gpio_write_reg(0xD625, 0xB7);
        lcd_gpio_write_reg(0xD626, 0x02);
        lcd_gpio_write_reg(0xD627, 0xCF);
        lcd_gpio_write_reg(0xD628, 0x02);
        lcd_gpio_write_reg(0xD629, 0xDE);
        lcd_gpio_write_reg(0xD62A, 0x02);
        lcd_gpio_write_reg(0xD62B, 0xF2);
        lcd_gpio_write_reg(0xD62C, 0x02);
        lcd_gpio_write_reg(0xD62D, 0xFE);
        lcd_gpio_write_reg(0xD62E, 0x03);
        lcd_gpio_write_reg(0xD62F, 0x10);
        lcd_gpio_write_reg(0xD630, 0x03);
        lcd_gpio_write_reg(0xD631, 0x33);
        lcd_gpio_write_reg(0xD632, 0x03);
        lcd_gpio_write_reg(0xD633, 0x6D);
        lcd_gpio_write_reg(0xF000, 0x55);
        lcd_gpio_write_reg(0xF001, 0xAA);
        lcd_gpio_write_reg(0xF002, 0x52);
        lcd_gpio_write_reg(0xF003, 0x08);
        lcd_gpio_write_reg(0xF004, 0x00);
        lcd_gpio_write_reg(0xB100, 0xCC);
        lcd_gpio_write_reg(0xB101, 0x00);
        lcd_gpio_write_reg(0xB600, 0x05);
        lcd_gpio_write_reg(0xB700, 0x70);
        lcd_gpio_write_reg(0xB701, 0x70);
        lcd_gpio_write_reg(0xB800, 0x01);
        lcd_gpio_write_reg(0xB801, 0x03);
        lcd_gpio_write_reg(0xB802, 0x03);
        lcd_gpio_write_reg(0xB803, 0x03);
        lcd_gpio_write_reg(0xBC00, 0x02);
        lcd_gpio_write_reg(0xBC01, 0x00);
        lcd_gpio_write_reg(0xBC02, 0x00);
        lcd_gpio_write_reg(0xC900, 0xD0);
        lcd_gpio_write_reg(0xC901, 0x02);
        lcd_gpio_write_reg(0xC902, 0x50);
        lcd_gpio_write_reg(0xC903, 0x50);
        lcd_gpio_write_reg(0xC904, 0x50);
        lcd_gpio_write_reg(0x3500, 0x00);
        lcd_gpio_write_reg(0x3A00, 0x55);
        lcd_wr_cmd(0x2900);
    }
    else if (g_atk_md0430_sta.chip_id == ATK_MD0430_CHIP_ID2)
    {
        lcd_wr_cmd(0xFF); 
        lcd_wr_data(0xFF);
        lcd_wr_data(0x98);
        lcd_wr_data(0x06);
        lcd_wr_cmd(0xBC);
        lcd_wr_data(0x01);
        lcd_wr_data(0x0F);
        lcd_wr_data(0x61);
        lcd_wr_data(0xFF);
        lcd_wr_data(0x01);
        lcd_wr_data(0x01);
        lcd_wr_data(0x0B);
        lcd_wr_data(0x10);
        lcd_wr_data(0x37);
        lcd_wr_data(0x63);
        lcd_wr_data(0xFF);
        lcd_wr_data(0xFF);
        lcd_wr_data(0x01);
        lcd_wr_data(0x01);
        lcd_wr_data(0x00);
        lcd_wr_data(0x00);
        lcd_wr_data(0xFF);
        lcd_wr_data(0x52);
        lcd_wr_data(0x01);
        lcd_wr_data(0x00);
        lcd_wr_data(0x40);
        lcd_wr_cmd(0xBD);
        lcd_wr_data(0x01);
        lcd_wr_data(0x23);
        lcd_wr_data(0x45);
        lcd_wr_data(0x67);
        lcd_wr_data(0x01);
        lcd_wr_data(0x23);
        lcd_wr_data(0x45);
        lcd_wr_data(0x67);
        lcd_wr_cmd(0xBE);
        lcd_wr_data(0x00);
        lcd_wr_data(0x01);
        lcd_wr_data(0xAB);
        lcd_wr_data(0x60);
        lcd_wr_data(0x22);
        lcd_wr_data(0x22);
        lcd_wr_data(0x22);
        lcd_wr_data(0x22);
        lcd_wr_data(0x22);
        lcd_wr_cmd(0xC7);
        lcd_wr_data(0x36);
        lcd_wr_cmd(0xED);
        lcd_wr_data(0x7F);
        lcd_wr_data(0x0F);
        lcd_wr_cmd(0XC0);
        lcd_wr_data(0x0F);
        lcd_wr_data(0x0B);
        lcd_wr_data(0x0A);
        lcd_wr_cmd(0XFC);
        lcd_wr_data(0x08);
        lcd_wr_cmd(0XDF); 
        lcd_wr_data(0x00);
        lcd_wr_data(0x00);
        lcd_wr_data(0x00);
        lcd_wr_data(0x00);
        lcd_wr_data(0x00);
        lcd_wr_data(0x20);
        lcd_wr_cmd(0XF3);
        lcd_wr_data(0x74);
        lcd_wr_cmd(0xB4);
        lcd_wr_data(0x00);
        lcd_wr_data(0x00);
        lcd_wr_data(0x00);
        lcd_wr_cmd(0xF7);
        lcd_wr_data(0x82);
        lcd_wr_cmd(0xB1);
        lcd_wr_data(0x00);
        lcd_wr_data(0x13);
        lcd_wr_data(0x13); 
        lcd_wr_cmd(0XF2);
        lcd_wr_data(0x80);
        lcd_wr_data(0x04);
        lcd_wr_data(0x40);
        lcd_wr_data(0x28);
        lcd_wr_cmd(0XC1);
        lcd_wr_data(0x17);
        lcd_wr_data(0x88);
        lcd_wr_data(0x88);
        lcd_wr_data(0x20);
        lcd_wr_cmd(0xE0);
        lcd_wr_data(0x00);
        lcd_wr_data(0x0A);
        lcd_wr_data(0x12);
        lcd_wr_data(0x10);
        lcd_wr_data(0x0E);
        lcd_wr_data(0x20);
        lcd_wr_data(0xCC);
        lcd_wr_data(0x07);
        lcd_wr_data(0x06);
        lcd_wr_data(0x0B);
        lcd_wr_data(0x0E);
        lcd_wr_data(0x0F);
        lcd_wr_data(0x0D);
        lcd_wr_data(0x15);
        lcd_wr_data(0x10);
        lcd_wr_data(0x00);
        lcd_wr_cmd(0xE1);
        lcd_wr_data(0x00);
        lcd_wr_data(0x0B);
        lcd_wr_data(0x13);
        lcd_wr_data(0x0D);
        lcd_wr_data(0x0E);
        lcd_wr_data(0x1B);
        lcd_wr_data(0x71);
        lcd_wr_data(0x06);
        lcd_wr_data(0x06);
        lcd_wr_data(0x0A);
        lcd_wr_data(0x0F);
        lcd_wr_data(0x0E);
        lcd_wr_data(0x0F);
        lcd_wr_data(0x15);
        lcd_wr_data(0x0C);
        lcd_wr_data(0x00);
        lcd_wr_cmd(0x2a);
        lcd_wr_data(0x00);
        lcd_wr_data(0x00);
        lcd_wr_data(0x01);
        lcd_wr_data(0xdf);
        lcd_wr_cmd(0x2b);
        lcd_wr_data(0x00);
        lcd_wr_data(0x00);
        lcd_wr_data(0x03);
        lcd_wr_data(0x1f);
        lcd_wr_cmd(0x3A);
        lcd_wr_data(0x55);
        lcd_wr_cmd(0x36);
        lcd_wr_data(0x00);
        lcd_wr_cmd(0x11);
        delay_ms(120);   
        lcd_wr_cmd(0x29);  
        delay_ms(20);  
        lcd_wr_cmd(0x2C);
    }

    else if (g_atk_md0430_sta.chip_id == ATK_MD0430_CHIP_ID3)
    {
        lcd_wr_cmd(0xE2); /* Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz */
        lcd_wr_data(0x1D);  /* 参数1 */
        lcd_wr_data(0x02);  /* 参数2 Divider M = 2, PLL = 300/(M+1) = 100MHz */
        lcd_wr_data(0x04);  /* 参数3 Validate M and N values */
        delay_ms(1);
        lcd_wr_cmd(0xE0); /*  Start PLL command */
        lcd_wr_data(0x01);  /*  enable PLL */
        delay_ms(10);
        lcd_wr_cmd(0xE0); /*  Start PLL command again */
        lcd_wr_data(0x03);  /*  now, use PLL output as system clock */
        delay_ms(12);
        lcd_wr_cmd(0x01); /* 软复位 */
        delay_ms(10);

        lcd_wr_cmd(0xE6); /* 设置像素频率,33Mhz */
        lcd_wr_data(0x2F);
        lcd_wr_data(0xFF);
        lcd_wr_data(0xFF);

        lcd_wr_cmd(0xB0); /* 设置LCD模式 */
        lcd_wr_data(0x20);  /* 24位模式 */
        lcd_wr_data(0x00);  /* TFT 模式 */

        lcd_wr_data((SSD_HOR_RESOLUTION - 1) >> 8); /* 设置LCD水平像素 */
        lcd_wr_data(SSD_HOR_RESOLUTION - 1);
        lcd_wr_data((SSD_VER_RESOLUTION - 1) >> 8); /* 设置LCD垂直像素 */
        lcd_wr_data(SSD_VER_RESOLUTION - 1);
        lcd_wr_data(0x00);  /* RGB序列 */

        lcd_wr_cmd(0xB4); /* Set horizontal period */
        lcd_wr_data((SSD_HT - 1) >> 8);
        lcd_wr_data(SSD_HT - 1);
        lcd_wr_data(SSD_HPS >> 8);
        lcd_wr_data(SSD_HPS);
        lcd_wr_data(SSD_HOR_PULSE_WIDTH - 1);
        lcd_wr_data(0x00);
        lcd_wr_data(0x00);
        lcd_wr_data(0x00);
        lcd_wr_cmd(0xB6); /* Set vertical perio */
        lcd_wr_data((SSD_VT - 1) >> 8);
        lcd_wr_data(SSD_VT - 1);
        lcd_wr_data(SSD_VPS >> 8);
        lcd_wr_data(SSD_VPS);
        lcd_wr_data(SSD_VER_FRONT_PORCH - 1);
        lcd_wr_data(0x00);
        lcd_wr_data(0x00);

        lcd_wr_cmd(0xF0); /* 设置SSD1963与CPU接口为16bit */
        lcd_wr_data(0x03);  /* 16-bit(565 format) data for 16bpp */

        lcd_wr_cmd(0x29); /* 开启显示 */
        /* 设置PWM输出  背光通过占空比可调 */
        lcd_wr_cmd(0xD0); /* 设置自动白平衡DBC */
        lcd_wr_data(0x00);  /* disable */

        lcd_wr_cmd(0xBE); /* 配置PWM输出 */
        lcd_wr_data(0x05);  /* 1设置PWM频率 */
        lcd_wr_data(0xFE);  /* 2设置PWM占空比 */
        lcd_wr_data(0x01);  /* 3设置C */
        lcd_wr_data(0x00);  /* 4设置D */
        lcd_wr_data(0x00);  /* 5设置E */
        lcd_wr_data(0x00);  /* 6设置F */

        lcd_wr_cmd(0xB8); /* 设置GPIO配置 */
        lcd_wr_data(0x03);  /* 2个IO口设置成输出 */
        lcd_wr_data(0x01);  /* GPIO使用正常的IO功能 */
        lcd_wr_cmd(0xBA);
        lcd_wr_data(0X01);  /* GPIO[1:0]=01,控制LCD方向 */
        
        lcd_ssd_backlight_set(100); /* 设置背光亮度，范围0~100 */
}

}

/**
 * @brief       设置ATK-MD0430模块列地址
 * @param       无
 * @retval      无
 */
static void atk_md0430_set_column_address(uint16_t sc, uint16_t ec)
{
    if (g_atk_md0430_sta.chip_id == ATK_MD0430_CHIP_ID1)
    {
        lcd_wr_cmd(0x2A00);
        lcd_wr_data((uint8_t)(sc >> 8) & 0xFF);
        lcd_wr_cmd(0x2A01);
        lcd_wr_data((uint8_t)sc & 0xFF);
        lcd_wr_cmd(0x2A02);
        lcd_wr_data((uint8_t)(ec >> 8) & 0xFF);
        lcd_wr_cmd(0x2A03);
        lcd_wr_data((uint8_t)ec & 0xFF);
    }
    else if (g_atk_md0430_sta.chip_id == ATK_MD0430_CHIP_ID2)
    {
        lcd_wr_cmd(0x2A);
        lcd_wr_data((uint8_t)(sc >> 8) & 0xFF);
        lcd_wr_data((uint8_t)sc & 0xFF);
        lcd_wr_data((uint8_t)(ec >> 8) & 0xFF);
        lcd_wr_data((uint8_t)ec & 0xFF);
    }
}

/**
 * @brief       设置ATK-MD0430模块页地址
 * @param       无
 * @retval      无
 */
static void atk_md0430_set_page_address(uint16_t sp, uint16_t ep)
{
    if (g_atk_md0430_sta.chip_id == ATK_MD0430_CHIP_ID1)
    {
        lcd_wr_cmd(0x2B00);
        lcd_wr_data((uint8_t)(sp >> 8) & 0xFF);
        lcd_wr_cmd(0x2B01);
        lcd_wr_data((uint8_t)sp & 0xFF);
        lcd_wr_cmd(0x2B02);
        lcd_wr_data((uint8_t)(ep >> 8) & 0xFF);
        lcd_wr_cmd(0x2B03);
        lcd_wr_data((uint8_t)ep & 0xFF);
    }
    else if (g_atk_md0430_sta.chip_id == ATK_MD0430_CHIP_ID2)
    {
        lcd_wr_cmd(0x2B);
        lcd_wr_data((uint8_t)(sp >> 8) & 0xFF);
        lcd_wr_data((uint8_t)sp & 0xFF);
        lcd_wr_data((uint8_t)(ep >> 8) & 0xFF);
        lcd_wr_data((uint8_t)ep & 0xFF);
    }
}

/**
 * @brief       开始写ATK-MD0430模块显存
 * @param       无
 * @retval      无
 */
static void atk_md0430_start_write_memory(void)
{
    if (g_atk_md0430_sta.chip_id == ATK_MD0430_CHIP_ID1)
    {
        lcd_wr_cmd(0x2C00);
    }
    else if (g_atk_md0430_sta.chip_id == ATK_MD0430_CHIP_ID2)
    {
        lcd_wr_cmd(0x2C);
    }
}

/**
 * @brief       开始读ATK-MD0430模块显存
 * @param       无
 * @retval      无
 */
static void atk_md0430_start_read_memory(void)
{
    if (g_atk_md0430_sta.chip_id == ATK_MD0430_CHIP_ID1)
    {
        lcd_wr_cmd(0x2E00);
    }
    else if (g_atk_md0430_sta.chip_id == ATK_MD0430_CHIP_ID2)
    {
        lcd_wr_cmd(0x2E);
    }
}

/**
 * @brief       平方函数，x^y
 * @param       x: 底数
 *              y: 指数
 * @retval      x^y
 */
static uint32_t atk_md0430_pow(uint8_t x, uint8_t y)
{
    uint8_t loop;
    uint32_t res = 1;
    
    for (loop=0; loop<y; loop++)
    {
        res *= x;
    }
    
    return res;
}

/**
 * @brief       ATK-MD0430模块初始化
 * @param       无
 * @retval      LCD_EOK  : ATK_MD0430模块初始化成功
 *              LCD_ERROR: ATK_MD0430模块初始化失败
 */
uint8_t lcd_init(void)
{
    uint16_t chip_id;
    #if (LCD_USING_TOUCH != 0)
    uint8_t ret;

#endif
    
    LCD_GPIO_init();             /* ATK-MD0430模块GPIO接口初始化 */
    chip_id = lcd_get_chip_id(); /* 获取ATK-MD0430模块驱动器ID */

    if ((chip_id != ATK_MD0430_CHIP_ID1) && (chip_id != ATK_MD0430_CHIP_ID2) && (chip_id != ATK_MD0430_CHIP_ID3))
    {
        return LCD_ERROR;
    }
    else
    {
        g_atk_md0430_sta.chip_id = chip_id;
        g_atk_md0430_sta.width = LCD_WIDTH;
        g_atk_md0430_sta.height = LCD_HEIGHT;
    }
    atk_md0430_reg_init();
    lcd_set_disp_dir(LCD_DISP_DIR_90);
    lcd_clear(ATK_MD0430_WHITE);
    lcd_display_on();
    
    #if (LCD_USING_TOUCH != 0)
    ret = atk_md0430_touch_init();
    if (ret != ATK_MD0430_TOUCH_EOK)
    {
        ret = atk_md0430_touch_init();
        if (ret != ATK_MD0430_TOUCH_EOK)
        {
            device = 0;
        }
        else device = 2;      
    }
    else if (ret == ATK_MD0430_TOUCH_EOK)
    {
        device = 1;
    }
    else
    {
        device = 0;
    }
    // ret = ATK_MD0430_touch_init();
    #endif
    
    return device;
}

/**
 * @brief       获取ATK-MD0430模块LCD宽度
 * @param       无
 * @retval      ATK-MD0430模块LCD宽度
 */
uint16_t lcd_get_width(void)
{
    return g_atk_md0430_sta.width;
}

/**
 * @brief       获取ATK-MD0430模块LCD高度
 * @param       无
 * @retval      ATK-MD0430模块LCD高度
 */
uint16_t lcd_get_height(void)
{
    return g_atk_md0430_sta.height;
}

/**
 * @brief       开启ATK-MD0430模块LCD背光
 * @param       无
 * @retval      无
 */
void lcd_backlight_on(void)
{
//    ATK_MD0430_BL(1);
}

/**
 * @brief       关闭ATK-MD0430模块LCD背光
 * @param       无
 * @retval      无
 */
void lcd_backlight_off(void)
{
//    ATK_MD0430_BL(0);
}

/**
 * @brief       开启ATK-MD0430模块LCD显示
 * @param       无
 * @retval      无
 */
void lcd_display_on(void)
{
    if (g_atk_md0430_sta.chip_id == 0X5510)
    {
        lcd_wr_cmd(0X2900);   /* 开启显示 */
    }
    else    /* 9341/5310/1963/7789/7796/9806 等发送开启显示指令 */
    {
        lcd_wr_cmd(0X29);     /* 开启显示 */
    }
}

/**
 * @brief       关闭ATK-MD0430模块LCD显示
 * @param       无
 * @retval      无
 */
void lcd_display_off(void)
{
    if (g_atk_md0430_sta.chip_id == 0X5510)
    {
        lcd_wr_cmd(0X2800);   /* 关闭显示 */
    }
    else    /* 9341/5310/1963/7789/7796/9806 等发送关闭显示指令 */
    {
        lcd_wr_cmd(0X28);     /* 关闭显示 */
    }
}

/**
 * @brief       设置光标位置(对RGB屏无效)
 * @param       x,y: 坐标
 * @retval      无
 */
void lcd_set_cursor(uint16_t x, uint16_t y)
{
    if (g_atk_md0430_sta.chip_id == 0X1963)
    {
        if (g_atk_md0430_sta.disp_dir == LCD_DISP_DIR_0 || g_atk_md0430_sta.disp_dir == LCD_DISP_DIR_180)    /* 竖屏模式, x坐标需要变换 */
        {
            x = g_atk_md0430_sta.width - 1 - x;
            lcd_wr_cmd(g_atk_md0430_sta.setxcmd);
            lcd_wr_data(0);
            lcd_wr_data(0);
            lcd_wr_data(x >> 8);
            lcd_wr_data(x & 0XFF);
        }
        else                    /* 横屏模式 */
        {
            lcd_wr_cmd(g_atk_md0430_sta.setxcmd);
            lcd_wr_data(x >> 8);
            lcd_wr_data(x & 0XFF);
            lcd_wr_data((g_atk_md0430_sta.width - 1) >> 8);
            lcd_wr_data((g_atk_md0430_sta.width - 1) & 0XFF);
        }

        lcd_wr_cmd(g_atk_md0430_sta.setycmd);
        lcd_wr_data(y >> 8);
        lcd_wr_data(y & 0XFF);
        lcd_wr_data((g_atk_md0430_sta.height - 1) >> 8);
        lcd_wr_data((g_atk_md0430_sta.height - 1) & 0XFF);

    }
    else if (g_atk_md0430_sta.chip_id == 0X5510)
    {
        lcd_wr_cmd(g_atk_md0430_sta.setxcmd);
        lcd_wr_data(x >> 8);
        lcd_wr_cmd(g_atk_md0430_sta.setxcmd + 1);
        lcd_wr_data(x & 0XFF);
        lcd_wr_cmd(g_atk_md0430_sta.setycmd);
        lcd_wr_data(y >> 8);
        lcd_wr_cmd(g_atk_md0430_sta.setycmd + 1);
        lcd_wr_data(y & 0XFF);
    }
    else    /* 9341/5310/7789/7796/9806 等 设置坐标 */
    {
        lcd_wr_cmd(g_atk_md0430_sta.setxcmd);
        lcd_wr_data(x >> 8);
        lcd_wr_data(x & 0XFF);
        lcd_wr_cmd(g_atk_md0430_sta.setycmd);
        lcd_wr_data(y >> 8);
        lcd_wr_data(y & 0XFF);
    }
}

/**
 * @brief       设置ATK-MD0430模块LCD扫描方向
 * @param       scan_dir: LCD_SCAN_DIR_L2R_U2D: 从左到右，从上到下
 *                        LCD_SCAN_DIR_L2R_D2U: 从左到右，从下到上
 *                        LCD_SCAN_DIR_R2L_U2D: 从右到左，从上到下
 *                        LCD_SCAN_DIR_R2L_D2U: 从右到左，从下到上
 *                        LCD_SCAN_DIR_U2D_L2R: 从上到下，从左到右
 *                        LCD_SCAN_DIR_U2D_R2L: 从上到下，从右到左
 *                        LCD_SCAN_DIR_D2U_L2R: 从下到上，从左到右
 *                        LCD_SCAN_DIR_D2U_R2L: 从下到上，从右到左
 * @retval      LCD_EOK   : 设置ATK-MD0430模块LCD扫描方向成功
 *              LCD_ERROR : 设置ATK-MD0430模块LCD扫描方向失败
*               LCD_EINVAL: 传入参数错误
 */
uint8_t lcd_set_scan_dir(atk_md0430_lcd_scan_dir_t scan_dir)
{
    // uint16_t regval;
    uint16_t regval = 0;
    uint16_t dirreg = 0;
    uint16_t temp;

if(g_atk_md0430_sta.chip_id == 0x9806)
{
    switch (g_atk_md0430_sta.disp_dir)
    {
        case LCD_DISP_DIR_0:
        {
            regval = (uint16_t)scan_dir;
            break;
        }
        case LCD_DISP_DIR_90:
        {
            switch (scan_dir)
            {
                case LCD_SCAN_DIR_L2R_U2D:
                {
                    regval = LCD_SCAN_DIR_U2D_R2L;
                    break;
                }
                case LCD_SCAN_DIR_L2R_D2U:
                {
                    regval = LCD_SCAN_DIR_U2D_L2R;
                    break;
                }
                case LCD_SCAN_DIR_R2L_U2D:
                {
                    regval = LCD_SCAN_DIR_D2U_R2L;
                    break;
                }
                case LCD_SCAN_DIR_R2L_D2U:
                {
                    regval = LCD_SCAN_DIR_D2U_L2R;
                    break;
                }
                case LCD_SCAN_DIR_U2D_L2R:
                {
                    regval = LCD_SCAN_DIR_R2L_U2D;
                    break;
                }
                case LCD_SCAN_DIR_U2D_R2L:
                {
                    regval = LCD_SCAN_DIR_R2L_D2U;
                    break;
                }
                case LCD_SCAN_DIR_D2U_L2R:
                {
                    regval = LCD_SCAN_DIR_L2R_U2D;
                    break;
                }
                case LCD_SCAN_DIR_D2U_R2L:
                {
                    regval = LCD_SCAN_DIR_L2R_D2U;
                    break;
                }
                default:
                {
                    return LCD_EINVAL;
                }
            }
            break;
        }
        case LCD_DISP_DIR_180:
        {
            switch (scan_dir)
            {
                case LCD_SCAN_DIR_L2R_U2D:
                {
                    regval = LCD_SCAN_DIR_R2L_D2U;
                    break;
                }
                case LCD_SCAN_DIR_L2R_D2U:
                {
                    regval = LCD_SCAN_DIR_R2L_U2D;
                    break;
                }
                case LCD_SCAN_DIR_R2L_U2D:
                {
                    regval = LCD_SCAN_DIR_L2R_D2U;
                    break;
                }
                case LCD_SCAN_DIR_R2L_D2U:
                {
                    regval = LCD_SCAN_DIR_L2R_U2D;
                    break;
                }
                case LCD_SCAN_DIR_U2D_L2R:
                {
                    regval = LCD_SCAN_DIR_D2U_R2L;
                    break;
                }
                case LCD_SCAN_DIR_U2D_R2L:
                {
                    regval = LCD_SCAN_DIR_D2U_L2R;
                    break;
                }
                case LCD_SCAN_DIR_D2U_L2R:
                {
                    regval = LCD_SCAN_DIR_U2D_R2L;
                    break;
                }
                case LCD_SCAN_DIR_D2U_R2L:
                {
                    regval = LCD_SCAN_DIR_U2D_L2R;
                    break;
                }
                default:
                {
                    return LCD_EINVAL;
                }
            }
            break;
        }
        case LCD_DISP_DIR_270:
        {
            switch (scan_dir)
            {
                case LCD_SCAN_DIR_L2R_U2D:
                {
                    regval = LCD_SCAN_DIR_D2U_L2R;
                    break;
                }
                case LCD_SCAN_DIR_L2R_D2U:
                {
                    regval = LCD_SCAN_DIR_D2U_R2L;
                    break;
                }
                case LCD_SCAN_DIR_R2L_U2D:
                {
                    regval = LCD_SCAN_DIR_U2D_L2R;
                    break;
                }
                case LCD_SCAN_DIR_R2L_D2U:
                {
                    regval = LCD_SCAN_DIR_U2D_R2L;
                    break;
                }
                case LCD_SCAN_DIR_U2D_L2R:
                {
                    regval = LCD_SCAN_DIR_L2R_D2U;
                    break;
                }
                case LCD_SCAN_DIR_U2D_R2L:
                {
                    regval = LCD_SCAN_DIR_L2R_U2D;
                    break;
                }
                case LCD_SCAN_DIR_D2U_L2R:
                {
                    regval = LCD_SCAN_DIR_R2L_D2U;
                    break;
                }
                case LCD_SCAN_DIR_D2U_R2L:
                {
                    regval = LCD_SCAN_DIR_R2L_U2D;
                    break;
                }
                default:
                {
                    return LCD_EINVAL;
                }
            }
            break;
        }
        default:
        {
            return LCD_ERROR;
        }
    }
}    

/* 横屏时，对1963不改变扫描方向, 其他IC改变扫描方向！竖屏时1963改变方向, 其他IC不改变扫描方向 */
else if(g_atk_md0430_sta.chip_id == 0X1963)
{
    switch (g_atk_md0430_sta.disp_dir)
    {
        case LCD_DISP_DIR_0:
        {
            switch (scan_dir)
            {
                case LCD_SCAN_DIR_L2R_U2D:
                {
                    regval = LCD_SCAN_DIR_U2D_L2R;
                    break;
                }
                case LCD_SCAN_DIR_L2R_D2U:
                {
                    regval = LCD_SCAN_DIR_U2D_R2L;
                    break;
                }
                case LCD_SCAN_DIR_R2L_U2D:
                {
                    regval = LCD_SCAN_DIR_D2U_L2R;
                    break;
                }
                case LCD_SCAN_DIR_R2L_D2U:
                {
                    regval = LCD_SCAN_DIR_D2U_R2L;
                    break;
                }
                case LCD_SCAN_DIR_U2D_L2R:
                {
                    regval = LCD_SCAN_DIR_L2R_U2D;
                    break;
                }
                case LCD_SCAN_DIR_U2D_R2L:
                {
                    regval = LCD_SCAN_DIR_L2R_D2U;
                    break;
                }
                case LCD_SCAN_DIR_D2U_L2R:
                {
                    regval = LCD_SCAN_DIR_R2L_U2D;
                    break;
                }
                case LCD_SCAN_DIR_D2U_R2L:
                {
                    regval = LCD_SCAN_DIR_R2L_D2U;
                    break;
                }
                default:
                {
                    return LCD_EINVAL;
                }
            }
            regval |= 0x01;
            break;
        }
        case LCD_DISP_DIR_90:
        {
            regval = scan_dir;
            break;
        }
        case LCD_DISP_DIR_180:
        {
            switch (scan_dir)
            {
                case LCD_SCAN_DIR_L2R_U2D:
                {
                    regval = LCD_SCAN_DIR_U2D_L2R;
                    break;
                }
                case LCD_SCAN_DIR_L2R_D2U:
                {
                    regval = LCD_SCAN_DIR_U2D_R2L;
                    break;
                }
                case LCD_SCAN_DIR_R2L_U2D:
                {
                    regval = LCD_SCAN_DIR_D2U_L2R;
                    break;
                }
                case LCD_SCAN_DIR_R2L_D2U:
                {
                    regval = LCD_SCAN_DIR_D2U_R2L;
                    break;
                }
                case LCD_SCAN_DIR_U2D_L2R:
                {
                    regval = LCD_SCAN_DIR_L2R_U2D;
                    break;
                }
                case LCD_SCAN_DIR_U2D_R2L:
                {
                    regval = LCD_SCAN_DIR_L2R_D2U;
                    break;
                }
                case LCD_SCAN_DIR_D2U_L2R:
                {
                    regval = LCD_SCAN_DIR_R2L_U2D;
                    break;
                }
                case LCD_SCAN_DIR_D2U_R2L:
                {
                    regval = LCD_SCAN_DIR_R2L_D2U;
                    break;
                }
                default:
                {
                    return LCD_EINVAL;
                }
            }
            regval |= 0x02;
            break;
        }
        case LCD_DISP_DIR_270:
        {
            regval = scan_dir;
            regval |= 0x03;
            break;
        }
        default:
        {
            return LCD_ERROR;
        }
    }
}
    g_atk_md0430_sta.scan_dir = (atk_md0430_lcd_scan_dir_t)regval;
    dirreg = 0X36;  /* 对绝大部分驱动IC, 由0X36寄存器控制 */

	if (g_atk_md0430_sta.chip_id == 0X5510)
    dirreg = 0X3600;
    else dirreg = 0X36;

    if (g_atk_md0430_sta.chip_id == 0X9341 || g_atk_md0430_sta.chip_id == 0X7789)   //9341 & 7789 要设置BGR位
    {
        regval |= 0X08;
    }
		
		
    lcd_gpio_write_reg(dirreg, regval);

    if (g_atk_md0430_sta.chip_id != 0X1963)   /* 1963不做坐标处理 */
    {
        if (regval & 0X20)
        {
            if (g_atk_md0430_sta.width < g_atk_md0430_sta.height)   /* 交换X,Y */
            {
                temp = g_atk_md0430_sta.width;
                g_atk_md0430_sta.width = g_atk_md0430_sta.height;
                g_atk_md0430_sta.height = temp;
            }
        }
        else
        {
            if (g_atk_md0430_sta.width > g_atk_md0430_sta.height)   /* 交换X,Y */
            {
                temp = g_atk_md0430_sta.width;
                g_atk_md0430_sta.width = g_atk_md0430_sta.height;
                g_atk_md0430_sta.height = temp;
            }
        }
    }

    /* 设置显示区域(开窗)大小 */
    if (g_atk_md0430_sta.chip_id == 0X5510)
    {
        lcd_wr_cmd(g_atk_md0430_sta.setxcmd);
        lcd_wr_data(0);
        lcd_wr_cmd(g_atk_md0430_sta.setxcmd + 1);
        lcd_wr_data(0);
        lcd_wr_cmd(g_atk_md0430_sta.setxcmd + 2);
        lcd_wr_data((g_atk_md0430_sta.width - 1) >> 8);
        lcd_wr_cmd(g_atk_md0430_sta.setxcmd + 3);
        lcd_wr_data((g_atk_md0430_sta.width - 1) & 0XFF);
        lcd_wr_cmd(g_atk_md0430_sta.setycmd);
        lcd_wr_data(0);
        lcd_wr_cmd(g_atk_md0430_sta.setycmd + 1);
        lcd_wr_data(0);
        lcd_wr_cmd(g_atk_md0430_sta.setycmd + 2);
        lcd_wr_data((g_atk_md0430_sta.height - 1) >> 8);
        lcd_wr_cmd(g_atk_md0430_sta.setycmd + 3);
        lcd_wr_data((g_atk_md0430_sta.height - 1) & 0XFF);
    }
    else
    {
        lcd_wr_cmd(g_atk_md0430_sta.setxcmd);
        lcd_wr_data(0);
        lcd_wr_data(0);
        lcd_wr_data((g_atk_md0430_sta.width - 1) >> 8);
        lcd_wr_data((g_atk_md0430_sta.width - 1) & 0XFF);
        lcd_wr_cmd(g_atk_md0430_sta.setycmd);
        lcd_wr_data(0);
        lcd_wr_data(0);
        lcd_wr_data((g_atk_md0430_sta.height - 1) >> 8);
        lcd_wr_data((g_atk_md0430_sta.height - 1) & 0XFF);
    }

    return LCD_EOK;
}


/**
 * @brief       设置ATK-MD0430模块LCD显示方向
 * @param       disp_dir: LCD_DISP_DIR_0  : LCD顺时针旋转0°显示内容
 *                        LCD_DISP_DIR_90 : LCD顺时针旋转90°显示内容
 *                        LCD_DISP_DIR_180: LCD顺时针旋转180°显示内容
 *                        LCD_DISP_DIR_270: LCD顺时针旋转270°显示内容
 * @retval      LCD_EOK   : 设置ATK-MD0430模块LCD显示方向成功
 *              LCD_EINVAL: 传入参数错误
 */
uint8_t lcd_set_disp_dir(atk_md0430_lcd_disp_dir_t disp_dir)
{
//         switch (disp_dir)
//     {
//         case LCD_DISP_DIR_0:
//         {
//             g_atk_md0430_sta.width = LCD_WIDTH;
//             g_atk_md0430_sta.height = LCD_HEIGHT;
//             break;
//         }
//         case LCD_DISP_DIR_90:
//         {
//             g_atk_md0430_sta.width = LCD_HEIGHT;
//             g_atk_md0430_sta.height = LCD_WIDTH;
//             break;
//         }
//         case LCD_DISP_DIR_180:
//         {
//             g_atk_md0430_sta.width = LCD_WIDTH;
//             g_atk_md0430_sta.height = LCD_HEIGHT;
//             break;
//         }
//         case LCD_DISP_DIR_270:
//         {
//             g_atk_md0430_sta.width = LCD_HEIGHT;
//             g_atk_md0430_sta.height = LCD_WIDTH;
//             break;
//         }
//         default:
//         {
//             return LCD_EINVAL;
//         }
//     }
    
       g_atk_md0430_sta.disp_dir = disp_dir;
//     lcd_set_scan_dir(LCD_SCAN_DIR_L2R_U2D);



    if (disp_dir == LCD_DISP_DIR_0 || disp_dir == LCD_DISP_DIR_180)       /* 竖屏 */
    {
        g_atk_md0430_sta.width = 240;
        g_atk_md0430_sta.height = 320;

        if (g_atk_md0430_sta.chip_id == 0x5510)
        {
            g_atk_md0430_sta.wramcmd = 0X2C00;
            g_atk_md0430_sta.setxcmd = 0X2A00;
            g_atk_md0430_sta.setycmd = 0X2B00;
            g_atk_md0430_sta.width = 480;
            g_atk_md0430_sta.height = 800;
        }
        else if (g_atk_md0430_sta.chip_id == 0X1963)
        {
            g_atk_md0430_sta.wramcmd = 0X2C;  /* 设置写入GRAM的指令 */
            g_atk_md0430_sta.setxcmd = 0X2B;  /* 设置写X坐标指令 */
            g_atk_md0430_sta.setycmd = 0X2A;  /* 设置写Y坐标指令 */
            g_atk_md0430_sta.width = 480;     /* 设置宽度480 */
            g_atk_md0430_sta.height = 800;    /* 设置高度800 */
        }
        else   /* 其他IC, 包括: 9341 / 5310 / 7789 / 7796 / 9806 等IC */
        {
            g_atk_md0430_sta.wramcmd = 0X2C;
            g_atk_md0430_sta.setxcmd = 0X2A;
            g_atk_md0430_sta.setycmd = 0X2B;
        }

        if (g_atk_md0430_sta.chip_id == 0X5310 || g_atk_md0430_sta.chip_id == 0x7796)     /* 如果是5310/7796 则表示是 320*480分辨率 */
        {
            g_atk_md0430_sta.width = 320;
            g_atk_md0430_sta.height = 480;
        }

        if (g_atk_md0430_sta.chip_id == 0X9806)    /* 如果是9806 则表示是 480*800 分辨率 */
        {
            g_atk_md0430_sta.width = 480;
            g_atk_md0430_sta.height = 800;
        }
    }
    else                /* 横屏 */
    {
        g_atk_md0430_sta.width = 320;         /* 默认宽度 */
        g_atk_md0430_sta.height = 240;        /* 默认高度 */

        if (g_atk_md0430_sta.chip_id == 0x5510)
        {
            g_atk_md0430_sta.wramcmd = 0X2C00;
            g_atk_md0430_sta.setxcmd = 0X2A00;
            g_atk_md0430_sta.setycmd = 0X2B00;
            g_atk_md0430_sta.width = 800;
            g_atk_md0430_sta.height = 480;
        }
        else if (g_atk_md0430_sta.chip_id == 0X1963 || g_atk_md0430_sta.chip_id == 0x9806)
        {
            g_atk_md0430_sta.wramcmd = 0X2C;  /* 设置写入GRAM的指令 */
            g_atk_md0430_sta.setxcmd = 0X2A;  /* 设置写X坐标指令 */
            g_atk_md0430_sta.setycmd = 0X2B;  /* 设置写Y坐标指令 */
            g_atk_md0430_sta.width = 800;     /* 设置宽度800 */
            g_atk_md0430_sta.height = 480;    /* 设置高度480 */
        }
        else   /* 其他IC, 包括: 9341 / 5310 / 7789 / 7796 等IC */
        {
            g_atk_md0430_sta.wramcmd = 0X2C;
            g_atk_md0430_sta.setxcmd = 0X2A;
            g_atk_md0430_sta.setycmd = 0X2B;
        }

        if (g_atk_md0430_sta.chip_id == 0X5310 || g_atk_md0430_sta.chip_id == 0x7796)     /* 如果是5310/7796 则表示是 320*480分辨率 */
        {
            g_atk_md0430_sta.width = 480;
            g_atk_md0430_sta.height = 320;
        }
    }

    lcd_set_scan_dir(LCD_SCAN_DIR_L2R_U2D);     /* 默认扫描方向 */
    
    return LCD_EOK;
}

/**
 * @brief       获取ATK-MD0430模块LCD显示方向
 * @param       无
 * @retval      ATK-MD0430模块LCD显示方向
 */
atk_md0430_lcd_disp_dir_t lcd_get_disp_dir(void)
{
    return g_atk_md0430_sta.disp_dir;
}

/**
 * @brief       ATK-MD0430模块LCD区域填充
 * @param       xs   : 区域起始X坐标
 *              ys   : 区域起始Y坐标
 *              xe   : 区域终止X坐标
 *              ye   : 区域终止Y坐标
 *              color: 区域填充颜色
 * @retval      无
 */
void lcd_fill(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color)
{
    uint16_t i, j;
    uint16_t xlen = 0;
    xlen = xe - xs + 1;
    
    for (i = ys; i <= ye; i++)
    {
        lcd_set_cursor(xs, i);      /* 设置光标位置 */
        lcd_wr_cmd(g_atk_md0430_sta.wramcmd);        /* 开始写入GRAM */

        for (j = 0; j < xlen; j++)
        {
            lcd_wr_data(color);
        }
    }
}

/**
 * @brief       ATK-MD0430模块LCD清屏
 * @param       color: 清屏颜色
 * @retval      无
 */
void lcd_clear(uint16_t color)
{
    uint32_t index = 0;
    uint32_t totalpoint = g_atk_md0430_sta.width;
    totalpoint *= g_atk_md0430_sta.height;    /* 得到总点数 */
    lcd_set_cursor(0x00, 0x0000);   /* 设置光标位置 */
    lcd_wr_cmd(g_atk_md0430_sta.wramcmd);        /* 开始写入GRAM */
    for (index = 0; index < totalpoint; index++)
    {
        lcd_wr_data(color);
    }

}

/**
 * @brief       ATK-MD0430模块LCD画点
 * @param       x    : 待画点的X坐标
 *              y    : 待画点的Y坐标
 *              color: 待画点的颜色
 * @retval      无
 */
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color)
{
    // atk_md0430_set_column_address(x, x);
    // atk_md0430_set_page_address(y, y);
    // atk_md0430_start_write_memory();
    // lcd_wr_data(color);
    lcd_set_cursor(x, y);       /* 设置光标位置 */
    lcd_wr_cmd(g_atk_md0430_sta.wramcmd);        /* 开始写入GRAM */
    lcd_wr_data(color);

}

/**
 * @brief       ATK-MD0430模块LCD读点
 * @param       x    : 待读点的X坐标
 *              y    : 待读点的Y坐标
 * @retval      待读点的颜色
 */
uint16_t lcd_read_point(uint16_t x, uint16_t y)
{
    uint16_t color;
    uint8_t color_r;
    uint8_t color_g;
    uint8_t color_b;
    
    if ((x >= g_atk_md0430_sta.width) || (y >= g_atk_md0430_sta.height))
    {
        return LCD_EINVAL;
    }
    
    atk_md0430_set_column_address(x, x);
    atk_md0430_set_page_address(y, y);
    atk_md0430_start_read_memory();
    
    color = lcd_gpio_read_dat(); /* Dummy */
    color = lcd_gpio_read_dat(); /* [15:11]: R, [7:2]:G */
    color_r = (uint8_t)(color >> 11) & 0x1F;
    color_g = (uint8_t)(color >> 2) & 0x3F;
    color = lcd_gpio_read_dat(); /* [15:11]: B */
    color_b = (uint8_t)(color >> 11) & 0x1F;
    
    return (uint16_t)(color_r << 11) | (color_g << 5) | color_b;
}

/**
 * @brief       ATK-MD0430模块LCD画线段
 * @param       x1   : 待画线段端点1的X坐标
 *              y1   : 待画线段端点1的Y坐标
 *              x2   : 待画线段端点2的X坐标
 *              y2   : 待画线段端点2的Y坐标
 *              color: 待画线段的颜色
 * @retval      无
 */
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t x_delta;
    uint16_t y_delta;
    int16_t x_sign;
    int16_t y_sign;
    int16_t error;
    int16_t error2;
    
    x_delta = (x1 < x2) ? (x2 - x1) : (x1 - x2);
    y_delta = (y1 < y2) ? (y2 - y1) : (y1 - y2);
    x_sign = (x1 < x2) ? 1 : -1;
    y_sign = (y1 < y2) ? 1 : -1;
    error = x_delta - y_delta;
    
    lcd_draw_point(x2, y2, color);
    
    while ((x1 != x2) || (y1 != y2))
    {
        lcd_draw_point(x1, y1, color);
        
        error2 = error << 1;
        if (error2 > -y_delta)
        {
            error -= y_delta;
            x1 += x_sign;
        }
        
        if (error2 < x_delta)
        {
            error += x_delta;
            y1 += y_sign;
        }
    }
}

/**
 * @brief       画水平线
 * @param       x,y: 起点坐标
 * @param       len  : 线长度
 * @param       color: 矩形的颜色
 * @retval      无
 */
void lcd_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
    if ((len == 0) || (x > g_atk_md0430_sta.width) || (y > g_atk_md0430_sta.height))return;

    lcd_fill(x, y, x + len - 1, y, color);
}

/**
 * @brief       ATK-MD0430模块LCD画矩形框
 * @param       x1   : 待画矩形框端点1的X坐标
 *              y1   : 待画矩形框端点1的Y坐标
 *              x2   : 待画矩形框端点2的X坐标
 *              y2   : 待画矩形框端点2的Y坐标
 *              color: 待画矩形框的颜色
 * @retval      无
 */
void lcd_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    lcd_draw_line(x1, y1, x2, y1, color);
    lcd_draw_line(x1, y2, x2, y2, color);
    lcd_draw_line(x1, y1, x1, y2, color);
    lcd_draw_line(x2, y1, x2, y2, color);
}

/**
 * @brief       ATK-MD0430模块LCD画圆形框
 * @param       x    : 待画圆形框原点的X坐标
 *              y    : 待画圆形框原点的Y坐标
 *              r    : 待画圆形框的半径
 *              color: 待画圆形框的颜色
 * @retval      无
 */
void lcd_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    int32_t x_t;
    int32_t y_t;
    int32_t error;
    int32_t error2;
    
    x_t = -r;
    y_t = 0;
    error = 2 - 2 * r;
    
    do {
        lcd_draw_point(x - x_t, y + y_t, color);
        lcd_draw_point(x + x_t, y + y_t, color);
        lcd_draw_point(x + x_t, y - y_t, color);
        lcd_draw_point(x - x_t, y - y_t, color);
        
        error2 = error;
        if (error2 <= y_t)
        {
            y_t++;
            error = error + (y_t * 2 + 1);
            if ((-x_t == y_t) && (error2 <= x_t))
            {
                error2 = 0;
            }
        }
        
        if (error2 > x_t)
        {
            x_t++;
            error = error + (x_t * 2 + 1);
        }
    } while (x_t <= 0);
}

/**
 * @brief       ATK-MD0430模块LCD显示1个字符
 * @param       x    : 待显示字符的X坐标
 *              y    : 待显示字符的Y坐标
 *              ch   : 待显示字符
 *              font : 待显示字符的字体
 *              color: 待显示字符的颜色
 * @retval      无
 */
void lcd_show_char(uint16_t x, uint16_t y, char ch, atk_md0430_lcd_font_t font, uint16_t color)
{
    const uint8_t *ch_code;
    uint8_t ch_width;
    uint8_t ch_height;
    uint8_t ch_size;
    uint8_t ch_offset;
    uint8_t byte_index;
    uint8_t byte_code;
    uint8_t bit_index;
    uint8_t width_index = 0;
    uint8_t height_index = 0;
    
    ch_offset = ch - ' ';
    
    switch (font)
    {
#if (ATK_MD0430_FONT_12 != 0)
        case ATK_MD0430_LCD_FONT_12:
        {
            ch_code = atk_md0430_font_1206[ch_offset];
            ch_width = ATK_MD0430_FONT_12_CHAR_WIDTH;
            ch_height = ATK_MD0430_FONT_12_CHAR_HEIGHT;
            ch_size = ATK_MD0430_FONT_12_CHAR_SIZE;
            break;
        }
#endif
#if (ATK_MD0430_FONT_16 != 0)
        case ATK_MD0430_LCD_FONT_16:
        {
            ch_code = atk_md0430_font_1608[ch_offset];
            ch_width = ATK_MD0430_FONT_16_CHAR_WIDTH;
            ch_height = ATK_MD0430_FONT_16_CHAR_HEIGHT;
            ch_size = ATK_MD0430_FONT_16_CHAR_SIZE;
            break;
        }
#endif
#if (ATK_MD0430_FONT_24 != 0)
        case ATK_MD0430_LCD_FONT_24:
        {
            ch_code = atk_md0430_font_2412[ch_offset];
            ch_width = ATK_MD0430_FONT_24_CHAR_WIDTH;
            ch_height = ATK_MD0430_FONT_24_CHAR_HEIGHT;
            ch_size = ATK_MD0430_FONT_24_CHAR_SIZE;
            break;
        }
#endif
#if (ATK_MD0430_FONT_32 != 0)
        case ATK_MD0430_LCD_FONT_32:
        {
            ch_code = atk_md0430_font_3216[ch_offset];
            ch_width = ATK_MD0430_FONT_32_CHAR_WIDTH;
            ch_height = ATK_MD0430_FONT_32_CHAR_HEIGHT;
            ch_size = ATK_MD0430_FONT_32_CHAR_SIZE;
            break;
        }
#endif
        default:
        {
            return;
        }
    }
    
    if ((x + ch_width > g_atk_md0430_sta.width) || (y + ch_height > g_atk_md0430_sta.height))
    {
        return;
    }
    
    for (byte_index=0; byte_index<ch_size; byte_index++)
    {
        byte_code = ch_code[byte_index];
        for (bit_index=0; bit_index<8; bit_index++)
        {
            if ((byte_code & 0x80) != 0)
            {
                lcd_draw_point(x + width_index, y + height_index, color);
            }
            height_index++;
            if (height_index == ch_height)
            {
                height_index = 0;
                width_index++;
                break;
            }
            byte_code <<= 1;
        }
    }
}

void show_string(uint16_t x, uint16_t y,char *str, atk_md0430_lcd_font_t font, uint16_t color)
{
    uint16_t height;
    switch(font)
    {
        #if (ATK_MD0430_FONT_12 != 0)
        case ATK_MD0430_LCD_FONT_12:
        {
        height = 12;           
        break;
        }
#endif
#if (ATK_MD0430_FONT_16 != 0)
        case ATK_MD0430_LCD_FONT_16:
        {
        height = 16;           
        break;
        }
#endif
#if (ATK_MD0430_FONT_24 != 0)
        case ATK_MD0430_LCD_FONT_24:
        {
        height = 24;           
        break;
        }
#endif
#if (ATK_MD0430_FONT_32 != 0)
        case ATK_MD0430_LCD_FONT_32:
        {
        height = 32;           
        break;
        }
#endif
        default:
        {
            return;
        }

    }
    
    lcd_show_string(x , y , g_atk_md0430_sta.width , height , str , font , color);
}
/**
 * @brief       ATK-MD0430模块LCD显示字符串
 * @note        会自动换行和换页
 * @param       x     : 待显示字符串的X坐标
 *              y     : 待显示字符串的Y坐标
 *              width : 待显示字符串的显示高度
 *              height: 待显示字符串的显示宽度
 *              str   : 待显示字符串
 *              font  : 待显示字符串的字体
 *              color : 待显示字符串的颜色
 * @retval      无
 */
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *str, atk_md0430_lcd_font_t font, uint16_t color)
{
    uint8_t ch_width;
    uint8_t ch_height;
    uint16_t x_raw;
    uint16_t y_raw;
    uint16_t x_limit;
    uint16_t y_limit;
    
    switch (font)
    {
#if (ATK_MD0430_FONT_12 != 0)
        case ATK_MD0430_LCD_FONT_12:
        {
            ch_width = ATK_MD0430_FONT_12_CHAR_WIDTH;
            ch_height = ATK_MD0430_FONT_12_CHAR_HEIGHT;
            break;
        }
#endif
#if (ATK_MD0430_FONT_16 != 0)
        case ATK_MD0430_LCD_FONT_16:
        {
            ch_width = ATK_MD0430_FONT_16_CHAR_WIDTH;
            ch_height = ATK_MD0430_FONT_16_CHAR_HEIGHT;
            break;
        }
#endif
#if (ATK_MD0430_FONT_24 != 0)
        case ATK_MD0430_LCD_FONT_24:
        {
            ch_width = ATK_MD0430_FONT_24_CHAR_WIDTH;
            ch_height = ATK_MD0430_FONT_24_CHAR_HEIGHT;
            break;
        }
#endif
#if (ATK_MD0430_FONT_32 != 0)
        case ATK_MD0430_LCD_FONT_32:
        {
            ch_width = ATK_MD0430_FONT_32_CHAR_WIDTH;
            ch_height = ATK_MD0430_FONT_32_CHAR_HEIGHT;
            break;
        }
#endif
        default:
        {
            return;
        }
    }
    
    x_raw = x;
    y_raw = y;
    x_limit = ((x + width + 1) > g_atk_md0430_sta.width) ? g_atk_md0430_sta.width : (x + width + 1);
    y_limit = ((y + height + 1) > g_atk_md0430_sta.height) ? g_atk_md0430_sta.height : (y + height + 1);
    
    while ((*str >= ' ') && (*str <= '~'))
    {
        if (x + ch_width >= x_limit)
        {
            x = x_raw;
            y += ch_height;
        }
        
        if (y + ch_height >= y_limit)
        {
            y = y_raw;
            x = x_raw;
        }
        
        lcd_show_char(x, y, *str, font, color);
        
        x += ch_width;
        str++;
    }
}

/**
 * @brief       ATK-MD0430模块LCD显示数字，可控制显示高位0
 * @param       x    : 待显示数字的X坐标
 *              y    : 待显示数字的Y坐标
 *              num  : 待显示数字
 *              len  : 待显示数字的位数
 *              mode : ATK_MD0430_NUM_SHOW_NOZERO: 数字高位0不显示
 *                     ATK_MD0430_NUM_SHOW_ZERO  : 数字高位0显示
 *              font : 待显示数字的字体
 *              color: 待显示数字的颜色
 * @retval      无
 */
void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, atk_md0430_num_mode_t mode, atk_md0430_lcd_font_t font, uint16_t color)
{
    uint8_t ch_width;
    uint8_t len_index;
    uint8_t num_index;
    uint8_t first_nozero = 0;
    char pad;
    
    switch (font)
    {
#if (ATK_MD0430_FONT_12 != 0)
        case ATK_MD0430_LCD_FONT_12:
        {
            ch_width = ATK_MD0430_FONT_12_CHAR_WIDTH;
            break;
        }
#endif
#if (ATK_MD0430_FONT_16 != 0)
        case ATK_MD0430_LCD_FONT_16:
        {
            ch_width = ATK_MD0430_FONT_16_CHAR_WIDTH;
            break;
        }
#endif
#if (ATK_MD0430_FONT_24 != 0)
        case ATK_MD0430_LCD_FONT_24:
        {
            ch_width = ATK_MD0430_FONT_24_CHAR_WIDTH;
            break;
        }
#endif
#if (ATK_MD0430_FONT_32 != 0)
        case ATK_MD0430_LCD_FONT_32:
        {
            ch_width = ATK_MD0430_FONT_32_CHAR_WIDTH;
            break;
        }
#endif
        default:
        {
            return;
        }
    }
    
    switch (mode)
    {
        case ATK_MD0430_NUM_SHOW_NOZERO:
        {
            pad = ' ';
            break;
        }
        case ATK_MD0430_NUM_SHOW_ZERO:
        {
            pad = '0';
            break;
        }
        default:
        {
            return;
        }
    }
    
    for (len_index=0; len_index<len; len_index++)
    {
        num_index = (num / atk_md0430_pow(10, len - len_index - 1)) % 10;
        if ((first_nozero == 0) && (len_index < (len - 1)))
        {
            if (num_index == 0)
            {
                lcd_show_char(x + ch_width * len_index, y, pad, font, color);
                continue;
            }
            else
            {
                first_nozero = 1;
            }
        }
        
        lcd_show_char(x + ch_width * len_index, y, num_index + '0', font, color);
    }
}

/**
 * @brief       ATK-MD0430模块LCD显示数字，不显示高位0
 * @param       x    : 待显示数字的X坐标
 *              y    : 待显示数字的Y坐标
 *              num  : 待显示数字
 *              len  : 待显示数字的位数
 *              font : 待显示数字的字体
 *              color: 待显示数字的颜色
 * @retval      无
 */
void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, atk_md0430_lcd_font_t font, uint16_t color)
{
    lcd_show_xnum(x, y, num, len, ATK_MD0430_NUM_SHOW_NOZERO, font, color);
}

/**
 * @brief       ATK-MD0430模块LCD图片
 * @note        图片取模方式: 水平扫描、RGB565、高位在前
 * @param       x     : 待显示图片的X坐标
 *              y     : 待显示图片的Y坐标
 *              width : 待显示图片的宽度
 *              height: 待显示图片的高度
 *              pic   : 待显示图片数组首地址
 * @retval      无
 */
void lcd_show_pic(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *pic)
{
    uint16_t x_index;
    uint16_t y_index;
    
    if ((x + width > g_atk_md0430_sta.width) || (y + height > g_atk_md0430_sta.height))
    {
        return;
    }
    
    atk_md0430_set_column_address(x, x + width - 1);
    atk_md0430_set_page_address(y, y + height - 1);
    atk_md0430_start_write_memory();
    for (y_index=y; y_index<=(y + height); y_index++)
    {
        for (x_index=x; x_index<=(x + width); x_index++)
        {
            lcd_wr_data(*pic);
            pic++;
        }
    }
}