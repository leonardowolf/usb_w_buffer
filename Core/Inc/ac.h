/*
 * app.h
 *
 *  Created on: Apr 18, 2022
 *      Author: munin
 */

#ifndef INC_AC_H_
#define INC_AC_H_
#include "stdint.h"
#include "u8g2.h"

uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
U8X8_UNUSED void *arg_ptr);

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
		void *arg_ptr);

void Start_APP(void);

#endif /* INC_AC_H_ */
