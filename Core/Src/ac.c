/*
 * app.c
 *
 *  Created on: Apr 18, 2022
 *      Author: munin
 */

#include <ac.h>
#include <com_monitor.h>
#include "stdlib.h"
#include "stdint.h"
#include <stdio.h>
#include <string.h>
#include "stdbool.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "u8g2.h"
#include "cwlibx.h"
#include "usbd_cdc_if.h"

extern SPI_HandleTypeDef hspi1;
u8g2_t u8g2;

uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
U8X8_UNUSED void *arg_ptr) {
	switch (msg) {
	case U8X8_MSG_GPIO_AND_DELAY_INIT:
		HAL_Delay(1);
		break;
	case U8X8_MSG_DELAY_MILLI:
		HAL_Delay(arg_int);
		break;
	case U8X8_MSG_GPIO_DC:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, arg_int);
		break;
	case U8X8_MSG_GPIO_RESET:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, arg_int);
		break;
	}
	return 1;
}

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
		void *arg_ptr) {
	switch (msg) {
	case U8X8_MSG_BYTE_SEND:
		HAL_SPI_Transmit(&hspi1, (uint8_t*) arg_ptr, arg_int, 10000);
		break;
	case U8X8_MSG_BYTE_INIT:
		break;
	case U8X8_MSG_BYTE_SET_DC:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, arg_int);
		break;
	case U8X8_MSG_BYTE_START_TRANSFER:
		break;
	case U8X8_MSG_BYTE_END_TRANSFER:
		break;
	default:
		return 0;
	}
	return 1;
}

void Start_APP(void){

	//	u8g2_Setup_uc1604_jlx19264_1(&u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi,
	//			u8x8_stm32_gpio_and_delay);

		u8g2_Setup_uc1604_jlx19264_f(&u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi,
				u8x8_stm32_gpio_and_delay);//[full framebuffer, size = 1536 bytes]


		u8g2_InitDisplay(&u8g2);
		u8g2_SetPowerSave(&u8g2, 0);
		set_contrast(OPTIMAL_CONTRAST);

		u8g2_SetDrawColor(&u8g2, 2);
		u8g2_SetBitmapMode(&u8g2, 1);
		u8g2_SetFontMode(&u8g2, 1);
		u8g2_SetFontPosTop(&u8g2);

	/*	U8g2 Font names
		<prefix> '_' <name> '_' <purpose> <char set>
		<purpose>	Description
		t	Transparent font, Do not use a background color.
		h	All glyphs have common height.
		m	All glyphs have common height and width (monospace).
		8	All glyphs fit into a 8x8 pixel box.
		<char set>	Description
		f	The font includes up to 256 glyphs.
		r	Only glyphs on the range of the ASCII codes 32 to 127 are included in the font.
		u	Only glyphs on the range of the ASCII codes 32 to 95 (uppercase chars) are included in the font.
		n	Only numbers and extra glyphs for writing date and time strings are included in the font.
		...	Other custom character list.*/

		u8g2_SetFont(&u8g2, u8g2_font_crox3c_mf);
		disp_splash();
		text_insertion_point(0 , 0);
	//	u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
		monitor_begin();
}
