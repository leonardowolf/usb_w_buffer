/*
 * cwlinux_lib.c
 *
 *  Created on: Jan 17, 2022
 *      Author: hugin
 *      o intuito desta biblioteca é integrar os comandos reconhecidos pelo cwluinux com os definidos na biblioteca de manipulação do display
 */
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "u8g2.h"
#include "ac.h"
#include "logo_defs.h"
#include "cwlibx.h"

extern u8g2_t u8g2;
//extern uint8_t u8g2_cursor[2];

#define delay_betwen_cmds 50 //tempo de descanso entre comandos

// 				Command Summary
//	->TEXT Commands

/**@brief	auto line wrap												(Default: OFF)
 *	habilita a quebra de linha
 *	-on
 *	quebra de linha ativada determina que ao chegar ao fim do display o texto
 *	se quebra para a primeira posição da proxima linha
 *		FE 43 FD
 *		254 67 253
 *		254 `C` 253
 *	-off
 *	quebra de linha desativada determina que ao chegar ao fim do display o texto
 *	se quebra para a primeira posição da propria linha em que ele estava sendo escrito
 *		FE 44 FD
 *		254 68 253
 *		254 `D` 253
 */
void auto_line_wrap(bool enable) {
	//u8g2_SendF(&u8g2, "c", (enable ? 0x0a7 : 0x0a6));
	text_wrap = enable;
}

/**@brief	auto scroll 												(Default: OFF)
 *	desloca todo o display em uma linha para abrir espaço para a ultima linha
 *	-on
 *	habilita
 *		FE 51 FD
 *		254 81 253
 *		254 `Q` 253
 *	-off
 *	desabilita
 *		FE 52 FD
 *		254 82 253
 *		254 `R` 253
 */
void auto_scroll(uint8_t enable) {
	u8g2_SendF(&u8g2, "c", 0x040);
}

/**@brief Set text insertion point									(Default: N/A)
 *	determina o ponto de inserção de texto como sendo o passado pelo usuario
 *		FE 47 [col] [row] FD
 *		254 71 [col] [row] 253
 *		254 `G` [col] [row] 253
 *	-HOME
 *	determina o ponto como sendo o inicio (0,0) do display
 *		FE 48 FD
 *		254 72 253
 *		254 `H` 253
 */
void text_insertion_point(uint8_t col, uint8_t row) {
	//convertendo a entrada para posição em termos de caractere
	cursor[0] = col * u8g2_GetMaxCharWidth(&u8g2) /*- '0'*/;
	cursor[1] = row * (u8g2_GetMaxCharHeight(&u8g2) - ESP_ENTRE_LINHAS)/*- '0'*/;
}

/**@brief Underline cursor 											(Default: OFF)
 *	cursor de indicação de entrada de texto
 *	-on
 *	habilita o cursor e o posiciona na coordenada determinada pelo usuario
 *		FE 4A [col] [row] FD
 *		254 74 [col] [row] 253
 *		254 `J` [col] [row] 253
 *	-off
 *	desabilita o cursor
 *		FE 4B FD
 *		254 75 253
 *		254 `K` 253
 */
void underline_cursor(uint8_t col, uint8_t row, uint8_t state);

/**@brief Cursor														(Default: N/A)
 *	função de deslocamento posicional do cursor underline
 *	-left
 *	desloca o cursor underline para a esquerda, se o cursor encontra o começo da linha
 *	ele se deslocará para o fim da mesma
 *		FE 4C FD
 *		254 76 253
 *		254 `L` 253
 *	-right
 *	desloca o cursor underline para a direita, se o cursor encontra o fim da linha ele
 * 	se deslocará para o início da mesma
 *		FE 4D FD
 *		254 77 253
 *		254 `M` 253
 */
void cursor_position(uint8_t side);

/**@brief Inverse Text												(Default: OFF)
 *	comando de inversão de texto, o fundo do texto passa a ser de pixels ativos e o texto
 *	passa a ser pixels inativos
 *	-on
 *	ativa a inversão de texto
 *		FE 66 FD
 *		254 102 253
 *		254 `f` 253
 *	-off
 *	desativa a inversão de texto
 *		FE 67 FD
 *		254 103 253
 *		254 `g` 253
 */
void inverse_text(bool state) {
	text_invertion = state;
}

//	->Bar Charts and Graphic Commands

/**@brief Initialize thick vertical bar graph							(Default: ON)
 *	Define a barra vertical como com 5px de largura
 *	FE 76 FD
 *	254 118 253
 *	254 `v` 253
 */
void def_thick_v_bar(void) {
	vertical_bar_width = 5;
}

/**@brief Inithialize thin vertical bar graph							(Default: OFF)
 * 	Define a barra vertical como com 2px de largura
 *	FE 73 FD
 *	254 115 253
 *	254 `s` 253
 */
void def_thin_v_bar(void) {
	vertical_bar_width = 2;
}

/**@brief Define a custom caracter									(Default: N/A)
 *	Define um caracter customizado, [cc] varia de [[0x01] a [0x10]].
 *	Os 6 bytes variam da seguinte forma:
 *	FE 4E [cc] [6bytes] FD
 *	254 104 [cc] [6bytes] 253
 *	254 `N` [cc] [6bytes] 253
 */
void custom_character(uint8_t index, uint8_t *bit_array);

/**@brief Draw a vertical bar graph									(Default: N/A)
 *	Desenha uma coluna vertical na ultima linha da coluna [col] de altura [height], com [height]
 *	variando de [[0x00] a [0x7A]] (0 a 122)
 *	FE 3D [col] [height] FD
 *	254 61 [col] [height] 253
 *	254 `=` [col] [height] 253
 */
void draw_v_bar_graph(uint8_t col, uint8_t height) {
	cursor[0] = col * u8g2_GetMaxCharWidth(&u8g2);

	u8g2_DrawBox(&u8g2, cursor[0], cursor[1], vertical_bar_width, height);
	u8g2_SendBuffer(&u8g2);
}

/**@brief Erase a vertical bar graph									(Default: N/A)
 *	Apaga a barra vertical na ultima linha da coluna [col]  com altura [height],
 *	com height	variando [[0x00] a [0x20]] (0 a 32)
 *	FE 2D [col] [height] FD
 * 	254 45 [col] [height] 253
 *	254 `-` [col] [height] 253
 */
void erase_v_bar_graph(uint8_t col, uint8_t height) {
	cursor[0] = col * u8g2_GetMaxCharWidth(&u8g2);
	u8g2_DrawBox(&u8g2, cursor[0], cursor[1], vertical_bar_width, height);
	u8g2_SendBuffer(&u8g2);
}

/**@brief Draw a horizontal bar graph									(Default: N/A)
 *	Desenha uma coluna vertical na ultima linha da coluna [col] de altura [height], com [height]
 *	variando de [[0x00] a [0x7A]] (0 a 122)
 *	FE 7C [cc] [height] FD
 *	254 124 [cc] [height] 253
 *	254 `|` [cc] [height] 253
 */
void draw_h_bar_graph(uint8_t col, uint8_t row, uint8_t lenght) {
	cursor[0] = (col  ) * u8g2_GetMaxCharHeight(&u8g2);
	cursor[1] = (row  ) * u8g2_GetMaxCharWidth(&u8g2);
	u8g2_DrawBox(&u8g2, cursor[0], cursor[1], lenght  ,
			u8g2_GetMaxCharHeight(&u8g2));
	u8g2_SendBuffer(&u8g2);
	//u8g2_DrawVLine(&u8g2, col, u8g2_uint_t y, u8g2_uint_t h);
}

/**@brief Erase a horizontal bar graph									(Default: N/A)
 *	Apaga a barra horizontal na coluna [col] da linha [row] com largura [length]
 *	onde length varia [[0x00] to [0x7A]] (0 a 122)
 *	FE 2D [col] [row] FD
 *	254 45 [col] [row] 253
 *	254 `-` [col] [row] 253
 */
void erase_h_bar(uint8_t col, uint8_t height);

/**@brief Put pixel													(Default: N/A)
 *	Desenha um pixel na posição (x,y). X varia de 0-121 e y varia 0-31
 * 	FE 70 [x] [y] FD
 *	254 112 [x] [y] 253
 *	254 `p` [x] [y] 253
 */
void put_pixel(uint8_t x, uint8_t y) {
	u8g2_SetDrawColor(&u8g2, 1);
	u8g2_DrawPixel(&u8g2, x  , y  );
	u8g2_SendBuffer(&u8g2);
}

/**@brief Clear Pixel													(Default: N/A)
 *	Apaga um pixel na posição (x,y). X varia de 0-121 e y varia 0-31
 *	FE 71 [x] [y] FD
 *	254 113 [x] [y] 253
 *	254 `q` [x] [y] 253
 */
void erase_pixel(uint8_t x, uint8_t y) {
	u8g2_SetDrawColor(&u8g2, 0);
	u8g2_DrawPixel(&u8g2, x  , y  );
	u8g2_SendBuffer(&u8g2);
}

/** @brief Draw a byte													(Default: N/A)
 *	Desenha um byte na posição [x,row] onde x varia de 0-121 e y varia de 0-3
 *	FE 3E [x] [row] [byte] [4 dummy bytes] FD
 *	254 62 [x] [row] [byte] [4 dummy bytes]  253
 *	254 `>` [x] [row] [byte] [4 dummy bytes]  253
 */
void put_byte(uint8_t x, uint8_t row, uint8_t *byte);

// Miscellaneous command summary

/** @brief reseta por software o display
 *   	FE 56 FD
 *		254 86 253
 *		254 'V' 253
 *	@retval
 */

void lcd_soft_reset(void) {
	HAL_NVIC_SystemReset();
}

/** @brief Clear Display																(Default: N/A)
 * Limpa a tela e posiciona o cursor no canto superior esquerdo
 *   	FE 58 FD
 *		254 88 253
 *		254 'X' 253
 *	@retval  flag confirmando que o comando foi executado
 */
void clear_display(void) {
	text_insertion_point(0 , 0);
	u8g2_ClearDisplay(&u8g2);
}

/** @brief Backlight state																(Default: N/A)
 * Define o status da Backlight/ Luz de fundo do display
 *  -on
 *  Habilita o Backlight
 *   	FE 42 FD
 *		254 66 253
 *		254 'B' 253
 *  -off
 *  Desabilita o Backlight
 *   	FE 46 FD
 *		254 70 253
 *		254 'F' 253
 */
void enable_backlight(bool enable) {
	//liga a backlight
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,
			enable ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/** @brief Backlight brightness															(Default: N/A)
 * Define o brlho do backlight em um dos niveis pre-configurados, [bright] varia de 1-7
 *   	FE 41 [bright] FD
 *		254 64 [bright] 253
 *		254 'A' [bright] 253
 */
void set_backlight_brightness(uint8_t bright);

/** @brief Auto Key Hold state															(Default: N/A)
 * -on
 * Habilita a função key hold
 *   	FE 32 FD
 *		254 50 253
 *		254 '2' 253
 * -off
 * desabilita a função key hold
 *   	FE 33 FD
 *		254 51 253
 *		254 '3' 253
 */
void key_hold_state(uint8_t state);

/** @brief Set RS232 port speed															(Default: 19200)
 * Define a velocidade da comunicação RS232-ttl. As velocidades são tabeladas. A saber:
 * [speed] esta contido no intervalo {1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200, 230400}
 *   	FE 39 [speed] FD
 *		254 57 [speed] 253
 *		254 '9' [speed] 253
 */
void set_rs232_speed(uint8_t speed);

/** @brief Save user defined characters													(Default: N/A)
 * Salva um caractere definido pelo usuario. [cc] varia de 1-16
 *   	FE 4F [cc] FD
 *		254 79 [cc] 253
 *		254 'O' [cc] 253
 */
void save_user_character(uint8_t cc);

/** @brief Load user defined characters													(Default: N/A)
 * Carrega um caractere definido pelo usuario. [cc] varia de 1-16
 *   	FE 50 [cc] FD
 *		254 90 [cc] 253
 *		254 'P' [cc] 253
 */
void load_user_characters(uint8_t cc);

/** @brief Save user settings															(Default: N/A)
 * Salva os [4bytes] de configurações definidas pelo usuario [ud], onde [ud] varia de 1-8
 *   	FE 53 [ud] [4bytes] [2dummy_bytes] FD
 *		254 83 [ud] [4bytes] [2dummy_bytes] 253
 *		254 'S' [ud] [4bytes] [2dummy_bytes] 253
 */
uint8_t save_user_settings(uint8_t ud, uint8_t *settings);

/** @brief Load user settings															(Default: N/A)
 * Carrega da memoria as configurações definidas pelo usuario indicado por [ud]
 *   	FE 54 [ud] FD
 *		254 84 [ud] 253
 *		254 'T' [ud] 253
 */
uint8_t load_user_settings(uint8_t ud);

/** @brief Relay state																	(Default: N/A)
 * Ativa o relay
 * -on
 * Ele será sempre ativo se o [timeout] = 0, entretanto ele será ativo por [timeout] segundos
 * sempre que [timeout] > 0. [timeout] é um numero inteiro contido em {0,...,10}
 *   	FE 61 [timeout] FD
 *		254 97 [timeout] 253
 *		254 'a' [timeout] 253
 * -off
 * Desativa o Relay
 *   	FE 62 FD
 *		254 98 253
 *		254 'b' 253
 */
uint8_t relay_set_state(uint8_t timeout);

/** @brief GPO state																	(Default: N/A)
 * define o status da porta gpio [gpo#]
 * -on
 * define a porta como High
 *   	FE 63 [gpi#] FD
 *		254 99 [gpi#] 253
 *		254 'c' [gpi#] 253
 * -off
 * define a porta como Low
 *   	FE 64 [gpi#] FD
 *		254 100 [gpi#] 253
 *		254 'd' [gpi#] 253
 */
uint8_t gpo_set_state(uint8_t gpo, uint8_t status);

/** @brief GPI Status																	(Default: N/A)
 * Retorna o valor de status da porta gpio [gpi#] definida como entrada na forma de bit
 *   	FE 65 [gpi#] FD
 *		254 101 [gpi#] 253
 *		254 'e' [gpi#] 253
 *	@retval flag indicando que deu certo [bool]
 */
uint8_t gpi_get_state(uint8_t gpi);

/** @brief Set contrast																	(Default: N/A)
 * define o contraste da tela, range [[0x00]-[0x1C]] - 0 a 25
 *    	FE 68 FD
 *		254 104 253
 *		254 'h' 253
 */
void set_contrast(uint8_t contrast) {
	u8g2_SetContrast(&u8g2, (contrast  ) * 9);
}

/** @brief Save Boot-up Logo															(Default: N/A)
 * Salva a tela atual como splash/boot-up screen
 *  	FE 69 FD
 *		254 106 253
 *		254 'i' 253
 */
void save_screen_to_splash(void);

/** @brief Display Boot-up Logo															(Default: N/A)
 * Desenha a splash screen/boot-up na tela imediatamente
 *  	FE 69 FD
 *		254 106 253
 *		254 'j' 253
 */
void disp_splash(void) {

	u8g2_DrawXBM(&u8g2, ((display_w / 2) - (vc_width / 2)),
			((display_h / 2) - (vc_height / 2)), vc_width, vc_height, &vc_bits);
	u8g2_SendBuffer(&u8g2);

}

/** @brief Restore default factory logo													(Default: N/A)
 * Restaura a splash screen/boot-up definida pela fabrica
 * 		FE 6B FD
 * 		254 107 253
 * 		254 'k' 253
 */
void restore_def_logo(void);

/** @brief write a string on screen														(Default: N/A)
 * escreve um array na tela.
 *
 */
void str_warper(txt_wrap *wrap, uint8_t *txt) {
	uint8_t index = 0, aux = 1;

	for (index = 0; txt[index] != '\0'; index++)
		;
	index++;
	wrap->wrap_times = index
			/ (u8g2_GetDisplayWidth(&u8g2) / u8g2_GetMaxCharWidth(&u8g2));
	do {
		wrap->wrap_str[aux] = &txt[aux
				* (u8g2_GetDisplayWidth(&u8g2) / u8g2_GetMaxCharWidth(&u8g2))];
		aux++;
	} while (aux != wrap->wrap_times);
}
void put_cursor(void){
	lcd_print("_");
	u8g2_SendBuffer(&u8g2);
}
void test_font(void){
	lcd_print("    XCoder v3.0");
	u8g2_SendBuffer(&u8g2);
}

void lcd_print(uint8_t *txt) {
	txt_wrap wrap;
	wrap.wrap_str[0] = txt;
	wrap.wrap_times = 0;
	uint8_t aux = 0;

	if (text_invertion) {
		if (text_wrap) {
			str_warper(&wrap, txt);
			for (aux = 0; aux + 1 <= wrap.wrap_times; aux++) {
				cursor[1] = (aux * u8g2_GetMaxCharHeight(&u8g2));
				u8g2_DrawButtonUTF8(&u8g2, cursor[0], cursor[1], U8G2_BTN_INV,
						0, 0, 0, wrap.wrap_str[aux]);
				u8g2_SendBuffer(&u8g2);

				if (u8g2_GetStrWidth(&u8g2,
						wrap.wrap_str[aux - 1]) > u8g2_GetDisplayWidth(&u8g2)) {
					cursor[1] = (aux * u8g2_GetMaxCharHeight(&u8g2));
					cursor[0] = 0;
				} else {
					cursor[0] += u8g2_GetStrWidth(&u8g2,
							wrap.wrap_str[aux - 1]);
				}
			}
		} else {
			u8g2_DrawButtonUTF8(&u8g2, cursor[0], cursor[1], U8G2_BTN_INV, 0, 0,
					0, txt);
			u8g2_SendBuffer(&u8g2);

			cursor[0] += u8g2_GetStrWidth(&u8g2, txt);
		}

	} else {
		if (text_wrap) {
			str_warper(&wrap, txt);
			for (aux = 0; aux + 1 <= wrap.wrap_times; aux++) {
				cursor[1] = (aux * u8g2_GetMaxCharHeight(&u8g2));
				u8g2_DrawUTF8(&u8g2, cursor[0], cursor[1], wrap.wrap_str[aux]);
				u8g2_SendBuffer(&u8g2);
			}
			if (u8g2_GetStrWidth(&u8g2,
					wrap.wrap_str[aux - 1]) > u8g2_GetDisplayWidth(&u8g2)) {
				cursor[1] = (aux * u8g2_GetMaxCharHeight(&u8g2));
				cursor[0] = 0;
			} else {
				cursor[0] += u8g2_GetStrWidth(&u8g2, wrap.wrap_str[aux - 1]);
			}
		} else {
			u8g2_DrawUTF8(&u8g2, cursor[0], cursor[1], txt);
			u8g2_SendBuffer(&u8g2);

			cursor[0] += u8g2_GetStrWidth(&u8g2, txt);
		}
	}

}

