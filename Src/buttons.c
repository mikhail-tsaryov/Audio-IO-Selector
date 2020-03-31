#include "buttons.h"

/**
  * @brief  Функция обработки короткого нажатия кнопки POWER.
  * @retval Нет
  */
void ShortPressPowerButton_Handler(void)
{
    // Если текущий режим STANDBY, переключиться в режим NORMAL
    if (System_State == STANDBY)
    {
        System_State = NORMAL; // Сменить режим работы
        LoadSettingsFromFlash(&SaveStartAddr);
        
        #ifdef INFO_OUTPUT
            SerialInfoOutput_PrintLoadedParameters();
        #endif
        AllowSaveMute_Flag = TRUE;
        RelaysModuleUpdate_Task = TRUE;

        disp1color_Wake();
        // Приветствие
        disp1color_FillScreenbuff(0);
        //disp1color_DrawRectangle(0, 0, DISP1COLOR_Width - 1, DISP1COLOR_Height - 1);
 
        #ifndef RUS_LANG
        disp1color_printfCenterAlign(0, 8, FONTID_10X16F, "Audio");
        disp1color_printfCenterAlign(0, 24, FONTID_10X16F, "I/O Selector");
        disp1color_printfCenterAlign(0, 50, FONTID_6X8M, "%c M.Tsaryov, %d.3", 0xA9, 2020);
        #else
        // TODO: Решить проблему с неверной кодировкой. Переделать шрифты???
        disp1color_printfCenterAlign(0, 8, FONTID_10X16F, "Аудио");
        disp1color_printfCenterAlign(0, 24, FONTID_10X16F, "коммутатор");
        disp1color_printfCenterAlign(0, 50, FONTID_6X8M, "%c М.Царёв, %d.3", 0xA9, 2020);
        #endif
        disp1color_UpdateFromBuff();
        
        HAL_Delay(WELCOME_TIME);

        PowerLED_Off(); // Отключить светодиод питания
    }
    // Если текущий режим NORMAL, переключиться в дежурный режим
    else if (System_State == NORMAL)
    {
        System_State = STANDBY; // Сменить режим работы
        // Погасить экран
        disp1color_Sleep();
        // Отключить выходы и обновить состояние релейного модуля, не сохраняя режим MUTE
        AllowSaveMute_Flag = FALSE;
        Mute_State = ON;
        RelaysModuleUpdate_Task = TRUE;
        PowerLED_On(); // Включить светодиод питания
        #ifdef INFO_OUTPUT
            SerialInfoOutput_PrintGoodbyeInfo();
        #endif
    }
    // Если текущий режим SETUP
    else if (System_State == SETUP)
    {
        // Если сейчас 1 этап настройки, переключиться на 2 этап
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
            SetupStage_State = SETUP_PAGE1;
        }
    }
}

/**
  * @brief  Функция обработки короткого нажатия кнопки INPUT.
  * @retval Нет
  */
void ShortPressInputButton_Handler(void)
{
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
        }
    }
    // Если текущий режим SETUP
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

/**
  * @brief  Функция обработки короткого нажатия кнопки OUTPUT.
  * @retval Нет
  */
void ShortPressOutputButton_Handler(void)
{
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
        }
    }
    // Если текущий режим SETUP
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

/**
  * @brief  Функция обработки долгого нажатия кнопки POWER.
  * @retval Нет
  */
void LongPressPowerButton_Handler(void)
{
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
        SetupStage_State = SETUP_PAGE1;
        AllowSaveMute_Flag = TRUE;
        PowerLED_Off();
    }
}

/**
  * @brief  Функция обработки долгого нажатия кнопки INPUT.
  * @retval Нет
  */
void LongPressInputButton_Handler(void)
{
    // Если текущий режим NORMAL
    if (System_State == NORMAL)
    {
        (Lock_State == ON) ? (Lock_State = OFF) : (Lock_State = ON); // Переключить режим блокировки
    }
}

/**
  * @brief  Функция обработки долгого нажатия кнопки OUTPUT.
  * @retval Нет
  */
void LongPressOutputButton_Handler(void)
{
    // Если текущий режим NORMAL
    if (System_State == NORMAL)
    {
        (Mute_State == ON) ? (Mute_State = OFF) : (Mute_State = ON); // Переключить режим MUTE
    }
}

/**
  * @brief  Функция опроса коротких нажатий кнопок.
  * @retval Нет
  */
void ShortButtonPresses_Pooling(void)
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
            // Зафиксировано короткое нажатие кнопки POWER
            uint8_t str[] = "* Short press Power Button *\r\n";
            HAL_UART_Transmit(&huart1, str, sizeof(str) - 1, 0xFFFF);

            CountdownLongPress_Task = FALSE; // Снимаем задачу отсчета времени долгого нажатия
            PowerButton_State = OPEN;
            ShortPressPowerButton_Handler();
            SaveSettingsToFlash(&SaveStartAddr); // Сохраняем данные
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
            // Зафиксировано короткое нажатие кнопки INPUT
            uint8_t str[] = "* Short press Input Button *\r\n";
            HAL_UART_Transmit(&huart1, str, sizeof(str) - 1, 0xFFFF);

            CountdownLongPress_Task = FALSE; // Снимаем задачу отсчета времени долгого нажатия
            InputButton_State = OPEN;
            ShortPressInputButton_Handler();
            SaveSettingsToFlash(&SaveStartAddr); // Сохраняем данные
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
            // Зафиксировано короткое нажатие кнопки OUTPUT
            uint8_t str[] = "* Short press Output Button *\r\n";
            HAL_UART_Transmit(&huart1, str, sizeof(str) - 1, 0xFFFF);

            CountdownLongPress_Task = FALSE; // Снимаем задачу отсчета времени долгого нажатия
            OutputButton_State = OPEN;
            ShortPressOutputButton_Handler();
            SaveSettingsToFlash(&SaveStartAddr); // Сохраняем данные
        }
    }

    DisplayUpdate_Task = TRUE; // Ставим флаг задачи обновления дисплея
}

/**
   * @brief  Функция опроса длинных нажатий кнопок.
   * @retval Нет
   */
void LongButtonPresses_Pooling(void)
{
    if ((PowerButton_State == CLOSE))
    {
        // Зафиксировано длинное нажатие кнопки POWER
        uint8_t str[] = "* Long press Power Button *\r\n";
        HAL_UART_Transmit(&huart1, str, sizeof(str) - 1, 0xFFFF);

        PowerButton_State = OPEN;
        LongPressPowerButton_Handler();
        SaveSettingsToFlash(&SaveStartAddr); // Сохраняем данные
    }

    else if ((InputButton_State == CLOSE))
    {
        // Зафиксировано длинное нажатие кнопки INPUT
        uint8_t str[] = "* Long press Input Button *\r\n";
        HAL_UART_Transmit(&huart1, str, sizeof(str) - 1, 0xFFFF);

        InputButton_State = OPEN;
        LongPressInputButton_Handler();
        SaveSettingsToFlash(&SaveStartAddr); // Сохраняем данные
    }

    else if ((OutputButton_State == CLOSE))
    {
        // Зафиксировано длинное нажатие кнопки OUTPUT
        uint8_t str[] = "* Long press Output Button * \r\n";
        HAL_UART_Transmit(&huart1, str, sizeof(str) - 1, 0xFFFF);

        OutputButton_State = OPEN;
        LongPressOutputButton_Handler();
        SaveSettingsToFlash(&SaveStartAddr); // Сохраняем данные
    }

    DisplayUpdate_Task = TRUE; // Ставим флаг задачи обновления дисплея
}