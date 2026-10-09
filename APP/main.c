#include "ti_msp_dl_config.h"
#include "lcd.h"
#include "demo.h"
#include "lcd_function.h"

uint16_t touch;
uint16_t touch1;

int main(void)
{
	DL_FlashCTL_executeClearStatus(FLASHCTL);
	SYSCFG_DL_init();
	// demo_run();
	uint8_t device = lcd_init();
	show_string(10, 10, "MSPM0G3507", ATK_MD0430_LCD_FONT_32, ATK_MD0430_RED);
	if(device == 1)
    show_string(10, 42, "ATK_MD0430_TOUCH_EOK", ATK_MD0430_LCD_FONT_24, ATK_MD0430_GREEN);    
	else if(device == 2)
	show_string(10, 42, "ATK_MD0700_TOUCH_EOK", ATK_MD0430_LCD_FONT_24, ATK_MD0430_GREEN);
	else show_string(460, 42, "TOUCH_ERR", ATK_MD0430_LCD_FONT_24, ATK_MD0430_RED);
	while(1)
	{
		show_coordinates(ATK_MD0430_RED);
		touch = set_bottom(400 , 200 ,450 , 250 , 1 , ATK_MD0430_BLUE);
		touch1 = set_bottom(200 , 200 ,250 , 250 , 2 , ATK_MD0430_BLUE);
		if(touch == 1)
		lcd_fill(200,400,300,500,ATK_MD0430_YELLOW);
		else if(touch1 == 2)
		lcd_fill(200,400,300,500,ATK_MD0430_GREEN);
		else 
		lcd_fill(200,400,300,500,ATK_MD0430_WHITE);
	} 
}
	
