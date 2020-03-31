#include "relays.h"
#include "spi.h"

/**
  * @brief  Функция формирования и отправки данных на релейный модуль.
  * @retval Нет
  */
void RelaysModule_Update(uint8_t Input, uint8_t Output, uint8_t Mute)
{
    uint32_t Frame = 0;

    // Сформировать данные для отправки
    // Если настроен порядок "входы/выходы"
    if (InOutOrder_Par == IN_OUT)
    {
        Frame = ((1 << (Input + OutputsCount_Par)) | (1 << Output)) * ((!Mute) & 1);
        // Переменная mute инвертируется и перемножается с маской, чтобы выделить младший бит
        // Тем самым при активном MUTE Frame станет равным нулю
    }
    // Если настроен порядок "выходы/входы"
    else if (InOutOrder_Par == OUT_IN)
    {
        Frame = ((1 << (Output + InputsCount_Par)) | (1 << Input)) * ((!Mute) & 1);
    }

    // Сбросить релейный модуль
    RelaysModule_Reset();

    // Отправить новые данные по SPI
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&Frame, ModulesCount_Par, 4);
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
void RelaysModule_Reset(void)
{
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET);

    // Сгенерировать импульс для переноса данных между регистрами TPIC6B595
    HAL_GPIO_WritePin(SPI_nCS_REL_GPIO_Port, SPI_nCS_REL_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SPI_nCS_REL_GPIO_Port, SPI_nCS_REL_Pin, GPIO_PIN_SET);

    // Небольшая задержка, чтобы реле успели выключиться
    HAL_Delay(5);
}