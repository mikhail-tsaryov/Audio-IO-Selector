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
// Saved and customizable values
extern uint8_t ModulesCount_Par;
extern uint8_t InputsCount_Par;
extern uint8_t OutputsCount_Par;
extern uint8_t InOutOrder_Par; 
// Save HW options
extern uint32_t SaveStartAddr;
extern uint8_t FlashPageBuffer[256];

// Task flags
extern volatile uint8_t CountdownLongPress_Task;
extern volatile uint8_t DeBouncer_Task;
extern volatile uint8_t ScanButtonsShort_Task;
extern volatile uint8_t ScanButtonsLong_Task;
extern volatile uint8_t RelaysUpdate_Task;
extern volatile uint8_t RelaysModuleUpdate_Task;
extern volatile uint8_t DisplayUpdate_Task;
extern volatile uint8_t DisplayWelcome_Task;
// Additional flags
extern volatile uint8_t AllowSaveMute_Flag;

// Software timers
extern volatile uint16_t DeBouncer_Timer;
extern volatile uint16_t LongPress_Timer;
extern volatile uint16_t DisplayUpdate_Timer;
extern volatile uint16_t DisplayWelcome_Timer;

// State variables
extern uint8_t System_State; 
extern uint8_t SetupStage_State;  
extern uint8_t Mute_State;
extern uint8_t Lock_State;
extern uint8_t ActiveInput;
extern uint8_t ActiveOutput;

// Buttons states
extern uint8_t PowerButton_State; 
extern uint8_t InputButton_State; 
extern uint8_t OutputButton_State;
extern uint8_t LockButton_State; // Virtual button - long press Input
extern uint8_t MuteButton_State; // Virtual button - long press Output

extern char UART_BufSrtingTransmit[32];
extern char UART_BufSrtingReceive;

extern void Tasks_Pooling(void);

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

// Hardware parameters
#define MAX_MODULES 4 // Maximum number of relay modules
#define RELAY_CNT 8   // Number of relays on one module

// Time parameters
#define DEBOUCE_TIME 30U    // Debouncer time
#define LONGPRESS_TIME 800U // Button long press time
#define WELCOME_TIME 3000U  // Welcome screen time

// Buttons states
#define OPEN 0 
#define CLOSE 1

// Task states
#define FALSE 0
#define TRUE 1

#define OFF 0
#define ON 1

// System states
#define NORMAL 0 
#define SETUP 1
#define STANDBY 2
#define EMERGENCY 3

// Setup mode pages
#define SETUP_PAGE1 0 // Setup number relay modules
#define SETUP_PAGE2 1 // Setup I/Os number
#define SETUP_PAGE3 2 // Setup I/O order
#define SETUP_PAGE4 3 // 

// Display color
#define BLACK 0 // Black color, no pixel
#define WHITE 1 // Pixel is set. Color depends on OLED

// I/O order variants
#define IN_OUT 0 // Inputs/outputs
#define OUT_IN 1 // Outputs/inputs

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
