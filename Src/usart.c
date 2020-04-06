/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "main.h"
#include "buttons.h"
#include "ext_flash.h"
#include "usbd_cdc_if.h"
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 19200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = UART_DBG_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(UART_DBG_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = UART_DBG_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(UART_DBG_RX_GPIO_Port, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, UART_DBG_TX_Pin|UART_DBG_RX_Pin);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

//#define USB_INFO_OUTPUT

void SerialInfoOutput_PrintWelcomeInfo(void)
{
    sprintf(UART_BufSrtingTransmit, "----------------------------\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "     Audio I/O Selector    \r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "----------------------------\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "(c) M.Tsaryov, 2020.3 \r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "----------------------------\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
}

void SerialInfoOutput_PrintSaveMessage(void)
{
    sprintf(UART_BufSrtingTransmit, "* Settings is saved\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
}

void SerialInfoOutput_PrintLoadMessage(void)
{
    sprintf(UART_BufSrtingTransmit, "* Settings is loaded\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
}

void SerialInfoOutput_PrintLoadedParameters(void)
{
    sprintf(UART_BufSrtingTransmit, "------ Loaded settings -----\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "Active Input:\t\t%X\r\n", FlashPageBuffer[0] + 1);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "Active Output:\t\t%X\r\n", FlashPageBuffer[1] + 1);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "Lock State:\t\t%X\r\n", FlashPageBuffer[2]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "Mute State:\t\t%X\r\n", FlashPageBuffer[3]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "Modules Count:\t\t%X\r\n", FlashPageBuffer[4]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "Inputs Count:\t\t%X\r\n", FlashPageBuffer[5]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "I/O Order:\t\t%X\r\n", FlashPageBuffer[6]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "----------------------------\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
}

void SerialInfoOutput_PrintGoodbyeInfo(void)
{
    sprintf(UART_BufSrtingTransmit, "----------------------------\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "          Power Off         \r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "----------------------------\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
}

void SerialInfoOutput_PrintButtonEvent(void)
{
    if (PowerButton_State == CLOSE)
        sprintf(UART_BufSrtingTransmit, "* Short press Power button\r\n");

    if (InputButton_State == CLOSE)
        sprintf(UART_BufSrtingTransmit, "* Short press Input button\r\n");

    if (OutputButton_State == CLOSE)
        sprintf(UART_BufSrtingTransmit, "* Short press Output button\r\n");

#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
}

void SerialInfoOutput_PrintSavedParameters(uint32_t *pAddr)
{
    ExternalFlash_ReadPage(pAddr);
    sprintf(UART_BufSrtingTransmit, "Saved settings:\r\n");
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "Active Input:\t\t%X\r\n", FlashPageBuffer[0] + 1);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "Active Output:\t\t%X\r\n", FlashPageBuffer[1] + 1);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "Lock State:\t\t%X\r\n", FlashPageBuffer[2]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "Mute State:\t\t%X\r\n", FlashPageBuffer[3]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "Modules Count:\t\t%X\r\n", FlashPageBuffer[4]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "Inputs Count:\t\t%X\r\n", FlashPageBuffer[5]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
    sprintf(UART_BufSrtingTransmit, "I/O Order:\t\t%X\r\n", FlashPageBuffer[6]);
#ifdef USB_INFO_OUTPUT
    CDC_Transmit_FS((uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit));
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
#endif
}

void UARTCommands_Processing(char Command)
{
    if (Command == 'c') // Connect
    {
        sprintf(UART_BufSrtingTransmit, "AIO Hello\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t *)&UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
    }
    else if (Command == 'i') // Change input
    {
        ShortPressInputButton_Handler();
        SaveSettingsToFlash(&SaveStartAddr); // Сохраняем данные
        sprintf(UART_BufSrtingTransmit, "Input %d\r\n", ActiveInput + 1);
        HAL_UART_Transmit(&huart1, (uint8_t *)&UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
    }
    else if(Command == 'o') // Change output
    {
        ShortPressOutputButton_Handler();
        SaveSettingsToFlash(&SaveStartAddr); // Сохраняем данные
        sprintf(UART_BufSrtingTransmit, "Output %d\r\n", ActiveOutput + 1);
        HAL_UART_Transmit(&huart1, (uint8_t *)&UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
    }
    else if(Command == 'l') // Change lock state
    {
        LongPressInputButton_Handler();
        SaveSettingsToFlash(&SaveStartAddr); // Сохраняем данные
        if (Lock_State == 0)
        {
            sprintf(UART_BufSrtingTransmit, "Lock state 0\r\n");
        }
        else
        {
            sprintf(UART_BufSrtingTransmit, "Lock state 1\r\n");
        }
        HAL_UART_Transmit(&huart1, (uint8_t *)&UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
    }
    else if(Command == 'm') // Change mute state
    {
        LongPressOutputButton_Handler();
        SaveSettingsToFlash(&SaveStartAddr); // Сохраняем данные
        if (Mute_State == 0)
        {
            sprintf(UART_BufSrtingTransmit, "Mute state 0\r\n");
        }
        else
        {
            sprintf(UART_BufSrtingTransmit, "Mute state 1\r\n");
        }
        HAL_UART_Transmit(&huart1, (uint8_t *)&UART_BufSrtingTransmit, strlen(UART_BufSrtingTransmit), HAL_MAX_DELAY);
    }
    else if (Command == 'r') // Read settings
    {
        SerialInfoOutput_PrintSavedParameters(&SaveStartAddr);
    }
    else if (Command == 's') // Save settings
    {
        SaveSettingsToFlash(&SaveStartAddr);
    }

    DisplayUpdate_Task = TRUE;
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&UART_BufSrtingReceive, 1);
}


/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
