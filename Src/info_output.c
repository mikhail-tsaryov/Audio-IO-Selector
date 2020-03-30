#include "info_output.h"
#include "usbd_cdc_if.h"

// Буфер строки для подготовки к выводу по UART
char UART_BufSrting[32];

void SerialInfoOutput_PrintWelcomeInfo(void)
{
    uint32_t TempBuf = ExternalFlash_ReadIdentification();
    uint8_t ManufacturerID = TempBuf >> 16;
    uint16_t DeviceID = TempBuf;

    sprintf(UART_BufSrting, "----------------------------\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "     Audio I/O Selector    \r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif 
    sprintf(UART_BufSrting, "----------------------------\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "(c) M.Tsaryov, 2020.3 \r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "Manufacturer ID: 0x%X\r\n", ManufacturerID);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "Device ID: 0x%X\r\n", DeviceID);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "Status Register: 0x%X\r\n", ExternalFlash_ReadStatusRegister());
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "----------------------------\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
}

void SerialInfoOutput_PrintGoodbyeInfo(void)
{
    sprintf(UART_BufSrting, "----------------------------\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "          Power Off         \r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "----------------------------\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
}

void SerialInfoOutput_PrintSavedParameters(uint32_t *pAddr)
{
    ExternalFlash_ReadPage(pAddr);
    sprintf(UART_BufSrting, "------ Saved settings ------\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "Active Input:\t\t%X\r\n", FlashPageBuffer[0] + 1);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "Active Output:\t\t%X\r\n", FlashPageBuffer[1] + 1);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "Lock State:\t\t%X\r\n", FlashPageBuffer[2]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "Mute State:\t\t%X\r\n", FlashPageBuffer[3]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "Modules Count:\t\t%X\r\n", FlashPageBuffer[4]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "Inputs Count:\t\t%X\r\n", FlashPageBuffer[5]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "I/O Order:\t\t%X\r\n", FlashPageBuffer[6]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "----------------------------\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
}

void SerialInfoOutput_PrintLoadedParameters(void)
{
    sprintf(UART_BufSrting, "------ Loaded settings -----\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "Active Input:\t\t%X\r\n", FlashPageBuffer[0] + 1);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "Active Output:\t\t%X\r\n", FlashPageBuffer[1] + 1);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "Lock State:\t\t%X\r\n", FlashPageBuffer[2]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "Mute State:\t\t%X\r\n", FlashPageBuffer[3]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "Modules Count:\t\t%X\r\n", FlashPageBuffer[4]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "Inputs Count:\t\t%X\r\n", FlashPageBuffer[5]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "I/O Order:\t\t%X\r\n", FlashPageBuffer[6]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrting, "----------------------------\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
#endif
}