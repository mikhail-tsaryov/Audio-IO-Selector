#include "info_output.h"
#include "usbd_cdc_if.h"

// Буфер строки для подготовки к выводу по UART
char UART_BufSrting[32];

void SerialInfoOutput_PrintWelcomeInfo(void)
{
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
    sprintf(UART_BufSrting, "----------------------------\r\n");
    #ifdef USB_INFO_OUTPUT
        CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
    #else
        HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
    #endif
}

void SerialInfoOutput_PrintSaveMessage(void)
{
    sprintf(UART_BufSrting, "* Settings is saved\r\n");
    #ifdef USB_INFO_OUTPUT
        CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
    #else
        HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
    #endif
}

void SerialInfoOutput_PrintLoadMessage(void)
{
    sprintf(UART_BufSrting, "* Settings is loaded\r\n");
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

void SerialInfoOutput_PrintButtonEvent(void)
{
    if (PowerButton_State == CLOSE)
        sprintf(UART_BufSrting, "* Short press Power button\r\n");

    if (InputButton_State == CLOSE)
        sprintf(UART_BufSrting, "* Short press Input button\r\n");

    if (OutputButton_State == CLOSE)
        sprintf(UART_BufSrting, "* Short press Output button\r\n");

    #ifdef USB_INFO_OUTPUT
        CDC_Transmit_FS((uint8_t *)UART_BufSrting, strlen(UART_BufSrting));
    #else
        HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrting, strlen(UART_BufSrting), HAL_MAX_DELAY);
    #endif
}