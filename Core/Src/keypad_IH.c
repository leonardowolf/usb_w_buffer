/*
 * keypad_IH.c
 *
 *  Created on: Apr 30, 2022
 *      Author: munin
 */

#include <com_monitor.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "keypad_IH.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cwlibx.h"
#include "usbd_cdc_if.h"

//{set,reset,up,down,left,right}
key_ih IRQ_VECTOR[6];

void Kpd_Irqv_Init(void) {
	//
	IRQ_VECTOR[0].IRQn_p = SET_P_EXTI_IRQn;
	IRQ_VECTOR[0].deb_counter = 0;
	//
	IRQ_VECTOR[1].IRQn_p = RESET_P_EXTI_IRQn;
	IRQ_VECTOR[1].deb_counter = 0;
	//
	IRQ_VECTOR[2].IRQn_p = UP_P_EXTI_IRQn;
	IRQ_VECTOR[2].deb_counter = 0;
	//
	IRQ_VECTOR[3].IRQn_p = DOWN_P_EXTI_IRQn;
	IRQ_VECTOR[3].deb_counter = 0;
	//
	IRQ_VECTOR[4].IRQn_p = LEFT_P_EXTI_IRQn;
	IRQ_VECTOR[4].deb_counter = 0;
	//
	IRQ_VECTOR[5].IRQn_p = RIGHT_P_EXTI_IRQn;
	IRQ_VECTOR[5].deb_counter = 0;
}

void SystickCallback(void) {
	uint8_t i;
	for (i = 0; i < 6; i++) {
		if (IRQ_VECTOR[i].deb_counter == 0) {
			HAL_NVIC_EnableIRQ(IRQ_VECTOR[i].IRQn_p);
		} else {
			IRQ_VECTOR[i].deb_counter--;
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == SET_P_Pin) {
		HAL_NVIC_DisableIRQ(IRQ_VECTOR[0].IRQn_p);
		IRQ_VECTOR[0].deb_counter = DEBOUNCE_TIME;
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	}
}
