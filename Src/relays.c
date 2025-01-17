#include "relays.h"
#include "spi.h"

/**
  * @brief  ������� ������������ � �������� ������ �� �������� ������.
  * @retval ���
  */
void RelaysModule_Update(void)
{
    uint32_t Frame = 0;

    // ������������ ������ ��� ��������
    // ���� �������� ������� "�����/������"
    if (InOutOrder_Par == IN_OUT)
    {
        Frame = ((1 << (ActiveInput + OutputsCount_Par)) | (1 << ActiveOutput)) * ((!Mute_State) & 1);
        // ���������� mute ������������� � ������������� � ������, ����� �������� ������� ���
        // ��� ����� ��� �������� MUTE Frame ������ ������ ����
    }
    // ���� �������� ������� "������/�����"
    else if (InOutOrder_Par == OUT_IN)
    {
        Frame = ((1 << (ActiveOutput + InputsCount_Par)) | (1 << ActiveInput)) * ((!Mute_State) & 1);
    }

    // �������� �������� ������
    RelaysModule_Reset();

    // ��������� ����� ������ �� SPI
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&Frame, ModulesCount_Par, 4);
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
void RelaysModule_Reset(void)
{
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET);

    // ������������� ������� ��� �������� ������ ����� ���������� TPIC6B595
    HAL_GPIO_WritePin(SPI_nCS_REL_GPIO_Port, SPI_nCS_REL_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SPI_nCS_REL_GPIO_Port, SPI_nCS_REL_Pin, GPIO_PIN_SET);

    // ��������� ��������, ����� ���� ������ �����������
    HAL_Delay(5);
}