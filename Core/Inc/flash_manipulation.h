/*
 * flash_manipulation.h
 *
 *  Created on: May 18, 2022
 *      Author: munin
 */

#ifndef INC_FLASH_MANIPULATION_H_
#define INC_FLASH_MANIPULATION_H_
#include <stdint.h>
#include <stdbool.h>

uint32_t Flash_Write_Data (uint32_t StartPageAddress, uint32_t *Data, uint16_t numberofwords);
void Flash_Read_Data (uint32_t StartPageAddress, uint8_t *RxBuf, uint16_t numberofwords);

#endif /* INC_FLASH_MANIPULATION_H_ */
