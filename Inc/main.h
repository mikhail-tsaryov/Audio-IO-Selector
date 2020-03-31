/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define INFO_OUTPUT
#define RUS_LANG
//#define USB_INFO_OUTPUT

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
// Параметры
extern uint8_t ModulesCount_Par;        // Количество релейных модулей
extern uint8_t InputsCount_Par;         // Количество входов в модуле
extern uint8_t OutputsCount_Par;        // Количество выходов в модуле
extern uint8_t InOutOrder_Par;     // Параметр, отвечающий за порядок расположения на разъемах - входы/выходы или выходы/входы
extern uint8_t IODisplayMode_Par; // Параметр отображения входов/выходов (цифрами или именами)
// Флаги задач
extern volatile uint8_t CountdownLongPress_Task;
extern volatile uint8_t DeBouncer_Task;
extern volatile uint8_t ScanButtonsShort_Task;
extern volatile uint8_t ScanButtonsLong_Task;
extern volatile uint8_t RelaysUpdate_Task;
extern volatile uint8_t RelaysModuleUpdate_Task;
extern volatile uint8_t DisplayUpdate_Task;
// Дополнительные флаги
extern volatile uint8_t AllowSaveMute_Flag; // Флаг разрешения сохранения состояния MUTE
// Таймеры
extern volatile uint16_t DeBouncer_Timer;
extern volatile uint16_t LongPress_Timer;
extern volatile uint16_t DisplayUpdate_Timer;
// Переменные состояния
extern uint8_t System_State; // Текущий режим работы устройства
extern uint8_t SetupStage_State;   // Текущий этап режима настройки
extern uint8_t Mute_State;     // Переменная состояния режима MUTE
extern uint8_t Lock_State;     // переменная состояния блокировки изменения входов и выходов
extern uint8_t ActiveInput;        // Текущий вход
extern uint8_t ActiveOutput;       // Текущий выход
// Состояния кнопок
extern uint8_t PowerButton_State; // Переменная состояния кнопки POWER
extern uint8_t InputButton_State; // Переменная состояния кнопки INPUT
extern uint8_t OutputButton_State; // Переменная состояния кнопки OUTPUT
extern uint8_t LockButton_State;   // Переменная состояния кнопки LOCK (долгое нажатие INPUT)
extern uint8_t MuteButton_State;   // Переменная состояния кнопки MUTE (долгое нажатие OUTPUT)

extern uint32_t SaveStartAddr; // Начальный адрес сохранения настроек
extern uint8_t FlashPageBuffer[256];

extern void PowerLED_On(void);
extern void PowerLED_Off(void);
extern void RelaysModule_Update(uint16_t, uint16_t, uint16_t);
extern void RelaysModule_Reset(void);
extern void Display_Update(void);
extern void SaveSettingsToFlash(uint32_t *);
extern void LoadSettingsFromFlash(uint32_t *);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI_nCS_REL_Pin GPIO_PIN_0
#define SPI_nCS_REL_GPIO_Port GPIOA
#define SPI_nCS_OLED_Pin GPIO_PIN_1
#define SPI_nCS_OLED_GPIO_Port GPIOA
#define OLED_RES_Pin GPIO_PIN_2
#define OLED_RES_GPIO_Port GPIOA
#define OLED_DC_Pin GPIO_PIN_3
#define OLED_DC_GPIO_Port GPIOA
#define SPI_MISO_Pin GPIO_PIN_6
#define SPI_MISO_GPIO_Port GPIOA
#define SPI_MOSI_Pin GPIO_PIN_7
#define SPI_MOSI_GPIO_Port GPIOA
#define BTN_IN_Pin GPIO_PIN_10
#define BTN_IN_GPIO_Port GPIOB
#define BTN_IN_EXTI_IRQn EXTI15_10_IRQn
#define BTN_OUT_Pin GPIO_PIN_11
#define BTN_OUT_GPIO_Port GPIOB
#define BTN_OUT_EXTI_IRQn EXTI15_10_IRQn
#define UART_DBG_TX_Pin GPIO_PIN_9
#define UART_DBG_TX_GPIO_Port GPIOA
#define UART_DBG_RX_Pin GPIO_PIN_10
#define UART_DBG_RX_GPIO_Port GPIOA
#define BTN_PWR_Pin GPIO_PIN_15
#define BTN_PWR_GPIO_Port GPIOA
#define BTN_PWR_EXTI_IRQn EXTI15_10_IRQn
#define FLASH_nWP_Pin GPIO_PIN_6
#define FLASH_nWP_GPIO_Port GPIOB
#define LED_PWR_Pin GPIO_PIN_7
#define LED_PWR_GPIO_Port GPIOB
#define LED_DBG_Pin GPIO_PIN_8
#define LED_DBG_GPIO_Port GPIOB
#define SPI_nCS_FLASH_Pin GPIO_PIN_9
#define SPI_nCS_FLASH_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define OLED_Enable() HAL_GPIO_WritePin(SPI_nCS_OLED_GPIO_Port, SPI_nCS_OLED_Pin, GPIO_PIN_RESET) // Макрос активации дисплея
#define OLED_Disable() HAL_GPIO_WritePin(SPI_nCS_OLED_GPIO_Port, SPI_nCS_OLED_Pin, GPIO_PIN_SET) // Макрос деактивации дисплея
#define OLED_DataEnable() HAL_GPIO_WritePin(SPI_nCS_OLED_GPIO_Port, SPI_nCS_OLED_Pin, GPIO_PIN_SET) // Макрос апереключения дисплея в режим данных
#define OLED_CommandEnable() HAL_GPIO_WritePin(SPI_nCS_OLED_GPIO_Port, SPI_nCS_OLED_Pin, GPIO_PIN_RESET) // Макрос переключения дисплея в режим команд

#define RelaysModule_Enable() HAL_GPIO_WritePin(SPI_nCS_REL_GPIO_Port, SPI_nCS_REL_Pin, GPIO_PIN_RESET) // Макрос активации релейного модуля
#define RelaysModule_Disable() HAL_GPIO_WritePin(SPI_nCS_REL_GPIO_Port, SPI_nCS_REL_Pin, GPIO_PIN_SET) // Макрос деактивации сдвигового регистра

#define FALSE 0
#define TRUE 1

#define OFF 0
#define ON 1

#define OPEN 0
#define CLOSE 1

#define NORMAL 0
#define SETUP 1
#define STANDBY 2
#define EMERGENCY 3

#define SETUP_PAGE1 0
#define SETUP_PAGE2 1
#define SETUP_PAGE3 2
#define SETUP_PAGE4 3

#define BLACK 0 // Black color, no pixel
#define WHITE 1 // Pixel is set. Color depends on OLED

#define IN_OUT 0 // Входы/выходы
#define OUT_IN 1 // Выходы/входы

#define NUMBERS 0 // Отображение цифрами
#define NAMES 1 // Отображение именами

#define MAX_MODULES 4 // Максимально поддерживаемое количество релейных модулей
#define RELAY_CNT 8   // Количество реле в одном релейном модуле

#define DEBOUCE_TIME 20U
#define LONGPRESS_TIME 800U
#define DISPLAYUPDATE_TIME 30U
#define WELCOME_TIME 3000U


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
