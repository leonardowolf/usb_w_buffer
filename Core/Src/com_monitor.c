/*
 * s_monitor.c
 *
 *  Created on: Jan 24, 2022
 *      Author: hugin
 */

#include <com_monitor.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cwlibx.h"
#include "usbd_cdc_if.h"

uint8_t cmd[CMD_BUF_SIZE];
uint8_t ans[ANS_BUF_SIZE];

uint32_t size;
bool new_cmd;

void monitor_send_string(uint8_t *buf) {
	CDC_Transmit_FS(buf, strlen(buf));
}

void monitor_check_cmd(char *cmd, uint32_t size) {
	if (strncmp(cmd, "help", 4) == 0) {

		snprintf((char*) ans, CMD_BUF_SIZE,
				"aqui eu deveria imprimir toda a tabela de comandos\r\n");
		monitor_send_string(ans);
	}
	//é um comando?
	else if (cmd[0] == LCD_CMD) {

		if (cmd[1] == LCD_FIRMWARE_NUMBER) {
			snprintf((char*) ans, CMD_BUF_SIZE, "Firmware Version: %s\r\n",
			model_version);
			monitor_send_string(ans);
		} else if (cmd[1] == LCD_MODEL_NUMBER) {
			snprintf((char*) ans, CMD_BUF_SIZE, "Model Version: %s\r\n",
			fw_version);
			monitor_send_string(ans);
		}

		else if (cmd[1] == LCD_SOFT_RESET) {
			lcd_soft_reset();
		}

		else if (cmd[1] == LCD_BACKLIGHT_ON) {
			enable_backlight(true);
		}

		else if (cmd[1] == LCD_BACKLIGHT_OFF) {
			enable_backlight(false);
		}

		else if (cmd[1] == LCD_INVERSE_TEXT_ON) {
			inverse_text(true);
		}

		else if (cmd[1] == LCD_INVERSE_TEXT_OFF) {
			inverse_text(false);
		}

		else if (cmd[1] == LCD_CLEAR) {
			clear_display();
		}
//		para os 2 comandos a seguir é necessário inverter as coordenadas pois o display tem o eixo de coordenadas com o hardware em posição retrato
		else if (cmd[1] == LCD_PUT_PIXEL) {
			put_pixel(cmd[3], cmd[2]);
		}

		else if (cmd[1] == LCD_CLEAR_PIXEL) {
			erase_pixel(cmd[3], cmd[2]);
		}

		else if (cmd[1] == LCD_WRAP_ENABLE) {
			//não achei comando definido talvez eu tenha que contar char na string e mudar o cursor eu mesmo...getStrWidth pode ajudar
//			snprintf((char*) ans, CMD_BUF_SIZE, "n sei ainda.");
//			monitor_send_string(ans);
			auto_line_wrap(true);

		}

		else if (cmd[1] == LCD_WRAP_DISABLE) {
			//não achei comando definido talvez eu tenha que contar char na string e mudar o cursor eu mesmo...getStrWidth pode ajudar
//			snprintf((char*) ans, CMD_BUF_SIZE, "n sei ainda.");
//			monitor_send_string(ans);

			auto_line_wrap(false);

		}

		else if (cmd[1] == LCD_SET_INSERT) {
			text_insertion_point(cmd[2], cmd[3]);
		}

		else if (cmd[1] == LCD_HOME_INSERT) {
			text_insertion_point(0, 0);
		}

		else if (cmd[1] == LCD_READ_GPI) {
			if (cmd[2] == '0') {
				snprintf((char*) ans, CMD_BUF_SIZE, "%d.%d V\r\n", ddp[0],
						ddp[1]);
				monitor_send_string(ans);
			}
		}

		else if (cmd[1] == LCD_DISPLAY_SPLASH) {
			disp_splash();
		}

		else if (cmd[1] == LCD_DEF_THICK_VERTICAL_BAR) {
			def_thick_v_bar();
		}

		else if (cmd[1] == LCD_DEF_THIN_VERTICAL_BAR) {
			def_thin_v_bar();

		}

		else if (cmd[1] == LCD_DRAW_VERTICAL_BAR_GRAPH) {
			draw_v_bar_graph(cmd[2], cmd[3]);
		}

		else if (cmd[1] == LCD_DRAW_VERTICAL_BAR_GRAPH) {

			erase_v_bar_graph(cmd[2], cmd[3]);
		}

		else if (cmd[1] == LCD_DRAW_HORIZONTAL_BAR_GRAPH) {
			draw_h_bar_graph(cmd[2], cmd[3], cmd[4]);
		}

	}
	//se não era comando, tenta escrever na tela
	/*	else {
	 lcd_print(cmd);
	 }*/
}

void app_usb_cbk(uint8_t *buf, uint32_t len) {
	uint8_t i;
	CDC_Transmit_FS(buf, len);

	for (i = 0; i < len; i++) {
		cmd[i] = *(buf + i);
	}

	size = len;
	new_cmd = true;
}

void monitor_interrupt(void) {
	if (new_cmd) {
		uint8_t offset, bgn_print, end_print, i;
		bgn_print = 0;
		end_print = size;
		bool is_cmd = false;
		bool printable = false;
		/*o buffer tem a entrada completa, agora só precisa percorrer char a char, e caso encontre
		 um inicializador de comando, chamar a função de interpretação com o vetor e o offset de onde
		 começa o inicializador*/

		for (offset = 0; offset <= size; offset++) {
			if (*(cmd + offset) == LCD_CMD_END && *(cmd + offset + 1) != LCD_CMD
					&& *(cmd + offset + 1) != 20) {
				//se o caractere atual é o de finalização de comando e o proximo não é o começo de outro e nem um espaço em branco então é o começo de um trecho imprimivel
				bgn_print = offset + 1;
			}
			if (offset > 0 && *(cmd + offset) == LCD_CMD
					&& *(cmd + offset - 1) != LCD_CMD_END || offset == size) {
				//se o caractere atual é o de inicialização de comando e o anterior não é o começo de outro e
				printable = true;
				end_print = offset;
			}
			if (printable) {
				strncpy(ans, cmd + bgn_print, end_print - bgn_print);
				CDC_Transmit_FS(ans, end_print - bgn_print);
				lcd_print(ans);
				for (i = 0; i < end_print - bgn_print; i++) {
					ans[i] = '\0';
				}
				printable = !printable;
			}

			if (*(cmd + offset) == LCD_CMD) {
				monitor_check_cmd((char*) cmd + offset, size);
				is_cmd = !is_cmd;
			} else if (*(cmd + offset) == LCD_CMD_END) {
				is_cmd = !is_cmd;

			}

		}
		new_cmd = !new_cmd;
	}
}

void monitor_begin(void) {
	uint8_t i;
	size = 0;
	new_cmd = false;

	/*	clear_display();
	 test_font();
	 clear_display();
	 put_cursor();*/

	//mensagem de inicialização
	snprintf((char*) ans, CMD_BUF_SIZE, "hora do show!\r\n");
	monitor_send_string(ans);
	for (i = 0; i < strlen("hora do show!\r\n"); i++) {
		ans[i] = '\0';
	}
}
