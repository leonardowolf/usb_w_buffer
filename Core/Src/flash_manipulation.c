/*
 * flash_manipulation.c
 *
 *  Created on: May 18, 2022
 *      Author: munin
 */
#include "main.h"
#include "stm32f1xx_hal.h"
#include "flash_manipulation.h"
#include <stdint.h>
#include <stdbool.h>

uint32_t Flash_Write_Data (uint32_t StartPageAddress, uint32_t *buf, uint16_t numberofwords)
{
	uint8_t i;
	//unlock
		HAL_FLASH_Unlock();
		//erase method
		  FLASH_EraseInitTypeDef FLASH_EraseInitStruct = {0};

		  FLASH_EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;    //Erase type set to erase pages( Available other type is mass erase)
		  FLASH_EraseInitStruct.PageAddress = StartPageAddress;            //Starting address of flash page (0x0800 0000 - 0x0801 FC00)
		  FLASH_EraseInitStruct.NbPages = 1;                    //The number of pages to be erased


		  uint32_t  errorStatus = 0;

		  HAL_FLASHEx_Erase(&FLASH_EraseInitStruct,&errorStatus);
		  //write data
		  for(i=0;*(buf+i) != '\0';i++){
			  HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,StartPageAddress+(i*2), *(buf+i));
		  }
	//lock back
		   HAL_FLASH_Lock();
}

void Flash_Read_Data (uint32_t StartPageAddress, uint8_t *RxBuf, uint16_t numberofwords)
{uint8_t i;
//posso ler facilmente olhando a memoria na região que fori delimitada. Gravar é que é complicado

	for(i = 0 ; *(RxBuf+i) != 255 ; i++ ){
		*(RxBuf+i) = *((uint8_t *)  StartPageAddress+(i));
	}
}
