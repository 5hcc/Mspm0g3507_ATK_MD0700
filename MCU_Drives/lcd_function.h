#ifndef __LCD_FUNCTION_H__
#define __LCD_FUNCTION_H__

#include "lcd.h"

void draw_hand(uint16_t color);
void show_coordinates(uint16_t color);
uint16_t set_bottom(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t buttom_number, uint16_t color);

#endif