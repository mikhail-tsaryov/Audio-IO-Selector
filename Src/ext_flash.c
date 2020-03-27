#include "ext_flash.h"
#include "spi.h"
#include "usart.h"


void ExternalFlash_Init(void)
{
    HAL_GPIO_WritePin(FLASH_nWP_GPIO_Port, FLASH_nWP_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
}

uint32_t ExternalFlash_ReadID(void)
{
    uint8_t WriteEn = 0x06;
    uint8_t WriteBuf = 0x9F;
    uint8_t Dummy = 0xA5;
    uint32_t ReadBuf = 0; 
    uint8_t Temp [3];
    
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&WriteBuf, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, Temp, 3, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);

    ReadBuf = (Temp[0] << 16) | (Temp[1] << 8) | Temp[2];
    HAL_UART_Transmit(&huart1, (uint8_t *)&ReadBuf, 3, HAL_MAX_DELAY);
    return ReadBuf;
}