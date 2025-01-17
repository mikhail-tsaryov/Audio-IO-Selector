#include "buttons.h"

/**
  * @brief  ������� ��������� ��������� ������� ������ POWER.
  * @retval ���
  */
void ShortPressPowerButton_Handler(void)
{
    // ���� ������� ����� STANDBY, ������������� � ����� NORMAL
    if (System_State == STANDBY)
    {
        System_State = NORMAL;                 // ������ ����� ������

        LoadSettingsFromFlash(&SaveStartAddr);
        AllowSaveMute_Flag = TRUE;
        RelaysModuleUpdate_Task = TRUE;

        DisplayWelcome_Task = TRUE;
        disp1color_Wake();
        //Display_Intro(WELCOME_TIME);

        PowerLED_Off(); // ��������� ��������� �������
    }
    // ���� ������� ����� NORMAL, ������������� � �������� �����
    else if (System_State == NORMAL)
    {
        System_State = STANDBY; // ������� ����� ������
        // �������� �����
        disp1color_Sleep();
        // ��������� ������ � �������� ��������� ��������� ������, �� �������� ����� MUTE
        AllowSaveMute_Flag = FALSE;
        Mute_State = ON;
        RelaysModuleUpdate_Task = TRUE;
        PowerLED_On(); // �������� ��������� �������
    }
    // ���� ������� ����� SETUP
    else if (System_State == SETUP)
    {
        // ���� ������ 1 ���� ���������, ������������� �� 2 ����
        if (SetupStage_State == SETUP_PAGE1)
        {
            SetupStage_State = SETUP_PAGE2;
        }
        // ���� ������ 2 ���� ���������, ������������� �� 3 ����
        else if (SetupStage_State == SETUP_PAGE2)
        {
            SetupStage_State = SETUP_PAGE3;
        }
        // ���� ������ 3 ���� ���������, ������������� �� NORMAL
        else if (SetupStage_State == SETUP_PAGE3)
        {
            SetupStage_State = SETUP_PAGE1;
        }
    }
}

/**
  * @brief  ������� ��������� ��������� ������� ������ INPUT.
  * @retval ���
  */
void ShortPressInputButton_Handler(void)
{
    // ���� ������� ����� NORMAL
    if (System_State == NORMAL)
    {
        // ������� ����, ���� �� �������� ����������
        if (Lock_State == OFF)
        {
            // ���� ���� �� ���������, ���������������� ����, ����� �������� ������
            (ActiveInput < (InputsCount_Par - 1)) ? (ActiveInput++) : (ActiveInput = 0);
            // �������� ��������� ��������� ������
            RelaysModuleUpdate_Task = TRUE;
        }
    }
    // ���� ������� ����� SETUP
    else if (System_State == SETUP)
    {
        if (SetupStage_State == SETUP_PAGE1)
        {
            if (ModulesCount_Par < MAX_MODULES)
            {
                ModulesCount_Par++;
            }
            else
            {
                ModulesCount_Par = 1;
            }

            if (InputsCount_Par > ModulesCount_Par * RELAY_CNT / 2)
            {
                InputsCount_Par = ModulesCount_Par * RELAY_CNT / 2;
            }
            OutputsCount_Par = ModulesCount_Par * RELAY_CNT - InputsCount_Par;

            // �������� ���������� ���� �� �������� �� ���������,
            // ���� �� �� �������� � ��������� ���������� ������
            if (ActiveInput + 1 > InputsCount_Par)
            {
                Mute_State = ON;
                ActiveInput = 0;
            }

            // �������� ���������� ����� �� �������� �� ���������,
            // ���� �� �� �������� � ��������� ���������� �������
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

            // �������� ���������� ���� �� �������� �� ���������,
            // ���� �� �� �������� � ��������� ���������� ������
            if (ActiveInput + 1 > InputsCount_Par)
            {
                Mute_State = ON;
                ActiveInput = 0;
            }

            // �������� ���������� ����� �� �������� �� ���������,
            // ���� �� �� �������� � ��������� ���������� �������
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

/**
  * @brief  ������� ��������� ��������� ������� ������ OUTPUT.
  * @retval ���
  */
void ShortPressOutputButton_Handler(void)
{
    // ���� ������� ����� NORMAL
    if (System_State == NORMAL)
    {
        // ������� �����, ���� �� �������� ����������
        if (Lock_State == OFF)
        {
            // ���� ����� �� ���������, ���������������� ����, ����� �������� ������
            (ActiveOutput < (OutputsCount_Par - 1)) ? (ActiveOutput++) : (ActiveOutput = 0);
            // �������� ��������� ��������� ������
            RelaysModuleUpdate_Task = TRUE;
        }
    }
    // ���� ������� ����� SETUP
    else if (System_State == SETUP)
    {
        if (SetupStage_State == SETUP_PAGE1)
        {
            if (ModulesCount_Par > 1)
            {
                ModulesCount_Par--;
            }
            else
            {
                ModulesCount_Par = MAX_MODULES;
            }

            if (InputsCount_Par > ModulesCount_Par * RELAY_CNT / 2)
            {
                InputsCount_Par = ModulesCount_Par * RELAY_CNT / 2;
            }
            OutputsCount_Par = ModulesCount_Par * RELAY_CNT - InputsCount_Par;

            // �������� ���������� ���� �� �������� �� ���������,
            // ���� �� �� �������� � ��������� ���������� ������
            if (ActiveInput + 1 > InputsCount_Par)
            {
                Mute_State = ON;
                ActiveInput = 0;
            }

            // �������� ���������� ����� �� �������� �� ���������,
            // ���� �� �� �������� � ��������� ���������� �������
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

            // �������� ���������� ���� �� �������� �� ���������,
            // ���� �� �� �������� � ��������� ���������� ������
            if (ActiveInput + 1 > InputsCount_Par)
            {
                Mute_State = ON;
                ActiveInput = 0;
            }

            // �������� ���������� ����� �� �������� �� ���������,
            // ���� �� �� �������� � ��������� ���������� �������
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

/**
  * @brief  ������� ��������� ������� ������� ������ POWER.
  * @retval ���
  */
void LongPressPowerButton_Handler(void)
{
    // ���� ������� ����� NORMAL
    if (System_State == NORMAL)
    {
        // ������������� � ����� SETUP
        System_State = SETUP;
        PowerLED_On();
    }
    // ���� ������� ����� SETUP
    else if (System_State == SETUP)
    {
        // ������������� � ����� NORMAL
        System_State = NORMAL;
        SetupStage_State = SETUP_PAGE1;
        AllowSaveMute_Flag = TRUE;
        PowerLED_Off();
    }
}

/**
  * @brief  ������� ��������� ������� ������� ������ INPUT.
  * @retval ���
  */
void LongPressInputButton_Handler(void)
{
    // ���� ������� ����� NORMAL
    if (System_State == NORMAL)
    {
        (Lock_State == ON) ? (Lock_State = OFF) : (Lock_State = ON); // ����������� ����� ����������
    }
}

/**
  * @brief  ������� ��������� ������� ������� ������ OUTPUT.
  * @retval ���
  */
void LongPressOutputButton_Handler(void)
{
    // ���� ������� ����� NORMAL
    if (System_State == NORMAL)
    {
        (Mute_State == ON) ? (Mute_State = OFF) : (Mute_State = ON); // ����������� ����� MUTE
    }
}

/**
  * @brief  ������� ������ �������� ������� ������.
  * @retval ���
  */
void ShortButtonPresses_Pooling(void)
{
    // ��������� ������� ������ POWER
    if (HAL_GPIO_ReadPin(BTN_PWR_GPIO_Port, BTN_PWR_Pin) == GPIO_PIN_RESET) // ����� ����������� ��� ������� ������
    {
        LongPress_Timer = LONGPRESS_TIME; // �������������� ������ ������� �������
        CountdownLongPress_Task = TRUE;   // ������ ������ ������� ������� ������� �������
        (PowerButton_State == OPEN) ? (PowerButton_State = CLOSE) : (PowerButton_State = OPEN);
    }
    else if (HAL_GPIO_ReadPin(BTN_PWR_GPIO_Port, BTN_PWR_Pin) == GPIO_PIN_SET) // ����� ����������� ��� ���������� ������
    {
        if (PowerButton_State == CLOSE)
        {
            // ������������� �������� ������� ������ POWER
            SerialInfoOutput_PrintButtonEvent();
            CountdownLongPress_Task = FALSE; // ������� ������ ������� ������� ������� �������
            PowerButton_State = OPEN;
            ShortPressPowerButton_Handler();
            SaveSettingsToFlash(&SaveStartAddr); // ��������� ������
        }
    }

    // ��������� ������� ������ INPUT
    if (HAL_GPIO_ReadPin(BTN_IN_GPIO_Port, BTN_IN_Pin) == GPIO_PIN_RESET) // ����� ����������� ��� ������� ������
    {
        LongPress_Timer = LONGPRESS_TIME; // �������������� ������ ������� �������
        CountdownLongPress_Task = TRUE;   // ������ ������ ������� ������� ������� �������
        (InputButton_State == OPEN) ? (InputButton_State = CLOSE) : (InputButton_State = OPEN);
    }
    else if (HAL_GPIO_ReadPin(BTN_IN_GPIO_Port, BTN_IN_Pin) == GPIO_PIN_SET) // ����� ����������� ��� ���������� ������
    {
        if (InputButton_State == CLOSE)
        {
            // ������������� �������� ������� ������ INPUT
            SerialInfoOutput_PrintButtonEvent();
            CountdownLongPress_Task = FALSE; // ������� ������ ������� ������� ������� �������
            InputButton_State = OPEN;
            ShortPressInputButton_Handler();
            SaveSettingsToFlash(&SaveStartAddr); // ��������� ������
        }
    }

    // ��������� ������� ������ OUTPUT
    if (HAL_GPIO_ReadPin(BTN_OUT_GPIO_Port, BTN_OUT_Pin) == GPIO_PIN_RESET) // ����� ����������� ��� ������� ������
    {
        LongPress_Timer = LONGPRESS_TIME; // �������������� ������ ������� �������
        CountdownLongPress_Task = TRUE;   // ������ ������ ������� ������� ������� �������
        (OutputButton_State == OPEN) ? (OutputButton_State = CLOSE) : (OutputButton_State = OPEN);
    }
    else if (HAL_GPIO_ReadPin(BTN_OUT_GPIO_Port, BTN_OUT_Pin) == GPIO_PIN_SET) // ����� ����������� ��� ���������� ������
    {
        if (OutputButton_State == CLOSE)
        {
            // ������������� �������� ������� ������ OUTPUT
            SerialInfoOutput_PrintButtonEvent();
            CountdownLongPress_Task = FALSE; // ������� ������ ������� ������� ������� �������
            OutputButton_State = OPEN;
            ShortPressOutputButton_Handler();
            SaveSettingsToFlash(&SaveStartAddr); // ��������� ������
        }
    }

    DisplayUpdate_Task = TRUE; // ������ ���� ������ ���������� �������
}

/**
   * @brief  ������� ������ ������� ������� ������.
   * @retval ���
   */
void LongButtonPresses_Pooling(void)
{
    if ((PowerButton_State == CLOSE))
    {
        // ������������� ������� ������� ������ POWER
        PowerButton_State = OPEN;
        LongPressPowerButton_Handler();
    }

    else if ((InputButton_State == CLOSE))
    {
        // ������������� ������� ������� ������ INPUT
        InputButton_State = OPEN;
        LongPressInputButton_Handler();
    }

    else if ((OutputButton_State == CLOSE))
    {
        // ������������� ������� ������� ������ OUTPUT
        OutputButton_State = OPEN;
        LongPressOutputButton_Handler();
    }

    SaveSettingsToFlash(&SaveStartAddr); // ��������� ������
    DisplayUpdate_Task = TRUE; // ������ ���� ������ ���������� �������
}