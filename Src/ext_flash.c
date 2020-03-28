#include <string.h>
#include "ext_flash.h"
#include "spi.h"
#include "usart.h"

/**
  * @brief Функция инициализации
  * @retval 
  */
void ExternalFlash_Init(void)
{
    HAL_GPIO_WritePin(FLASH_nWP_GPIO_Port, FLASH_nWP_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);
    ExternalFlash_WriteStatusRegister(0);
}

/**
  * @brief Функция читает Manufacturer’s ID и Device ID
  * @retval Старший байт - Manufacturer’s ID, младший байт - Device ID
  */
uint16_t ExternalFlash_ReadID(void)
{
    uint8_t SendBuf[4] = {RDID, 0x00, 0x00, 0x00};
    uint16_t ReadBuf = 0; 

    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, SendBuf, 4, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, (uint8_t *)&ReadBuf, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);

    return ReadBuf;
}

/**
  * @brief Функция читает Status Register
  * @retval Содержимое Status Register
  */
uint8_t ExternalFlash_ReadStatusRegister(void)
{
    uint8_t ReadSR_Cmd = RDSR;
    uint8_t ReadBuf = 0;

    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &ReadSR_Cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, (uint8_t *)&ReadBuf, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);

    return ReadBuf;
}

/**
  * @brief Функция разрешает запись (Write-Enable)
  * @retval 
  */
void ExternalFlash_WriteEnable(void)
{
    uint8_t WriteEnable_Cmd = WREN;
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &WriteEnable_Cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);
}

/**
  * @brief Функция запрещает запись бит (Write-Disable)
  * @retval 
  */
void ExternalFlash_WriteDisable(void)
{
    uint8_t WriteDisable_Cmd = WRDI;
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &WriteDisable_Cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);
}

uint8_t ExternalFlash_Read(uint32_t *pAddr)
{
    uint8_t SendBuf[4] = {READ, (*pAddr >> 16), (*pAddr >> 8), *pAddr};
    uint8_t ReadBuf = 0;

    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, SendBuf, 4, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, (uint8_t *)&ReadBuf, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);

    return ReadBuf;
}

void ExternalFlash_EraseSector(uint32_t *pAddr)
{
    uint8_t SendBuf[4] = {ERASE_SECTOR, (*pAddr >> 16), (*pAddr >> 8), *pAddr};

    ExternalFlash_WriteEnable();
    
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, SendBuf, 4, HAL_MAX_DELAY);
    while (ExternalFlash_ReadStatusRegister() & BUSY);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);
}

void ExternalFlash_WriteByte(uint8_t *pData, uint32_t *pAddr)
{
    uint8_t SendBuf[4] = {WRITE, (*pAddr >> 16), (*pAddr >> 8), *pAddr};
    
    HAL_GPIO_WritePin(FLASH_nWP_GPIO_Port, FLASH_nWP_Pin, GPIO_PIN_SET);

    ExternalFlash_EraseSector(pAddr);
    // Записать байт
    ExternalFlash_WriteEnable();
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, SendBuf, 4, HAL_MAX_DELAY);
    HAL_SPI_Transmit(&hspi1, pData, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);
    
    ExternalFlash_WriteDisable();
    HAL_GPIO_WritePin(FLASH_nWP_GPIO_Port, FLASH_nWP_Pin, GPIO_PIN_RESET);
}

void ExternalFlash_WriteStatusRegister(uint8_t Value)
{
    uint8_t EnableWriteSR_Cmd = EWSR;
    uint8_t WriteBuf[2] = {WRSR, Value};

    ExternalFlash_WriteEnable();
    // Разрешить изменение Status Register
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &EnableWriteSR_Cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);
    // Перезаписать Status Register
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, WriteBuf, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);
}