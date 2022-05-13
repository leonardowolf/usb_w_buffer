/*
 * s_monitor.h
 *
 *  Created on: Jan 24, 2022
 *      Author: hugin
 */

#ifndef INC_COM_MONITOR_H_
#define INC_COM_MONITOR_H_
#include "stdint.h"

void monitor_send_string(uint8_t *data);
void monitor_check_cmd(char *cmd, uint32_t size);
void monitor_irq_handler(void);
void monitor_interrupt();

void app_usb_cbk(uint8_t* buf, uint32_t len);
void monitor_begin(void);

#define CMD_BUF_SIZE 200
#define ANS_BUF_SIZE 90

#endif /* INC_COM_MONITOR_H_ */
