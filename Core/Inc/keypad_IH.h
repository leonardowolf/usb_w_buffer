/*
 * keypad_IH.h
 *
 *  Created on: Apr 30, 2022
 *      Author: munin
 */

#ifndef INC_KEYPAD_IH_H_
#define INC_KEYPAD_IH_H_

#define DEBOUNCE_TIME 500

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "stm32f1xx_hal.h"


typedef struct{
	uint16_t deb_counter;
	IRQn_Type IRQn_p;
} key_ih;

void Kpd_Irqv_Init(void);
void SystickCallback(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void IRQ_GPIO_dealer(uint16_t GPIO_Pin);

#endif /* INC_KEYPAD_IH_H_ */
