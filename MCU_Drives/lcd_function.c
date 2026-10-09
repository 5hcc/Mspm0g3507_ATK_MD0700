#include "lcd_function.h"

    static atk_md0430_touch_point_t md0430_last_point = {0, 0, 0};  // 上一个触摸点
    atk_md0430_touch_point_t md0430_current_point[1];
  
    static atk_md0700_touch_point_t md0700_last_point = {0, 0, 0};  // 上一个触摸点
    atk_md0700_touch_point_t md0700_current_point[1];

/**
 * @brief       手绘演示
 * @param       无
 * @retval      无
 */
void draw_hand(uint16_t color)
{
    /* ATK-MD0430模块触摸扫描 */
    if(device == 1)
    {
    if (atk_md0430_touch_scan(md0430_current_point, sizeof(md0430_current_point) / sizeof(md0430_current_point[0])) == sizeof(md0430_current_point) / sizeof(md0430_current_point[0]))
    {
        // 检查触摸点是否在有效范围内
        if ((md0430_current_point[0].x > 0) && (md0430_current_point[0].x < LCD_WIDTH) && 
            (md0430_current_point[0].y > 0) && (md0430_current_point[0].y < LCD_HEIGHT))
        {
            // 如果有上一个点，绘制从上一个点到当前点的线
            if (md0430_last_point.x != 0 || md0430_last_point.y != 0)
            {
                // 计算距离，如果距离大，画中间点来填充空缺
                int dx = md0430_current_point[0].x - md0430_last_point.x;
                int dy = md0430_current_point[0].y - md0430_last_point.y;
                int distance = (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy);  // 曼哈顿距离
                if (distance > 10)  // 如果距离大于10，画中间点
                {
                    int mid_x = (md0430_last_point.x + md0430_current_point[0].x) / 2;
                    int mid_y = (md0430_last_point.y + md0430_current_point[0].y) / 2;
                    lcd_draw_line(md0430_last_point.x, md0430_last_point.y, mid_x, mid_y, color);
                    lcd_draw_line(mid_x, mid_y, md0430_current_point[0].x, md0430_current_point[0].y, color);
                }
                else
                {
                    lcd_draw_line(md0430_last_point.x, md0430_last_point.y, md0430_current_point[0].x, md0430_current_point[0].y, color);
                }
            }
            // 更新上一个点
            md0430_last_point = md0430_current_point[0];
        }
    }
    else
    {
        // 没有触摸，重置上一个点
        md0430_last_point.x = 0;
        md0430_last_point.y = 0;
    }
    
    delay_ms(10);  // 短暂延迟，增加扫描频率
    }
    else if(device == 2)
    {
        if (atk_md0700_touch_scan(md0700_current_point, sizeof(md0700_current_point) / sizeof(md0700_current_point[0])) == sizeof(md0700_current_point) / sizeof(md0700_current_point[0]))
    {
        // 检查触摸点是否在有效范围内
        if ((md0700_current_point[0].x > 0) && (md0700_current_point[0].x < LCD_WIDTH) && 
            (md0700_current_point[0].y > 0) && (md0700_current_point[0].y < LCD_HEIGHT))
        {
            // 如果有上一个点，绘制从上一个点到当前点的线
            if (md0700_last_point.x != 0 || md0700_last_point.y != 0)
            {
                // 计算距离，如果距离大，画中间点来填充空缺
                int dx = md0700_current_point[0].x - md0700_last_point.x;
                int dy = md0700_current_point[0].y - md0700_last_point.y;
                int distance = (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy);  // 曼哈顿距离
                if (distance > 10)  // 如果距离大于10，画中间点
                {
                    int mid_x = (md0700_last_point.x + md0700_current_point[0].x) / 2;
                    int mid_y = (md0700_last_point.y + md0700_current_point[0].y) / 2;
                    lcd_draw_line(md0700_last_point.x, md0700_last_point.y, mid_x, mid_y, color);
                    lcd_draw_line(mid_x, mid_y, md0700_current_point[0].x, md0700_current_point[0].y, color);
                }
                else
                {
                    lcd_draw_line(md0700_last_point.x, md0700_last_point.y, md0700_current_point[0].x, md0700_current_point[0].y, color);
                }
            }
            // 更新上一个点
            md0700_last_point = md0700_current_point[0];
        }
    }
    else
    {
        // 没有触摸，重置上一个点
        md0700_last_point.x = 0;
        md0700_last_point.y = 0;
    }
    
    delay_ms(10);  // 短暂延迟，增加扫描频率
    }
}

/**
 * @brief       坐标显示
 * @param       无
 * @retval      无
 */
void show_coordinates(uint16_t color)
{

    uint16_t data[2] = {0};

    if(device == 1)
    {
        if (atk_md0430_touch_scan(md0430_current_point, sizeof(md0430_current_point) / sizeof(md0430_current_point[0])) == sizeof(md0430_current_point) / sizeof(md0430_current_point[0]))
        {
            // 检查触摸点是否在有效范围内
            if ((md0430_current_point[0].x > 0) && (md0430_current_point[0].x < LCD_WIDTH) && 
                (md0430_current_point[0].y > 0) && (md0430_current_point[0].y < LCD_HEIGHT))
            {
                if (md0430_last_point.x != 0 || md0430_last_point.y != 0)
                {
                    lcd_fill(300, 0, 479, 50, ATK_MD0430_WHITE);
                    lcd_show_num(400-20, 10, md0430_current_point[0].x, 3, ATK_MD0430_LCD_FONT_24, color);
                    lcd_show_char(440-20, 10, ',', ATK_MD0430_LCD_FONT_24, color);
                    lcd_show_num(450-20, 10, md0430_current_point[0].y, 3, ATK_MD0430_LCD_FONT_24, color);
                }
                // 更新上一个点
                md0430_last_point = md0430_current_point[0];
            }
        }
        else
        {
            // 没有触摸，重置上一个点
            md0430_last_point.x = 0;
            md0430_last_point.y = 0;
        }
        
        delay_ms(10);  // 短暂延迟，增加扫描频率
    }

    if(device == 2)
    {
        if (atk_md0700_touch_scan(md0700_current_point, sizeof(md0700_current_point) / sizeof(md0700_current_point[0])) == sizeof(md0700_current_point) / sizeof(md0700_current_point[0]))
        {
            // 检查触摸点是否在有效范围内
            if ((md0700_current_point[0].x > 0) && (md0700_current_point[0].x < LCD_WIDTH) && 
                (md0700_current_point[0].y > 0) && (md0700_current_point[0].y < LCD_HEIGHT))
            {
                if (md0700_last_point.x != 0 || md0700_last_point.y != 0)
                {
                    lcd_fill(300, 0, 479, 50, ATK_MD0430_WHITE);
                    lcd_show_num(400-20, 10, md0700_current_point[0].x, 3, ATK_MD0430_LCD_FONT_24, color);
                    lcd_show_char(440-20, 10, ',', ATK_MD0430_LCD_FONT_24, color);
                    lcd_show_num(450-20, 10, md0700_current_point[0].y, 3, ATK_MD0430_LCD_FONT_24, color);
                }
                // 更新上一个点
                md0700_last_point = md0700_current_point[0];
            }
        }
        else
        {
            // 没有触摸，重置上一个点
            md0700_last_point.x = 0;
            md0700_last_point.y = 0;
        }
        
        delay_ms(10);  // 短暂延迟，增加扫描频率
    }
}

uint16_t set_bottom(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t buttom_number, uint16_t color)
{

    lcd_draw_rect(x1, y1, x2, y2, color);
    lcd_show_num((x1 + 5), (y1 + 5), buttom_number, 2, ATK_MD0430_LCD_FONT_12, color);

    if(device == 1)
    {
        if (atk_md0430_touch_scan(md0430_current_point, sizeof(md0430_current_point) / sizeof(md0430_current_point[0])) == sizeof(md0430_current_point) / sizeof(md0430_current_point[0]))
            {
                // 检查触摸点是否在有效范围内
                if ((md0430_current_point[0].x >= x1) && (md0430_current_point[0].x <= x2) &&
                    (md0430_current_point[0].y >= y1) && (md0430_current_point[0].y <= y2))
                {
                    delay_ms(10);  // 短暂延迟，增加扫描频率
                    return buttom_number;
                }
            }
            else
            {
                delay_ms(10);
                return 0;
            }

            delay_ms(10);  // 短暂延迟，增加扫描频率
            return 0;
    }

    if(device == 2)
    {
        if (atk_md0700_touch_scan(md0700_current_point, sizeof(md0700_current_point) / sizeof(md0700_current_point[0])) == sizeof(md0700_current_point) / sizeof(md0700_current_point[0]))
            {
                // 检查触摸点是否在有效范围内
                if ((md0700_current_point[0].x >= x1) && (md0700_current_point[0].x <= x2) &&
                    (md0700_current_point[0].y >= y1) && (md0700_current_point[0].y <= y2))
                {
                    delay_ms(10);  // 短暂延迟，增加扫描频率
                    return buttom_number;
                }
            }
            else
            {
                delay_ms(10);
                return 0;
            }

            delay_ms(10);  // 短暂延迟，增加扫描频率
            return 0;
    }
    
    else return 0;
}