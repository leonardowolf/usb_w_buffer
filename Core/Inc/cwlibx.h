/*
 * cwlinux_lib.h
 *
 *  Created on: Jan 17, 2022
 *      Author: hugin
 */

#ifndef INC_CWLIBX_H_
#define INC_CWLIBX_H_

#include <stdbool.h>

#define fw_version 							"1.0"
#define model_version 						"1.0"
#define CW_DEFAULT_BAUD						19200
#define OPTIMAL_CONTRAST					4

#define display_w 							192
#define display_h 							64

#define LCD_MODEL_NUMBER					48
#define LCD_FIRMWARE_NUMBER					49
#define LCD_INIT_CHINESE_S  				55
#define LCD_INIT_CHINESE_T  				56
#define LCD_SET_BAUD    					57
#define LCD_DRAW_VERTICAL_BAR_GRAPH 		61
#define LCD_BACKLIGHT_BRIGHTNESS  			64
#define LCD_BACKLIGHT_ON    				66
#define LCD_WRAP_ENABLE   					67
#define LCD_WRAP_DISABLE  					68
#define LCD_BACKLIGHT_OFF   				70
#define LCD_SET_INSERT    					71  /* go to X,Y */
#define LCD_HOME_INSERT   					72  /* go to home */
#define LCD_UNDERLINE_CURSOR_ON 			74  /* set cursor on at X,Y */
#define LCD_UNDERLINE_CURSOR_OFF  			75
#define LCD_MOVE_CURSOR_LEFT  				76
#define LCD_MOVE_CURSOR_RIGHT 				77
#define LCD_SETCHAR   						78
#define LCD_SCROLL_ENABLE 					81
#define LCD_SCROLL_DISABLE   				82
#define LCD_SOFT_RESET    					86
#define LCD_CLEAR   						88
#define LCD_READ_GPI						101
#define LCD_INVERSE_TEXT_ON 				102
#define LCD_INVERSE_TEXT_OFF  				103
#define LCD_DISPLAY_SPLASH					105
#define LCD_PUT_PIXEL   					112
#define LCD_CLEAR_PIXEL   					113
#define LCD_DEF_THIN_VERTICAL_BAR			115
#define LCD_DEF_THICK_VERTICAL_BAR			118
#define LCD_DRAW_HORIZONTAL_BAR_GRAPH 		124
#define LCD_CMD      						254
#define LCD_CMD_END   						253

static bool text_invertion = false;
static bool text_wrap = false;
static uint8_t cursor[2] = { 0, 0 };
static uint8_t ddp[2] = { 3, 141 };
static uint8_t vertical_bar_width = 5;

typedef struct {

	uint8_t *wrap_str[5];
	uint8_t wrap_times;

} txt_wrap;

void inverse_text(bool state);
void set_contrast(uint8_t contrast);
void lcd_soft_reset(void);
void enable_backlight(bool enable);
void clear_display(void);
void auto_scroll(uint8_t enable);
void text_insertion_point(uint8_t col, uint8_t row);
void put_pixel(uint8_t x, uint8_t y);
void erase_pixel(uint8_t x, uint8_t y);
void disp_splash(void);
void put_cursor(void);
void lcd_print(uint8_t *txt);
void def_thin_v_bar(void);
void def_thick_v_bar(void);
void draw_v_bar_graph(uint8_t col, uint8_t height);
void erase_v_bar_graph(uint8_t col, uint8_t height);
void draw_h_bar_graph(uint8_t col, uint8_t row, uint8_t lenght);
void str_warper(txt_wrap *wrap, uint8_t *txt);

#endif /* INC_CWLIBX_H_ */
