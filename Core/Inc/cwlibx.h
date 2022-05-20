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

#define LCD_ERASE_HORIZONTAL_BAR_GRAPH 		43
#define LCD_ERASE_VERTICAL_BAR_GRAPH 		45
#define LCD_MODEL_NUMBER					48
#define LCD_FIRMWARE_NUMBER					49
#define LCD_INIT_CHINESE_S  				55
#define LCD_INIT_CHINESE_T  				56
#define LCD_SET_BAUD    					57
#define LCD_DRAW_VERTICAL_BAR_GRAPH 		61
#define LCD_BACKLIGHT_BRIGHTNESS  			65
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
#define LCD_DEFINE_CUSTOM_CHARACTER			78
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
#define LCD_PUT_BYTE						62
#define LCD_DEF_THIN_VERTICAL_BAR			115
#define LCD_DEF_THICK_VERTICAL_BAR			118
#define LCD_DRAW_HORIZONTAL_BAR_GRAPH 		124
#define LCD_CMD      						254
#define LCD_CMD_END   						253

#define N_LINES 							5
#define MAX_CHARS_ONSCREEN 					21

#define CUSTOM_CHARACTER_SIZE 				6
#define CUSTOM_CHARACTER_BUFFER_SIZE		16

#define N 8
#define MASK_BUFFER 105


static bool text_invertion = false;
static bool text_wrap = false;
static uint16_t LCD_BRIGHT = 59999;
static uint8_t cursor[2] = { 0, 0 };
static uint8_t ddp[2] = { 3, 141 };
static uint8_t vertical_bar_width = 5;
static uint8_t ESP_ENTRE_LINHAS = 2;


typedef struct
{
    uint8_t wrap_str[N_LINES][MAX_CHARS_ONSCREEN];
    uint8_t wrap_times;

} txt_wrap_t;

typedef struct{
uint8_t custom_caracter[CUSTOM_CHARACTER_SIZE];
uint8_t custo_character_index;
} custom_character_t;

uint16_t exp(uint16_t val,uint16_t pot );
void decode_convert_expand_encode(uint8_t *hex_char);
void init_custom_character_db(void);
void inverse_text(bool state);
void set_contrast(uint8_t contrast);
void lcd_soft_reset(void);
void enable_backlight(bool enable);
void set_backlight_brightness(uint8_t bright);
void clear_display(void);
void auto_scroll(uint8_t enable);
void text_insertion_point(uint8_t col, uint8_t row);
void put_pixel(uint8_t x, uint8_t y);
void erase_pixel(uint8_t x, uint8_t y);
void put_byte(uint8_t x, uint8_t row, uint8_t byte);
void define_custom_character(uint8_t *cmd);
void disp_splash(void);
void put_Ucursor(bool enable,uint8_t col, uint8_t row,uint8_t hoover );
void lcd_print(uint8_t *txt);
void def_v_bar_thickness(uint8_t thick);
void draw_un_v_bar_graph(uint8_t col, uint8_t height, bool erase);
void draw_un_h_bar_graph(uint8_t col, uint8_t row, uint8_t lenght , bool erase);
void str_warper(txt_wrap_t *wrap, uint8_t *txt);
void custom_character_dealer( uint8_t *txt);
bool Custom_Character_masker(uint8_t *txt,uint8_t * mask, uint8_t *v_cursor);
void clean_it(uint8_t *str);

#endif /* INC_CWLIBX_H_ */
