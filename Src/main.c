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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define DEBUG

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// Параметры
uint8_t ModulesCount_Par = 1; // Количество релейных модулей
uint8_t InputsCount_Par = 4; // Количество входов в модуле
uint8_t OutputsCount_Par = 4; // Количество выходов в модуле
uint8_t InOutOrder_Par = OUT_IN;  // Параметр, отвечающий за порядок расположения на разъемах - входы/выходы или выходы/входы
uint8_t IODisplayMode_Par = NUMBERS; // Параметр отображения входов/выходов (цифрами или именами)
// Флаги задач
volatile uint8_t DeBouncer_Task = FALSE;
volatile uint8_t CountdownLongPress_Task = FALSE;
volatile uint8_t ScanButtonsShort_Task = FALSE;
volatile uint8_t ScanButtonsLong_Task = FALSE;
volatile uint8_t RelaysUpdate_Task = FALSE;
volatile uint8_t RelaysModuleUpdate_Task = FALSE;
volatile uint8_t DisplayUpdate_Task = FALSE;
// Дополнительные флаги
volatile uint8_t AllowSaveMute_Flag = FALSE; // Флаг разрешения сохранения состояния MUTE
volatile uint8_t AllowInputChange_Flag = FALSE; // Флаг разрешения изменения количества модулей (введена, чтобы не допустить изменение параметра при входе в режим настройки)
// Таймеры
volatile uint16_t DeBouncer_Timer = DEBOUCE_TIME;
volatile uint16_t LongPress_Timer = LONGPRESS_TIME;
volatile uint16_t DisplayUpdate_Timer = DISPLAYUPDATE_TIME;
// Переменные состояния
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
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void PowerLED_On(void);
void PowerLED_Off(void);
void ScanButtonsShort(void);
void ScanButtonsLong(void);
void RelaysModule_Update(uint16_t, uint16_t, uint16_t);
void RelaysModule_Reset(void);
void Display_Update(void);
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
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

    // OLED Init
    disp1color_Init();
    disp1color_SetBrightness(255);
    disp1color_Sleep();
    
    PowerLED_On();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      // Если есть задача сканировать короткие нажатия кнопок
      if (ScanButtonsShort_Task == TRUE)
      {
          ScanButtonsShort_Task = FALSE;
          ScanButtonsShort();
      }

      // Если есть задача сканировать долгие нажатия кнопок
      if (ScanButtonsLong_Task == TRUE)
      {
          ScanButtonsLong_Task = FALSE;
          ScanButtonsLong();
      }

      // Если есть задача отправить данные в релейный модуль
      if (RelaysUpdate_Task == TRUE)
      {
          RelaysUpdate_Task = FALSE;
          //RelaysModule_Update();
      }

      // Если есть задача обновления дисплея
      if (DisplayUpdate_Task == TRUE)
      {
          DisplayUpdate_Task = FALSE;
          Display_Update();
      }
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
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
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

/**
  * @brief  Функция отключает светодиод питания.
  * @retval Нет
  */
inline void PowerLED_Off(void)
{
    HAL_GPIO_WritePin(LED_PWR_GPIO_Port, LED_PWR_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  Функция включает светодиод питания.
  * @retval Нет
  */
inline void PowerLED_On(void)
{
    HAL_GPIO_WritePin(LED_PWR_GPIO_Port, LED_PWR_Pin, GPIO_PIN_SET);
}

/**
  * @brief  Функция опроса коротких нажатий кнопок.
  * @retval Нет
  */
void ScanButtonsShort(void)
{
    // Проверить нажатие кнопки POWER
    if (HAL_GPIO_ReadPin(BTN_PWR_GPIO_Port, BTN_PWR_Pin) == GPIO_PIN_RESET) // Ветка выполняется при нажатии кнопки
    {
        LongPress_Timer = LONGPRESS_TIME; // Инициализируем таймер долгого нажатия
        CountdownLongPress_Task = TRUE;   // Ставим задачу отсчета времени долгого нажатия
        (PowerButton_State == OPEN) ? (PowerButton_State = CLOSE) : (PowerButton_State = OPEN);
    }
    else if (HAL_GPIO_ReadPin(BTN_PWR_GPIO_Port, BTN_PWR_Pin) == GPIO_PIN_SET) // Ветка выполняется при отпускании кнопки
    {
        if (PowerButton_State == CLOSE)
        {
            CountdownLongPress_Task = FALSE; // Снимаем задачу отсчета времени долгого нажатия
            PowerButton_State = OPEN;
            //-----------------------------------------------------------------
            // Зафиксировано короткое нажатие кнопки POWER
            //-----------------------------------------------------------------
            if (System_State == STANDBY)
            {
                PowerLED_Off();        // Отключить светодиод питания
                disp1color_Wake();
                // Приветствие
                disp1color_FillScreenbuff(0);
                //disp1color_DrawRectangle(0, 0, DISP1COLOR_Width - 1, DISP1COLOR_Height - 1);

                #ifndef RUS_LANG
                disp1color_printfCenterAlign(0, 8, FONTID_10X16F, "Audio");
                disp1color_printfCenterAlign(0, 24, FONTID_10X16F, "I/O Selector");
                disp1color_printfCenterAlign(0, 50, FONTID_6X8M, "© M.Tsaryov, %d.3", 2020);
                #else
                disp1color_printfCenterAlign(0, 8, FONTID_10X16F, "Аудио");
                disp1color_printfCenterAlign(0, 24, FONTID_10X16F, "коммутатор");
                disp1color_printfCenterAlign(0, 50, FONTID_6X8M, "© М.Царёв, %d.3", 2020);
                #endif
                disp1color_UpdateFromBuff();
                HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
                HAL_Delay(WELCOME_TIME);
                HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

                System_State = NORMAL; // Сменить режим работы
                AllowSaveMute_Flag = TRUE;
                RelaysModuleUpdate_Task = TRUE;
            }
            // Если текущий режим NORMAL, переключиться в дежурный режим
            else if (System_State == NORMAL)
            {
                System_State = STANDBY;         // Сменить режим работы
                // Погасить экран
                disp1color_Sleep();
                // Отключить выходы и обновить состояние релейного модуля, не сохраняя режим MUTE
                AllowSaveMute_Flag = FALSE;
                Mute_State = ON;
                RelaysModuleUpdate_Task = TRUE;
                PowerLED_On(); // Включить светодиод питания
            }
            // Если текущий режим SETUP
            else if (System_State == SETUP)
            {
                // Если сейчас 1 этап настройка, переключиться на 2 этап
                if (SetupStage_State == SETUP_PAGE1)
                {
                    SetupStage_State = SETUP_PAGE2;
                }
                // Если сейчас 2 этап настройки, переключиться на 3 этап
                else if (SetupStage_State == SETUP_PAGE2)
                {
                    SetupStage_State = SETUP_PAGE3;
                }
                // Если сейчас 3 этап настройки, переключиться на NORMAL
                else if (SetupStage_State == SETUP_PAGE3)
                {
                    System_State = NORMAL;
                    SetupStage_State = SETUP_PAGE1;
                    AllowInputChange_Flag = FALSE;
                    AllowSaveMute_Flag = TRUE;
                }
            }
        }
    }

    // Проверить нажатие кнопки INPUT
    if (HAL_GPIO_ReadPin(BTN_IN_GPIO_Port, BTN_IN_Pin) == GPIO_PIN_RESET) // Ветка выполняется при нажатии кнопки
    {
        LongPress_Timer = LONGPRESS_TIME; // Инициализируем таймер долгого нажатия
        CountdownLongPress_Task = TRUE;   // Ставим задачу отсчета времени долгого нажатия
        (InputButton_State == OPEN) ? (InputButton_State = CLOSE) : (InputButton_State = OPEN);
    }
    else if (HAL_GPIO_ReadPin(BTN_IN_GPIO_Port, BTN_IN_Pin) == GPIO_PIN_SET) // Ветка выполняется при отпускании кнопки
    {
        if (InputButton_State == CLOSE)
        {
            CountdownLongPress_Task = FALSE; // Снимаем задачу отсчета времени долгого нажатия
            InputButton_State = OPEN;
            //-----------------------------------------------------------------
            // Зафиксировано короткое нажатие кнопки INPUT
            //-----------------------------------------------------------------
            // Если текущий режим NORMAL
            if (System_State == NORMAL)
            {
                // Сменить вход, если не включена блокировка
                if (Lock_State == OFF)
                {
                    // Если вход не последний, инкрементировать вход, иначе включить первый 
                    (ActiveInput < (InputsCount_Par - 1)) ? (ActiveInput++) : (ActiveInput = 0);
                    // Обновить состояние релейного модуля
                    RelaysModuleUpdate_Task = TRUE;
                    // Передать по USB
                    char str_tx[31];
                    sprintf(str_tx, "Input %d\r\n", ActiveInput);
                    CDC_Transmit_FS((unsigned char *)str_tx, strlen(str_tx));
                }
            }
            // Если текущий режим SETUP
            else if (System_State == SETUP)
            {
                if (SetupStage_State == SETUP_PAGE1)
                {
                    if (AllowInputChange_Flag == FALSE)
                    {
                        AllowInputChange_Flag = TRUE;
                    }
                    else if (ModulesCount_Par < MAX_MODULES)
                    {
                        ModulesCount_Par++;
                    }
                    else
                    {
                        ModulesCount_Par = 1;
                    }

                    if ((InputsCount_Par > ModulesCount_Par * RELAY_CNT / 2) | (OutputsCount_Par > ModulesCount_Par * RELAY_CNT - InputsCount_Par))
                    {
                        InputsCount_Par = ModulesCount_Par * RELAY_CNT / 2;
                        OutputsCount_Par = ModulesCount_Par * RELAY_CNT - InputsCount_Par;
                    }

                    // Сбросить включенный вход на значение по умолчанию,
                    // если он не попадает в настройки количества входов
                    if (ActiveInput + 1 > InputsCount_Par)
                    {
                        Mute_State = ON;
                        ActiveInput = 0;
                    }

                    // Сбросить включенный выход на значение по умолчанию,
                    // если он не попадает в настройки количества выходов
                    if (ActiveOutput + 1 > OutputsCount_Par)
                    {
                        Mute_State = ON;
                        ActiveOutput = 0;
                    }
                }

                else if (SetupStage_State == SETUP_PAGE2)
                {
                    if (InputsCount_Par < ModulesCount_Par * RELAY_CNT - 1)
                    {
                        InputsCount_Par++;
                    }
                    else
                    {
                        InputsCount_Par = 1;
                    }

                    OutputsCount_Par = ModulesCount_Par * RELAY_CNT - InputsCount_Par;

                    // Сбросить включенный вход на значение по умолчанию,
                    // если он не попадает в настройки количества входов
                    if (ActiveInput + 1 > InputsCount_Par)
                    {
                        Mute_State = ON;
                        ActiveInput = 0;
                    }

                    // Сбросить включенный выход на значение по умолчанию,
                    // если он не попадает в настройки количества выходов
                    if (ActiveOutput + 1 > OutputsCount_Par)
                    {
                        Mute_State = ON;
                        ActiveOutput = 0;
                    }
                }

                else if (SetupStage_State == SETUP_PAGE3)
                {
                    (InOutOrder_Par == IN_OUT) ? (InOutOrder_Par = OUT_IN) : (InOutOrder_Par = IN_OUT);
                }
            }
        }
    }

    // Проверить нажатие кнопки OUTPUT
    if (HAL_GPIO_ReadPin(BTN_OUT_GPIO_Port, BTN_OUT_Pin) == GPIO_PIN_RESET) // Ветка выполняется при нажатии кнопки
    {
        LongPress_Timer = LONGPRESS_TIME; // Инициализируем таймер долгого нажатия
        CountdownLongPress_Task = TRUE;   // Ставим задачу отсчета времени долгого нажатия
        (OutputButton_State == OPEN) ? (OutputButton_State = CLOSE) : (OutputButton_State = OPEN);
    }
    else if (HAL_GPIO_ReadPin(BTN_OUT_GPIO_Port, BTN_OUT_Pin) == GPIO_PIN_SET) // Ветка выполняется при отпускании кнопки
    {
        if (OutputButton_State == CLOSE)
        {
            CountdownLongPress_Task = FALSE; // Снимаем задачу отсчета времени долгого нажатия
            OutputButton_State = OPEN;
            //-----------------------------------------------------------------
            // Зафиксировано короткое нажатие кнопки OUTPUT
            //-----------------------------------------------------------------
            // Если текущий режим NORMAL
            if (System_State == NORMAL)
            {
                // Сменить выход, если не включена блокировка
                if (Lock_State == OFF)
                {
                    // Если выход не последний, инкрементировать вход, иначе включить первый
                    (ActiveOutput < (OutputsCount_Par - 1)) ? (ActiveOutput++) : (ActiveOutput = 0);
                    // Обновить состояние релейного модуля
                    RelaysModuleUpdate_Task = TRUE;
                    // Передать по USB
                    char str_tx[31];
                    sprintf(str_tx, "Output %d\r\n", ActiveOutput);
                    CDC_Transmit_FS((unsigned char *)str_tx, strlen(str_tx));
                }
            }
            // Если текущий режим SETUP
            else if (System_State == SETUP)
            {
                if (SetupStage_State == SETUP_PAGE1)
                {
                    if (AllowInputChange_Flag == FALSE)
                    {
                        AllowInputChange_Flag = TRUE;
                    }
                    else if (ModulesCount_Par > 1)
                    {
                        ModulesCount_Par--;
                    }
                    else
                    {
                        ModulesCount_Par = MAX_MODULES;
                    }

                    if ((InputsCount_Par > ModulesCount_Par * RELAY_CNT / 2) | (OutputsCount_Par > ModulesCount_Par * RELAY_CNT - InputsCount_Par))
                    {
                        InputsCount_Par = ModulesCount_Par * RELAY_CNT / 2;
                        OutputsCount_Par = ModulesCount_Par * RELAY_CNT - InputsCount_Par;
                    }

                    // Сбросить включенный вход на значение по умолчанию,
                    // если он не попадает в настройки количества входов
                    if (ActiveInput + 1 > InputsCount_Par)
                    {
                        Mute_State = ON;
                        ActiveInput = 0;
                    }

                    // Сбросить включенный выход на значение по умолчанию,
                    // если он не попадает в настройки количества выходов
                    if (ActiveOutput + 1 > OutputsCount_Par)
                    {
                        Mute_State = ON;
                        ActiveOutput = 0;
                    }
                }

                else if (SetupStage_State == SETUP_PAGE2)
                {
                    if (OutputsCount_Par < ModulesCount_Par * RELAY_CNT - 1)
                    {
                        OutputsCount_Par++;
                    }
                    else
                    {
                        OutputsCount_Par = 1;
                    }

                    InputsCount_Par = ModulesCount_Par * RELAY_CNT - OutputsCount_Par;

                    // Сбросить включенный вход на значение по умолчанию,
                    // если он не попадает в настройки количества входов
                    if (ActiveInput + 1 > InputsCount_Par)
                    {
                        Mute_State = ON;
                        ActiveInput = 0;
                    }

                    // Сбросить включенный выход на значение по умолчанию,
                    // если он не попадает в настройки количества выходов
                    if (ActiveOutput + 1 > OutputsCount_Par)
                    {
                        Mute_State = ON;
                        ActiveOutput = 0;
                    }
                }

                else if (SetupStage_State == SETUP_PAGE3)
                {
                    (InOutOrder_Par == IN_OUT) ? (InOutOrder_Par = OUT_IN) : (InOutOrder_Par = IN_OUT);
                }
            }
        }
    }

    DisplayUpdate_Task = TRUE; // Ставим флаг задачи обновления дисплея
}

/**
  * @brief  Функция опроса длинных нажатий кнопок.
  * @retval Нет
  */
void ScanButtonsLong(void)
{
    if ((PowerButton_State == CLOSE))
    {
        PowerButton_State = OPEN;
        //-----------------------------------------------------------------
        // Зафиксировано длинное нажатие кнопки POWER
        //-----------------------------------------------------------------
        HAL_GPIO_TogglePin(LED_DBG_GPIO_Port, LED_DBG_Pin);

        // Если текущий режим NORMAL
        if (System_State == NORMAL)
        {
            // Переключиться в режим SETUP
            System_State = SETUP;
            PowerLED_On();
        }
        // Если текущий режим SETUP
        else if (System_State == SETUP)
        {
            // Переключиться в режим NORMAL
            System_State = NORMAL;
            PowerLED_Off();
        }
    }

    else if ((InputButton_State == CLOSE))
    {
        InputButton_State = OPEN;
        //-----------------------------------------------------------------
        // Зафиксировано длинное нажатие кнопки INPUT
        //-----------------------------------------------------------------
        HAL_GPIO_TogglePin(LED_DBG_GPIO_Port, LED_DBG_Pin);
        // Если текущий режим NORMAL
        if (System_State == NORMAL)
        {
            (Lock_State == ON) ? (Lock_State = OFF) : (Lock_State = ON); // Переключить режим блокировки
        }
    }

    else if ((OutputButton_State == CLOSE))
    {
        OutputButton_State = OPEN;
        //-----------------------------------------------------------------
        // Зафиксировано длинное нажатие кнопки OUTPUT
        //-----------------------------------------------------------------
        HAL_GPIO_TogglePin(LED_DBG_GPIO_Port, LED_DBG_Pin); 
        // Если текущий режим NORMAL
        if (System_State == NORMAL)
        {
            (Mute_State == ON) ? (Mute_State = OFF) : (Mute_State = ON); // Переключить режим MUTE
        }
    }

    DisplayUpdate_Task = TRUE; // Ставим флаг задачи обновления дисплея
}

/**
  * @brief  Функция обновления OLED-дисплея.
  * @retval Нет
  */
void Display_Update(void)
{
    disp1color_FillScreenbuff(BLACK);

    if (System_State == NORMAL)
    {
        disp1color_DrawRectangle(0, 18, 40, 47);
        disp1color_printf(3, 2, FONTID_10X16F, "%c%c%c", 0x1E, 0x1E, 0x1E);
        disp1color_printf(3, 48, FONTID_10X16F, "%c%c%c", 0x1C, 0x1C, 0x1C);
        disp1color_printfCenterAlign(5, 24, FONTID_10X16F, "%d", ActiveInput + 1);
        disp1color_printfCenterAlign(45, 24, FONTID_10X16F, "%d", ActiveOutput + 1);
        #ifndef RUS_LANG
        disp1color_printfCenterAlign(24, 3, FONTID_6X8M, "DAC Mercury");
        disp1color_printfCenterAlign(24, 54, FONTID_6X8M, "AMP Neptune");
        #else
        disp1color_printfCenterAlign(24, 3, FONTID_6X8M, "DAC Mercury");
        disp1color_printfCenterAlign(24, 54, FONTID_6X8M, "AMP Neptune");
        #endif

        if (Lock_State == ON)
        {
            disp1color_printf(16, 24, FONTID_10X16F, "%c", 0x1F); // Значок блокировки
        }

        if (Mute_State == ON)
        {
            // Стрелка
            disp1color_DrawLine(83, 31, 93, 31);
            disp1color_DrawLine(83, 32, 93, 32);
            disp1color_DrawLine(89, 27, 93, 31);
            disp1color_DrawLine(89, 28, 93, 32);
            disp1color_DrawLine(89, 35, 93, 31);
            disp1color_DrawLine(89, 36, 93, 32);
        }
    }

    // Если текущий режим SETUP
    else if (System_State == SETUP)
    {
        // Заголовок
        #ifndef RUS_LANG
        disp1color_printfCenterAlign(0, 2, FONTID_10X16F, "Settings");
        #else
        disp1color_printfCenterAlign(0, 2, FONTID_10X16F, "Настройки");
        #endif
        disp1color_DrawRectangleFilled(0, 20, DISP1COLOR_Width - 1, 20);

        // Если сейчас 1 этап настройки,
        if (SetupStage_State == SETUP_PAGE1)
        {
            // Количество установленных релейный модулей
            
            #ifndef RUS_LANG
            disp1color_printf(5, 27, FONTID_10X16F, "Relay\r\nmodules");
            #else
            disp1color_printf(5, 27, FONTID_10X16F, "Релейных\r\nмодулей");
            #endif
            disp1color_printf(100, 36, FONTID_10X16F, "%d", ModulesCount_Par);
        }

        // Если сейчас 2 этап настройки,
        else if (SetupStage_State == SETUP_PAGE2)
        {            
            // Количество входов и выходов
            #ifndef RUS_LANG
            disp1color_printf(5, 27, FONTID_10X16F, "Inputs\r\nOutputs");
            #else
            disp1color_printf(5, 27, FONTID_10X16F, "Входов\r\nВыходов");
            #endif
            disp1color_printf(100, 27, FONTID_10X16F, "%d\r\n%d", InputsCount_Par, OutputsCount_Par);
        }

        // Если сейчас 3 этап настройки,
        else if (SetupStage_State == SETUP_PAGE3)
        {
            // Порядок расположения входов/выходов
            #ifndef RUS_LANG
            disp1color_printfCenterAlign(0, 27, FONTID_10X16F, "I/O order");
            #else
            disp1color_printfCenterAlign(0, 27, FONTID_10X16F, "Порядок");
            #endif
            disp1color_DrawRectangle(13, 60, DISP1COLOR_Width - 15, 65);

            if (InOutOrder_Par == IN_OUT)
            {
                disp1color_printfCenterAlign(0, 44, FONTID_10X16F, "%c%c%c ... %c%c%c", 0x1E, 0x1E, 0x1E, 0x1D, 0x1D, 0x1D);
            }
            else if (InOutOrder_Par == OUT_IN)
            {
                disp1color_printfCenterAlign(0, 44, FONTID_10X16F, "%c%c%c ... %c%c%c", 0x1D, 0x1D, 0x1D, 0x1E, 0x1E, 0x1E);
            }
        }
        // Если сейчас 4 этап настройки,
        else if (SetupStage_State == SETUP_PAGE4)
        {
            //  Режим отображения входов/выходов
            #ifndef RUS_LANG
            disp1color_printfCenterAlign(0, 27, FONTID_10X16F, "Display");
            #else
            disp1color_printfCenterAlign(0, 27, FONTID_10X16F, "Отображение");
            #endif
            disp1color_DrawRectangle(12, 44, DISP1COLOR_Width - 1 - 12, DISP1COLOR_Height - 1);

            if (IODisplayMode_Par == NUMBERS)
            {
                disp1color_printfCenterAlign(0, 46, FONTID_10X16F, "14 > 8");
            }
            else
            {
                disp1color_printfCenterAlign(0, 46, FONTID_10X16F, "DAC > Amp");
            }
        }
    }

    // Вывести буфер кадра на дисплей
    disp1color_UpdateFromBuff();
}

/**
  * @brief  Функция формирования и отправки данных на релейный модуль.
  * @retval Нет
  */
void RelaysModule_Update(uint16_t Input, uint16_t Output, uint16_t Mute)
{
    uint32_t Frame = 0;
    
    // Сформировать данные для отправки
    // Если настроен порядок "входы/выходы"
    if (InOutOrder_Par == IN_OUT)
    {
        Frame = ((1 << (Input + OutputsCount_Par)) | (1 << Output)) * (!Mute & 1);
        // Переменная mute инвертируется и перемножается с маской, чтобы выделить младший бит
        // Тем самым при активном MUTE temp станет равным нулю
    }
    // Если настроен порядок "выходы/входы"
    else if (InOutOrder_Par == OUT_IN)
    {
        Frame = ((1 << (Output + InputsCount_Par)) | (1 << Input)) * (!Mute & 1);
    }

    // Сбросить релейный модуль
    RelaysModule_Reset();

    // Отправить новые данные по SPI
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&Frame, ModulesCount_Par, 1);
    // Сгенерировать импульс для переноса данных между регистрами TPIC6B595
    HAL_GPIO_WritePin(SPI_nCS_REL_GPIO_Port, SPI_nCS_REL_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SPI_nCS_REL_GPIO_Port, SPI_nCS_REL_Pin, GPIO_PIN_SET);
    // Небольшая задержка, чтобы реле успели сработать
    HAL_Delay(5);
}

/**
  * @brief  Функция сброса релейного модуля.
  * @retval Нет
  */
inline void RelaysModule_Reset(void)
{
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET);

    // Сгенерировать импульс для переноса данных между регистрами TPIC6B595
    HAL_GPIO_WritePin(SPI_nCS_REL_GPIO_Port, SPI_nCS_REL_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SPI_nCS_REL_GPIO_Port, SPI_nCS_REL_Pin, GPIO_PIN_SET);

    // Небольшая задержка, чтобы реле успели выключиться
    HAL_Delay(5);
}

/**
  * @brief  Функция-обработчик внешних прерываний.
  * @retval Нет
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{   
    // Если не запущена обработка другой кнопки
    if ((GPIO_Pin == BTN_PWR_Pin) | (GPIO_Pin == BTN_IN_Pin) | (GPIO_Pin == BTN_OUT_Pin))
    {
        DeBouncer_Timer = DEBOUCE_TIME; // Инициализируем таймер
        DeBouncer_Task = TRUE;          // Взводим флаг задачи подавления дребезга
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
