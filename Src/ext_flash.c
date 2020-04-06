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
}

/**
  * @brief Функция читает Manufacturer’s ID и Device ID
  * @retval Старший байт - Manufacturer’s ID, два младших байта - Device ID
  */
uint32_t ExternalFlash_ReadIdentification(void)
{
    uint8_t SendBuf[4] = {RDPD, 0x00, 0x00, 0x00};
    uint8_t ReadBuf[3]; 

    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, SendBuf, 4, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, ReadBuf, 3, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);

    uint8_t ReadID_Cmd = RDID;

    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &ReadID_Cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, ReadBuf, 3, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);

    return ((ReadBuf[0] << 16) | (ReadBuf[1] << 8) | ReadBuf[2]);
}

/**
  * @brief Функция читает Status Register
  * @retval Содержимое Status Register
  */
uint8_t ExternalFlash_ReadStatusRegister(void)
{
    uint8_t SendBuf = RDSR;
    uint8_t ReadBuf = 0;

    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&SendBuf, 1, HAL_MAX_DELAY);
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

void ExternalFlash_ReadPage(uint32_t *pAddr)
{
    uint8_t SendBuf[4] = {READ, (*pAddr >> 16), (*pAddr >> 8), *pAddr};

    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, SendBuf, 4, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, FlashPageBuffer, 256, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);
}

void ExternalFlash_EraseSector(uint32_t *pAddr)
{
    uint8_t SendBuf[4] = {ERASE_SECTOR, (*pAddr >> 16), (*pAddr >> 8), *pAddr};

    ExternalFlash_WriteEnable();
    
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, SendBuf, 4, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);
    ExternalFlash_WriteDisable();
}

void ExternalFlash_WriteBlock(uint32_t *pAddr)
{
    uint8_t SendBuf[4] = {WRITE, (*pAddr >> 16), (*pAddr >> 8), *pAddr};
    
    HAL_GPIO_WritePin(FLASH_nWP_GPIO_Port, FLASH_nWP_Pin, GPIO_PIN_SET);

    ExternalFlash_EraseSector(pAddr);
    while (ExternalFlash_ReadStatusRegister() & BUSY);
    
    // Записать байт
    ExternalFlash_WriteEnable();
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, SendBuf, 4, HAL_MAX_DELAY);
    HAL_SPI_Transmit(&hspi1, FlashPageBuffer, 256, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI_nCS_FLASH_GPIO_Port, SPI_nCS_FLASH_Pin, GPIO_PIN_SET);
    while (ExternalFlash_ReadStatusRegister() & BUSY);
    //ExternalFlash_WriteDisable();
    HAL_GPIO_WritePin(FLASH_nWP_GPIO_Port, FLASH_nWP_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  Функция сохраняет все настройки во внешнюю память.
  * @retval Нет
  */
void SaveSettingsToFlash(uint32_t *pBaseAddr)
{
    FlashPageBuffer[0] = ActiveInput;
    FlashPageBuffer[1] = ActiveOutput;
    FlashPageBuffer[2] = Lock_State;
    FlashPageBuffer[3] = (Mute_State & AllowSaveMute_Flag);
    FlashPageBuffer[4] = ModulesCount_Par;
    FlashPageBuffer[5] = InputsCount_Par;
    FlashPageBuffer[6] = InOutOrder_Par;

    while (ExternalFlash_ReadStatusRegister() & BUSY);
    ExternalFlash_WriteBlock(pBaseAddr);
}

/**
  * @brief  Функция загружает все настройки из внешней памяти.
  * @retval Нет
  */
void LoadSettingsFromFlash(uint32_t *pBaseAddr)
{
    ExternalFlash_ReadPage(pBaseAddr);

    if ((FlashPageBuffer[4] > 0) & (FlashPageBuffer[4] <= MAX_MODULES))
    {
        ModulesCount_Par = FlashPageBuffer[4];
    }

    if ((FlashPageBuffer[5] > 0) & (FlashPageBuffer[5] <= ModulesCount_Par * RELAY_CNT - 1))
    {
        InputsCount_Par = FlashPageBuffer[5];
        OutputsCount_Par = ModulesCount_Par * RELAY_CNT - InputsCount_Par;
    }

    if ((FlashPageBuffer[0] >= 0) & (FlashPageBuffer[0] < InputsCount_Par))
    {
        ActiveInput = FlashPageBuffer[0];
    }

    if ((FlashPageBuffer[1] >= 0) & (FlashPageBuffer[1] < OutputsCount_Par))
    {
        ActiveOutput = FlashPageBuffer[1];
    }

    if ((FlashPageBuffer[6] == IN_OUT) | (FlashPageBuffer[6] == OUT_IN))
    {
        InOutOrder_Par = FlashPageBuffer[6];
    }

    if ((FlashPageBuffer[2] == OFF) | (FlashPageBuffer[2] == ON))
    {
        Lock_State = FlashPageBuffer[2];
    }

    if ((FlashPageBuffer[3] == OFF) | (FlashPageBuffer[3] == ON))
    {
        Mute_State = FlashPageBuffer[3];
    }
}