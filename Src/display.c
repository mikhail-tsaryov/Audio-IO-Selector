#include "display.h"

/**
  * @brief  ������� ���������� OLED-�������.
  * @retval ���
  */
void Display_Update(void)
{
    disp1color_FillScreenbuff(BLACK);
    
    // ���� ������� ����� NORMAL
    if (System_State == NORMAL)
    {
        // ���� ������� ����� NORMAL � ����������� ����� ��������
        if (DisplayWelcome_Task == TRUE)
        {
            //disp1color_DrawRectangle(0, 0, DISP1COLOR_Width - 1, DISP1COLOR_Height - 1); // �����
            #ifndef RUS_LANG
                disp1color_printfCenterAlign(0, 8, FONTID_10X16F, "Audio");
                disp1color_printfCenterAlign(0, 24, FONTID_10X16F, "I/O Selector");
                disp1color_printfCenterAlign(0, 50, FONTID_6X8M, "%c M.Tsaryov, %d.3", 0xA9, 2020);
            #else
                // TODO: ������ �������� � �������� ����������. ���������� ������???
                disp1color_printfCenterAlign(0, 8, FONTID_10X16F, "�����");
                disp1color_printfCenterAlign(0, 24, FONTID_10X16F, "����������");
                disp1color_printfCenterAlign(0, 50, FONTID_6X8M, "%c �.����, %d.3", 0xA9, 2020);
            #endif
        }
        else
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
                disp1color_DrawArrow(88, 31); // �������
            }
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

        /*
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
        */
    }
    
    disp1color_UpdateFromBuff(); // ������� ����� ����� �� �������
}

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