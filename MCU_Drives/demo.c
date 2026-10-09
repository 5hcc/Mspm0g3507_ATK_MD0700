// #include "demo.h"
// #include "lcd.h"
// #include <math.h>
// #include <stdlib.h>

// #define PI (float)(3.1415926)

// static float cube[8][3] = {
//     {-32, -32, -32},
//     {-32, +32, -32},
//     {+32, +32, -32},
//     {+32, -32, -32},
//     {-32, -32, +32},
//     {-32, +32, +32},
//     {+32, +32, +32},
//     {+32, -32, +32}
// };

// static uint8_t line_id[24] = {
//     1, 2, 2, 3,
//     3, 4, 4, 1,
//     5, 6, 6, 7,
//     7, 8, 8, 5,
//     8, 4, 7, 3,
//     6, 2, 5, 1
// };

// /**
//  * @brief       �������˷�
//  * @param       a      : ����a
//  *              b[3][3]: ����b
//  * @retval      ������
//  */
// static float *demo_matconv(float *a, float b[3][3])
// {
//     float res[3];
//     uint8_t res_index;
//     uint8_t a_index;
    
//     for (res_index=0; res_index<3; res_index++)
//     {
//         res[res_index] = b[res_index][0] * a[0] + b[res_index][1] * a[1] + b[res_index][2] * a[2];
//     }
    
//     for (a_index=0; a_index<3; a_index++)
//     {
//         a[a_index] = res[a_index];
//     }
    
//     return a;
// }

// /**
//  * @brief       ��ת����
//  * @param       point: ��Ҫ��ת������
//  *              x    : X����ת��
//  *              y    : Y����ת��
//  *              z    : Z����ת��
//  * @retval      ������
//  */
// static void demo_rotate(float *point, float x, float y, float z)
// {
//     float rx[3][3];
//     float ry[3][3];
//     float rz[3][3];
    
//     x /= PI;
//     y /= PI;
//     z /= PI;
    
//     rx[0][0] = cos(x);
//     rx[0][1] = 0;
//     rx[0][2] = sin(x);
//     rx[1][0] = 0;
//     rx[1][1] = 1;
//     rx[1][2] = 0;
//     rx[2][0] = -sin(x);
//     rx[2][1] = 0;
//     rx[2][2] = cos(x);
    
//     ry[0][0] = 1;
//     ry[0][1] = 0;
//     ry[0][2] = 0;
//     ry[1][0] = 0;
//     ry[1][1] = cos(y);
//     ry[1][2] = -sin(y);
//     ry[2][0] = 0;
//     ry[2][1] = sin(y);
//     ry[2][2] = cos(y);
    
//     rz[0][0] = cos(z);
//     rz[0][1] = -sin(z);
//     rz[0][2] = 0;
//     rz[1][0] = sin(z);
//     rz[1][1] = cos(z);
//     rz[1][2] = 0;
//     rz[2][0] = 0;
//     rz[2][1] = 0;
//     rz[2][2] = 1;
    
//     demo_matconv(demo_matconv(demo_matconv(point, rz), ry), rx);
// }

// /**
//  * @brief       �ֻ滭�ߺ�����ģ���ֻ�Ч��
//  * @param       x1     : ���x����
//  *              y1     : ���y����
//  *              x2     : �յ�x����
//  *              y2     : �յ�y����
//  *              color  : ��ɫ
//  * @retval      ��
//  */
// static void draw_hand_drawn_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
// {
//     int dx = (x2 - x1) < 0 ? -(x2 - x1) : (x2 - x1);
//     int dy = (y2 - y1) < 0 ? -(y2 - y1) : (y2 - y1);
//     int sx = (x1 < x2) ? 1 : -1;
//     int sy = (y1 < y2) ? 1 : -1;
//     int err = dx - dy;
//     int e2;
//     int steps = 0;
//     int max_steps = dx + dy;  // �����

//     while (1)
//     {
//         // ���ֵ㣨2x2��
//         for (int dx = 0; dx < 2; dx++)
//         {
//             for (int dy = 0; dy < 2; dy++)
//             {
//                 int px = x1 + dx;
//                 int py = y1 + dy;
//                 if (px >= 0 && px < LCD_WIDTH && py >= 0 && py < LCD_HEIGHT)
//                 {
//                     lcd_draw_point(px, py, color);
//                 }
//             }
//         }

//         if (x1 == x2 && y1 == y2)
//             break;

//         e2 = 2 * err;
//         if (e2 > -dy)
//         {
//             err -= dy;
//             x1 += sx;
//         }
//         if (e2 < dx)
//         {
//             err += dx;
//             y1 += sy;
//         }

//         steps++;
//         if (steps > max_steps * 2)  // ��ֹ����ѭ��
//             break;
//     }
// }

// /**
//  * @brief       ��ʾ������3D��ת
//  * @param       ��
//  * @retval      ��
//  */
// static void demo_show_cube(void)
// {
//     uint8_t point_index;
//     uint8_t line_index;
//     static uint16_t x = 240;
//     static uint16_t y = 441;
//     static atk_md0430_touch_point_t point[1];
    
//     /* ATK-MD0430ģ�鴥��ɨ�� */
//     if (atk_md0430_touch_scan(point, sizeof(point) / sizeof(point[0])) == sizeof(point) / sizeof(point[0]))
//     {
//         if ((point[0].x > 56) && (point[0].x < (lcd_get_width() - 56)) && (point[0].y > 138) && (point[0].y < (lcd_get_height() - 56)))
//         {
//             x = point[0].x;
//             y = point[0].y;
//         }
//     }
    
//     for (point_index=0; point_index<8; point_index++)
//     {
//         demo_rotate(cube[point_index], 0.05f, 0.03f, 0.02f);
//     }
    
//     for (line_index=0; line_index<24; line_index+=2)
//     {
//         /* ATK-MD0430ģ��LCD�ֻ滭�߶� */
//         draw_hand_drawn_line(   x + cube[line_id[line_index] - 1][0],
//                                 y + cube[line_id[line_index] - 1][1],
//                                 x + cube[line_id[line_index + 1] - 1][0],
//                                 y + cube[line_id[line_index + 1] - 1][1],
//                                 ATK_MD0430_BLUE);
//     }
    
//     delay_ms(20);
    
// //    lcd_fill(x - 56, y - 56, x + 56, y + 56, ATK_MD0430_WHITE);
// }

// /**
//  * @brief       �ֻ���ʾ
//  * @param       ��
//  * @retval      ��
//  */
// static void demo_draw_hand(void)
// {
//     static atk_md0430_touch_point_t last_point = {0, 0, 0};  // ��һ��������
//     atk_md0430_touch_point_t current_point[1];
    
//     /* ATK-MD0430ģ�鴥��ɨ�� */
//     if (atk_md0430_touch_scan(current_point, sizeof(current_point) / sizeof(current_point[0])) == sizeof(current_point) / sizeof(current_point[0]))
//     {
//         // ��鴥�����Ƿ�����Ч��Χ��
//         if ((current_point[0].x > 0) && (current_point[0].x < LCD_WIDTH) && 
//             (current_point[0].y > 0) && (current_point[0].y < LCD_HEIGHT))
//         {
//             // �������һ���㣬���ƴ���һ���㵽��ǰ�����
//             if (last_point.x != 0 || last_point.y != 0)
//             {
//                 // ������룬�������󣬻��м��������ȱ
//                 int dx = current_point[0].x - last_point.x;
//                 int dy = current_point[0].y - last_point.y;
//                 int distance = (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy);  // �����پ���
//                 if (distance > 10)  // ����������10�����м��
//                 {
//                     int mid_x = (last_point.x + current_point[0].x) / 2;
//                     int mid_y = (last_point.y + current_point[0].y) / 2;
//                     draw_hand_drawn_line(last_point.x, last_point.y, mid_x, mid_y, ATK_MD0430_BLACK);
//                     draw_hand_drawn_line(mid_x, mid_y, current_point[0].x, current_point[0].y, ATK_MD0430_BLACK);
//                 }
//                 else
//                 {
//                     draw_hand_drawn_line(last_point.x, last_point.y, current_point[0].x, current_point[0].y, ATK_MD0430_BLACK);
//                 }
//             }
//             //һ
//             last_point = current_point[0];
//         }
//     }
//     else
//     {
//         // û�д�����������һ����
//         last_point.x = 0;
//         last_point.y = 0;
//     }
    
//     delay_ms(10);  // �����ӳ٣�����ɨ��Ƶ��
// }

// /**
//  * @brief       ������ʾ��ں���
//  * @param       ��
//  * @retval      ��
//  */
// void demo_run(void)
// {
//     uint8_t ret;
    
//     /* ��ʼ��ATK-MD0430ģ�� */
//     ret = lcd_init();
//     if (ret != 0)
//     {
//         // printf("ATK-MD0430 init failed!\r\n");
//         while (1)
//         {
//             delay_ms(200);
//         }
//     }
    
//     /* ATK-MD0430ģ��LCD���� */
//     lcd_clear(ATK_MD0430_WHITE);
//     /* ATK-MD0430ģ��LCD��ʾ�ַ��� */
//     lcd_show_string(10, 10, LCD_WIDTH, 32, "MSPM0G3507", ATK_MD0430_LCD_FONT_32, ATK_MD0430_RED);
//     lcd_show_string(10, 42, LCD_WIDTH, 24, "ATK-MD0430", ATK_MD0430_LCD_FONT_24, ATK_MD0430_RED);    
//     while (1)
//     {
//         /* �ֻ���ʾ */
//         demo_show_cube();
//     }
// }

/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MD0700模块测试实验（GPIO）
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 MiniSTM32 V4开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "demo.h"
#include "lcd.h"
#include <math.h>

#define PI (float)(3.1415926)

static float cube[8][3] = {
    {-32, -32, -32},
    {-32, +32, -32},
    {+32, +32, -32},
    {+32, -32, -32},
    {-32, -32, +32},
    {-32, +32, +32},
    {+32, +32, +32},
    {+32, -32, +32}
};

static uint8_t line_id[24] = {
    1, 2, 2, 3,
    3, 4, 4, 1,
    5, 6, 6, 7,
    7, 8, 8, 5,
    8, 4, 7, 3,
    6, 2, 5, 1
};

/**
 * @brief       计算矩阵乘法
 * @param       a      : 矩阵a
 *              b[3][3]: 矩阵b
 * @retval      计算结果
 */
static float *demo_matconv(float *a, float b[3][3])
{
    float res[3];
    uint8_t res_index;
    uint8_t a_index;
    
    for (res_index=0; res_index<3; res_index++)
    {
        res[res_index] = b[res_index][0] * a[0] + b[res_index][1] * a[1] + b[res_index][2] * a[2];
    }
    
    for (a_index=0; a_index<3; a_index++)
    {
        a[a_index] = res[a_index];
    }
    
    return a;
}

/**
 * @brief       旋转向量
 * @param       point: 需要旋转的向量
 *              x    : X轴旋转量
 *              y    : Y轴旋转量
 *              z    : Z轴旋转量
 * @retval      计算结果
 */
static void demo_rotate(float *point, float x, float y, float z)
{
    float rx[3][3];
    float ry[3][3];
    float rz[3][3];
    
    x /= PI;
    y /= PI;
    z /= PI;
    
    rx[0][0] = cos(x);
    rx[0][1] = 0;
    rx[0][2] = sin(x);
    rx[1][0] = 0;
    rx[1][1] = 1;
    rx[1][2] = 0;
    rx[2][0] = -sin(x);
    rx[2][1] = 0;
    rx[2][2] = cos(x);
    
    ry[0][0] = 1;
    ry[0][1] = 0;
    ry[0][2] = 0;
    ry[1][0] = 0;
    ry[1][1] = cos(y);
    ry[1][2] = -sin(y);
    ry[2][0] = 0;
    ry[2][1] = sin(y);
    ry[2][2] = cos(y);
    
    rz[0][0] = cos(z);
    rz[0][1] = -sin(z);
    rz[0][2] = 0;
    rz[1][0] = sin(z);
    rz[1][1] = cos(z);
    rz[1][2] = 0;
    rz[2][0] = 0;
    rz[2][1] = 0;
    rz[2][2] = 1;
    
    demo_matconv(demo_matconv(demo_matconv(point, rz), ry), rx);
}

/**
 * @brief       演示立方体3D旋转
 * @param       无
 * @retval      无
 */
static void demo_show_cube(void)
{
    uint8_t point_index;
    uint8_t line_index;
    static uint16_t x = 240;
    static uint16_t y = 441;
    static atk_md0700_touch_point_t point[1];
    
    /* ATK-MD0700模块触摸扫描 */
    if (atk_md0700_touch_scan(point, sizeof(point) / sizeof(point[0])) == sizeof(point) / sizeof(point[0]))
    {
        if ((point[0].x > 56) && (point[0].x < (lcd_get_width() - 56)) && (point[0].y > 138) && (point[0].y < (lcd_get_height() - 56)))
        {
            x = point[0].x;
            y = point[0].y;
        }
    }
    
    for (point_index=0; point_index<8; point_index++)
    {
        demo_rotate(cube[point_index], 0.05f, 0.03f, 0.02f);
    }
    
    for (line_index=0; line_index<24; line_index+=2)
    {
        /* ATK-MD0700模块LCD画线段 */
        lcd_draw_line(   x + cube[line_id[line_index] - 1][0],
                                y + cube[line_id[line_index] - 1][1],
                                x + cube[line_id[line_index + 1] - 1][0],
                                y + cube[line_id[line_index + 1] - 1][1],
                                ATK_MD0430_BLUE);
    }
    
    delay_ms(20);
    
    lcd_fill(x - 56, y - 56, x + 56, y + 56, ATK_MD0430_WHITE);
}

/**
 * @brief       例程演示入口函数
 * @param       无
 * @retval      无
 */
void demo_run(void)
{
    uint8_t ret;
    
    /* 初始化ATK-MD0700模块 */
    ret = lcd_init();
    // if (ret != 0)
    // {
    //     // printf("ATK-MD0700 init failed!\r\n");
    //     lcd_show_string(10, 10, LCD_WIDTH, 32, "err",         ATK_MD0430_LCD_FONT_32, ATK_MD0430_RED);

    //     while (1)
    //     {
    //         delay_ms(200);
    //     }
    // }
    
    /* ATK-MD0700模块LCD清屏 */
    lcd_clear(ATK_MD0430_WHITE);
    /* ATK-MD0700模块LCD显示字符串 */
    lcd_show_string(10, 10, LCD_WIDTH, 32, "STM32",         ATK_MD0430_LCD_FONT_32, ATK_MD0430_RED);
    lcd_show_string(10, 42, LCD_WIDTH, 24, "ATK-MD0700",    ATK_MD0430_LCD_FONT_24, ATK_MD0430_RED);
    lcd_show_string(10, 66, LCD_WIDTH, 16, "ATOM@ALIENTEK", ATK_MD0430_LCD_FONT_16, ATK_MD0430_RED);
    
    while (1)
    {
        /* 演示立方体3D旋转 */
        demo_show_cube();
    }
}
