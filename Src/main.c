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
#include "ext_flash.h"
#include "info_output.h"
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
// ���������
uint8_t ModulesCount_Par = 1; // ���������� �������� �������
uint8_t InputsCount_Par = 4; // ���������� ������ � ������
uint8_t OutputsCount_Par = 4; // ���������� ������� � ������
uint8_t InOutOrder_Par = OUT_IN;  // ��������, ���������� �� ������� ������������ �� �������� - �����/������ ��� ������/�����
uint8_t IODisplayMode_Par = NUMBERS; // �������� ����������� ������/������� (������� ��� �������)
// ����� �����
volatile uint8_t DeBouncer_Task = FALSE;
volatile uint8_t CountdownLongPress_Task = FALSE;
volatile uint8_t ScanButtonsShort_Task = FALSE;
volatile uint8_t ScanButtonsLong_Task = FALSE;
volatile uint8_t RelaysUpdate_Task = FALSE;
volatile uint8_t RelaysModuleUpdate_Task = FALSE;
volatile uint8_t DisplayUpdate_Task = FALSE;
// �������������� �����
volatile uint8_t AllowSaveMute_Flag = FALSE; // ���� ���������� ���������� ��������� MUTE
// �������
volatile uint16_t DeBouncer_Timer = DEBOUCE_TIME;
volatile uint16_t LongPress_Timer = LONGPRESS_TIME;
volatile uint16_t DisplayUpdate_Timer = DISPLAYUPDATE_TIME;
// ���������� ���������
uint8_t System_State = STANDBY; // ������� ����� ������ ����������
uint8_t SetupStage_State = 0; // ������� ���� ������ ���������
uint8_t Mute_State = FALSE; // ���������� ��������� ������ MUTE
uint8_t Lock_State = FALSE; // ���������� ��������� ���������� ��������� ������ � �������
uint8_t ActiveInput = 0;  // ������� ����
uint8_t ActiveOutput = 0; // ������� �����
// ��������� ������
uint8_t PowerButton_State = OPEN; // ���������� ��������� ������ POWER
uint8_t InputButton_State = OPEN; // ���������� ��������� ������ INPUT
uint8_t OutputButton_State = OPEN; // ���������� ��������� ������ OUTPUT
uint8_t LockButton_State = OPEN; // ���������� ��������� ������ LOCK (������ ������� INPUT)
uint8_t MuteButton_State = OPEN; // ���������� ��������� ������ MUTE (������ ������� OUTPUT)

uint32_t SaveStartAddr = 0x000000; // ��������� ����� ���������� ��������
uint8_t FlashPageBuffer[256] = {0}; // ����� ������ Flash

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

  #ifdef INFO_OUTPUT
    SerialInfoOutput_PrintWelcomeInfo();
  #endif

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
        // ���� ���� ������ ����������� �������� ������� ������
        if (ScanButtonsShort_Task == TRUE)
        {
            ScanButtonsShort_Task = FALSE;
            ShortButtonPresses_Pooling();
        }

        // ���� ���� ������ ����������� ������ ������� ������
        if (ScanButtonsLong_Task == TRUE)
        {
            ScanButtonsLong_Task = FALSE;
            LongButtonPresses_Pooling();
        }

        // ���� ���� ������ ��������� ������ � �������� ������
        if (RelaysUpdate_Task == TRUE)
        {
            RelaysUpdate_Task = FALSE;
            RelaysModule_Update(ActiveInput, ActiveOutput, Mute_State);
        }

        // ���� ���� ������ ���������� �������
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

/**
  * @brief  ������� ��������� ��������� �������.
  * @retval ���
  */
inline void PowerLED_Off(void)
{
    HAL_GPIO_WritePin(LED_PWR_GPIO_Port, LED_PWR_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  ������� �������� ��������� �������.
  * @retval ���
  */
inline void PowerLED_On(void)
{
    HAL_GPIO_WritePin(LED_PWR_GPIO_Port, LED_PWR_Pin, GPIO_PIN_SET);
}

/**
  * @brief  ������� ���������� OLED-�������.
  * @retval ���
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
            disp1color_printf(16, 24, FONTID_10X16F, "%c", 0x1F); // ������ ����������
        }

        if (Mute_State == OFF)
        {
            // �������
            disp1color_DrawLine(83, 31, 93, 31);
            disp1color_DrawLine(83, 32, 93, 32);
            disp1color_DrawLine(89, 27, 93, 31);
            disp1color_DrawLine(89, 28, 93, 32);
            disp1color_DrawLine(89, 35, 93, 31);
            disp1color_DrawLine(89, 36, 93, 32);
        }
    }

    // ���� ������� ����� SETUP
    else if (System_State == SETUP)
    {
        // ���������
        #ifndef RUS_LANG
        disp1color_printfCenterAlign(0, 2, FONTID_10X16F, "Settings");
        #else
        disp1color_printfCenterAlign(0, 2, FONTID_10X16F, "���������");
        #endif
        disp1color_DrawRectangleFilled(0, 20, DISP1COLOR_Width - 1, 20);

        // ���� ������ 1 ���� ���������,
        if (SetupStage_State == SETUP_PAGE1)
        {
            // ���������� ������������� �������� �������
            
            #ifndef RUS_LANG
            disp1color_printf(5, 27, FONTID_10X16F, "Relay\r\nmodules");
            #else
            disp1color_printf(5, 27, FONTID_10X16F, "��������\r\n�������");
            #endif
            disp1color_printf(100, 36, FONTID_10X16F, "%d", ModulesCount_Par);
        }

        // ���� ������ 2 ���� ���������,
        else if (SetupStage_State == SETUP_PAGE2)
        {            
            // ���������� ������ � �������
            #ifndef RUS_LANG
            disp1color_printf(5, 27, FONTID_10X16F, "Inputs\r\nOutputs");
            #else
            disp1color_printf(5, 27, FONTID_10X16F, "������\r\n�������");
            #endif
            disp1color_printf(100, 27, FONTID_10X16F, "%d\r\n%d", InputsCount_Par, OutputsCount_Par);
        }

        // ���� ������ 3 ���� ���������,
        else if (SetupStage_State == SETUP_PAGE3)
        {
            // ������� ������������ ������/�������
            #ifndef RUS_LANG
            disp1color_printfCenterAlign(0, 27, FONTID_10X16F, "I/O order");
            #else
            disp1color_printfCenterAlign(0, 27, FONTID_10X16F, "�������");
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
        // ���� ������ 4 ���� ���������,
        else if (SetupStage_State == SETUP_PAGE4)
        {
            //  ����� ����������� ������/�������
            #ifndef RUS_LANG
            disp1color_printfCenterAlign(0, 27, FONTID_10X16F, "Display");
            #else
            disp1color_printfCenterAlign(0, 27, FONTID_10X16F, "�����������");
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

    // ������� ����� ����� �� �������
    disp1color_UpdateFromBuff();
}

/**
  * @brief  ������� ������������ � �������� ������ �� �������� ������.
  * @retval ���
  */
void RelaysModule_Update(uint16_t Input, uint16_t Output, uint16_t Mute)
{
    uint32_t Frame = 0;
    
    // ������������ ������ ��� ��������
    // ���� �������� ������� "�����/������"
    if (InOutOrder_Par == IN_OUT)
    {
        Frame = ((1 << (Input + OutputsCount_Par)) | (1 << Output)) * (!Mute & 1);
        // ���������� mute ������������� � ������������� � ������, ����� �������� ������� ���
        // ��� ����� ��� �������� MUTE temp ������ ������ ����
    }
    // ���� �������� ������� "������/�����"
    else if (InOutOrder_Par == OUT_IN)
    {
        Frame = ((1 << (Output + InputsCount_Par)) | (1 << Input)) * (!Mute & 1);
    }

    // �������� �������� ������
    RelaysModule_Reset();

    // ��������� ����� ������ �� SPI
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&Frame, ModulesCount_Par, 1);
    // ������������� ������� ��� �������� ������ ����� ���������� TPIC6B595
    HAL_GPIO_WritePin(SPI_nCS_REL_GPIO_Port, SPI_nCS_REL_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SPI_nCS_REL_GPIO_Port, SPI_nCS_REL_Pin, GPIO_PIN_SET);
    // ��������� ��������, ����� ���� ������ ���������
    HAL_Delay(5);
}

/**
  * @brief  ������� ������ ��������� ������.
  * @retval ���
  */
inline void RelaysModule_Reset(void)
{
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET);

    // ������������� ������� ��� �������� ������ ����� ���������� TPIC6B595
    HAL_GPIO_WritePin(SPI_nCS_REL_GPIO_Port, SPI_nCS_REL_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SPI_nCS_REL_GPIO_Port, SPI_nCS_REL_Pin, GPIO_PIN_SET);

    // ��������� ��������, ����� ���� ������ �����������
    HAL_Delay(5);
}

/**
  * @brief  �������-���������� ������� ����������.
  * @retval ���
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{   
    // ���� �� �������� ��������� ������ ������
    if ((GPIO_Pin == BTN_PWR_Pin) | (GPIO_Pin == BTN_IN_Pin) | (GPIO_Pin == BTN_OUT_Pin))
    {
        DeBouncer_Timer = DEBOUCE_TIME; // �������������� ������
        DeBouncer_Task = TRUE;          // ������� ���� ������ ���������� ��������
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
