/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <disp1color.h>
#include <font.h>
#include <string.h>
#include "usbd_cdc_if.h"
#include "buttons.h"
#include "display.h"
#include "ext_flash.h"
#include "info_output.h"
#include "relays.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// Настраиваемые параметры
uint8_t ModulesCount_Par = 1; // Количество релейных модулей
uint8_t InputsCount_Par = 4; // Количество входов в модуле
uint8_t OutputsCount_Par = 4; // Количество выходов в модуле
uint8_t InOutOrder_Par = OUT_IN;  // Параметр, отвечающий за порядок расположения на разъемах - входы/выходы или выходы/входы
// uint8_t IODisplayMode_Par = NUMBERS; // Параметр отображения входов/выходов (цифрами или именами)
// Флаги задач
volatile uint8_t DeBouncer_Task = FALSE;
volatile uint8_t CountdownLongPress_Task = FALSE;
volatile uint8_t ScanButtonsShort_Task = FALSE;
volatile uint8_t ScanButtonsLong_Task = FALSE;
volatile uint8_t RelaysUpdate_Task = FALSE;
volatile uint8_t RelaysModuleUpdate_Task = FALSE;
volatile uint8_t DisplayUpdate_Task = FALSE;
volatile uint8_t DisplayWelcome_Task = FALSE;
// Дополнительные флаги
volatile uint8_t AllowSaveMute_Flag = FALSE; // Флаг разрешения сохранения состояния MUTE
// Таймеры обработки нажатий кнопок
volatile uint16_t DeBouncer_Timer = DEBOUCE_TIME;
volatile uint16_t LongPress_Timer = LONGPRESS_TIME;
volatile uint16_t DisplayWelcome_Timer = WELCOME_TIME;
// Переменные состояния системы
uint8_t System_State = STANDBY; // Текущий режим работы устройства
uint8_t SetupStage_State = 0; // Текущий этап режима настройки
uint8_t Mute_State = FALSE; // Переменная состояния режима MUTE
uint8_t Lock_State = FALSE; // переменная состояния блокировки изменения входов и выходов
uint8_t ActiveInput = 0;  // Текущий вход
uint8_t ActiveOutput = 0; // Текущий выход
// Состояния кнопок
uint8_t PowerButton_State = OPEN; // Переменная состояния кнопки POWER
uint8_t InputButton_State = OPEN; // Переменная состояния кнопки INPUT
uint8_t OutputButton_State = OPEN; // Переменная состояния кнопки OUTPUT
uint8_t LockButton_State = OPEN; // Переменная состояния кнопки LOCK (долгое нажатие INPUT)
uint8_t MuteButton_State = OPEN; // Переменная состояния кнопки MUTE (долгое нажатие OUTPUT)

uint32_t SaveStartAddr = 0x000000; // Начальный адрес сохранения настроек
uint8_t FlashPageBuffer[256] = {0}; // Буфер чтения Flash

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USB_DEVICE_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  ExternalFlash_Init();

    // OLED Init
    disp1color_Init();
    disp1color_SetBrightness(255);
    disp1color_Sleep();

    // Enable Power LED
    PowerLED_On();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
        Tasks_Pooling();
        
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Tasks_Pooling(void)
{
    // Если есть задача сканировать короткие нажатия кнопок
    if (ScanButtonsShort_Task == TRUE)
    {
        ScanButtonsShort_Task = FALSE;
        ShortButtonPresses_Pooling();
    }

    // Если есть задача сканировать долгие нажатия кнопок
    if (ScanButtonsLong_Task == TRUE)
    {
        ScanButtonsLong_Task = FALSE;
        LongButtonPresses_Pooling();
    }

    // Если есть задача отправить данные в релейный модуль
    if (RelaysUpdate_Task == TRUE)
    {
        RelaysUpdate_Task = FALSE;
        RelaysModule_Update();
    }

    // Если есть задача обновления дисплея
    if (DisplayUpdate_Task == TRUE)
    {
        DisplayUpdate_Task = FALSE;
        Display_Update();
    }
}

/**
  * @brief  Функция-обработчик внешних прерываний для кнопок.
  * @retval Нет
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{   
    // Если прерывание произошло именно от кнопок
    if ((GPIO_Pin == BTN_PWR_Pin) | (GPIO_Pin == BTN_IN_Pin) | (GPIO_Pin == BTN_OUT_Pin))
    {
        DeBouncer_Timer = DEBOUCE_TIME; // Инициализируем таймер задачи подавления дребезга
        DeBouncer_Task = TRUE;          // Взводим флаг задачи подавления дребезга
    }
}

/**
  * @brief  Функция-обработчик прерываний приема по UART.
  * @retval Нет
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        // TODO: Описать прием команд по UART
        /* Нужно реализовать дополнительный режим работы - режим конфигурации, 
        в котором будет вызываться функция HAL_UART_Receive_IT, приниматься данные, 
        и по данному прерыванию они будут анализироваться.
        Подробнее - https://istarik.ru/blog/stm32/120.html
        */
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
