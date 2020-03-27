//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// ��� ����������� ����������� ���������������� ��������. �� ������ ���������
// ��� �� ����� �����, �� �� �������� ������� ������ �� ��� YouTube-����� 
// "����������� � ���������" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// �����: �������� ������ / Nadyrshin Ruslan
//------------------------------------------------------------------------------

#include "spi.h"
#include <ssd1306.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

//==============================================================================

// ������ ��������������� ������ ��������� � ������ ����� ssd1306
#define SSD1306_Adressing_Horizontal            0       // ������� ��������� �� �����������, ����� ��������� �� ���������
#define SSD1306_Adressing_Vertical              1       // ������� ��������� �� ���������, ����� ��������� �� �����������
#define SSD1306_Adressing_Page                  2       // ��������� ������ �� �����������

//==============================================================================
// ���� ������ �������
//==============================================================================
// ������� �� ������� Fundamental
#define CMD_SetContrast                 0x81
#define CMD_AllPixRAM                   0xA4
#define CMD_AllPixOn                    0xA5
#define CMD_SetInverseOff               0xA6
#define CMD_SetInverseOn                0xA7
#define CMD_Sleep                       0xAE
#define CMD_Wake                        0xAF
#define CMD_Lock                        0xFD
// ������� �� ������� Scrolling
#define CMD_DeactivateScroll            0x2E
// ������� �� ������� Addressing Setting
// ������� ������ ������ ���������� ��������� � ������ �����
#define CMD_SetMemAdressingMode         0x20    
// ������� ������ ��������� ��������� �������� � ������� ��� ���������� ��������� � ������ �����
// ����������� ��� ������� ���������� SSD1306_Adressing_Horizontal � SSD1306_Adressing_Vertical
#define CMD_SetColumnAddr               0x21
#define CMD_SetPageAddr                 0x22
// ������� ������ �������� � ��������� ��������� ������� ��� ���������� ��������� � ������ �����
// ����������� ��� ������ ���������� SSD1306_Adressing_Page
#define CMD_PageAddrMode_SetPage        0xB0
#define CMD_PageAddrMode_StartColumnLo  0x00
#define CMD_PageAddrMode_StartColumnHi  0x10

// ������� �� ������� Hardware Configuration
#define CMD_SetDisplayStartLine         0x40
#define CMD_SetSegmentRemap             0xA0
#define CMD_SetMultiplexRatio           0xA8
#define CMD_SetCOMoutScanDirection      0xC0 
#define CMD_SetDisplayOffset            0xD3
#define CMD_SetCOMPinsConfig            0xDA
  
// ������� �� ������� Timing & Driving Scheme Setting
#define CMD_SetDisplayClockDivider      0xD5
#define CMD_SetPrechargePeriod          0xD9
#define CMD_SetVCOMHDeselectLevel       0xDB

// ������� �� ������� Charge Pump
#define CMD_ChargePumpSetting           0x8D
//==============================================================================

// ������� ������� � ��������
static uint8_t Height;
static uint8_t Width;


//==============================================================================
// ��������� ������� ������� � ����������� � ssd1306
//==============================================================================
static void SendCommand(uint8_t Cmd, uint8_t *pBuff, uint16_t BuffLen)
{
    HAL_GPIO_WritePin(SPI_nCS_OLED_GPIO_Port, SPI_nCS_OLED_Pin, GPIO_PIN_RESET); // select OLED
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);           // command
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&Cmd, 1, HAL_MAX_DELAY);

    for (uint16_t i = 0; i < BuffLen; i++)
    {
        HAL_SPI_Transmit(&hspi1, pBuff, 1, HAL_MAX_DELAY);
        pBuff++;
    }

    HAL_GPIO_WritePin(SPI_nCS_OLED_GPIO_Port, SPI_nCS_OLED_Pin, GPIO_PIN_SET); // un-select OLED
}
//==============================================================================


//==============================================================================
// ��������� ������� ������ ������ � ssd1306
//==============================================================================
static void SendData(uint8_t *pBuff, uint16_t BuffLen)
{
    HAL_GPIO_WritePin(SPI_nCS_OLED_GPIO_Port, SPI_nCS_OLED_Pin, GPIO_PIN_RESET); // select OLED
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);             // data

    for (uint16_t i = 0; i < BuffLen; i++)
    {
        HAL_SPI_Transmit(&hspi1, pBuff, 1, HAL_MAX_DELAY);
        pBuff++;
    }

    HAL_GPIO_WritePin(SPI_nCS_OLED_GPIO_Port, SPI_nCS_OLED_Pin, GPIO_PIN_SET); // un-select OLED
}
//==============================================================================


//==============================================================================
// ��������� ������� � ������� ����� ����� �� ������� pBuff
//==============================================================================
void SSD1306_DisplayFullUpdate(uint8_t *pBuff, uint16_t BuffLen)
{
    SSD1306_SetColumns(0, Width - 1);
    SSD1306_SetPages(0, (Height >> 3) - 1);
    SendData(pBuff, BuffLen);
}
//==============================================================================


//==============================================================================
// ��������� ������������� ����� ���������� ��������� � ������ ����� ssd1306
//==============================================================================
void SSD1306_SetMemAdressingMode(uint8_t Mode)
{
  if (Mode > 2)
    return;

  SendCommand(CMD_SetMemAdressingMode, (uint8_t *)Mode, 1);
}
//==============================================================================


//==============================================================================
// ��������� ������������� ��������� � �������� ������ ������� 
// ��� ������������ ��������� � ������ ����� ��� ������ ������.
//==============================================================================
void SSD1306_SetColumns(uint8_t Start, uint8_t End)
{
  Start &= 0x7F;
  End &= 0x7F;
  uint8_t Buff[] = {Start, End};
  SendCommand(CMD_SetColumnAddr, Buff, 2);
}
//==============================================================================


//==============================================================================
// ��������� ������������� ��������� � �������� ������ �������� 
// ��� ������������ ��������� � ������ ����� ��� ������ ������.
//==============================================================================
void SSD1306_SetPages(uint8_t Start, uint8_t End)
{
  Start &= 0x07;
  End &= 0x07;
  uint8_t Buff[] = {Start, End};
  SendCommand(CMD_SetPageAddr, Buff, 2);
}
//==============================================================================


//==============================================================================
// ��������� ������������� �������� ��� ������ ���������� �����������, �����
// ����� ��������� � ������ �������� ������ �� ����������� (SSD1306_Adressing_Page).
//==============================================================================
void SSD1306_PageAddrMode_SetPage(uint8_t Page)
{
  Page &= 0x07;
  SendCommand(CMD_PageAddrMode_SetPage | Page, 0, 0);
}
//==============================================================================


//==============================================================================
// ��������� ������������� � ������, �� ������� ��������� � ������ ����� ����� ������������
// ������ ��� ������ ���������� ��������� (SSD1306_Adressing_Page)
//==============================================================================
void SSD1306_PageAddrMode_StartColumn(uint8_t Start)
{
  Start &= 0x7F;
  SendCommand(CMD_PageAddrMode_StartColumnLo | (Start & 0x07), 0, 0);
  SendCommand(CMD_PageAddrMode_StartColumnHi | (Start >> 4), 0, 0);
}
//==============================================================================


//==============================================================================
// Set display RAM display start line register from 0-63
//==============================================================================
void SSD1306_SetDisplayStartLine(uint8_t Line)
{
  Line &= 0x3F;
  SendCommand(CMD_SetDisplayStartLine | Line, 0, 0);
}
//==============================================================================


//==============================================================================
// Value=0: column address 0 is mapped to SEG0 (RESET)
// Value=1: column address 127 is mapped to SEG0 
//==============================================================================
void SSD1306_SetSegmentRemap(uint8_t Value)
{
  Value = Value ? 0x01 : 0x00;
  SendCommand(CMD_SetSegmentRemap | Value, 0, 0);
}
//==============================================================================


//==============================================================================
void SSD1306_SetMultiplexRatio(uint8_t Mux)
{
  Mux--;
  Mux &= 0x3F;
  SendCommand(CMD_SetMultiplexRatio, &Mux, 1);
}
//==============================================================================


//==============================================================================
// �������� �� ��������� ������� ����������� ������������ ������ �����
//==============================================================================
void SSD1306_SetDisplayOffset(uint8_t Offset)
{
  SendCommand(CMD_SetDisplayOffset, &Offset, 1);
}
//==============================================================================


//==============================================================================
void SSD1306_SetDisplayClockDivider(uint8_t DCLKdiv, uint8_t Fosc)
{
  DCLKdiv--;
  DCLKdiv &= 0x0F;
  DCLKdiv |= ((Fosc & 0x0F) << 4);
  SendCommand(CMD_SetDisplayClockDivider, &DCLKdiv, 1);
}
//==============================================================================


//==============================================================================
void SSD1306_ChargePumpSetting(uint8_t Value)
{
  Value = Value ? 0x14 : 0x10;
  SendCommand(CMD_ChargePumpSetting, &Value, 1);
}
//==============================================================================


//==============================================================================
// ����������� ������������
// Value=0: normal mode (RESET) Scan from COM0 to COM[N �1]
// Value=1: remapped mode. Scan from COM[N-1] to COM0
// Where N is the Multiplex ratio. 
//==============================================================================
void SSD1306_SetCOMoutScanDirection(uint8_t Value)
{
  Value = Value ? 0x08 : 0x00;
  SendCommand(CMD_SetCOMoutScanDirection | Value, 0, 0);
}
//==============================================================================


//==============================================================================
// AltCOMpinConfig=0: Sequential COM pin configuration
// AltCOMpinConfig=1(RESET): Alternative COM pinconfiguration
// LeftRightRemap=0(RESET): Disable COM Left/Right remap
// LeftRightRemap=1: Enable COM Left/Right remap 
//==============================================================================
void SSD1306_SetCOMPinsConfig(uint8_t AltCOMpinConfig, uint8_t LeftRightRemap)
{
  uint8_t tmpValue = (1 << 1);
  
  if (AltCOMpinConfig)
    tmpValue |= (1 << 4);
  
  if (LeftRightRemap)
    tmpValue |= (1 << 5);
  
  SendCommand(CMD_SetCOMPinsConfig, &tmpValue, 1);
}
//==============================================================================


//==============================================================================
void SSD1306_SetPrechargePeriod(uint8_t Phase1period, uint8_t Phase2period)
{
  Phase1period &= 0x0F;
  Phase1period &= 0x0F;
  
  if (!Phase1period)
    Phase1period = 2;
  
  if (!Phase2period)
    Phase2period = 2;
  
  Phase1period |= (Phase2period << 4);
  
  SendCommand(CMD_SetPrechargePeriod, &Phase1period, 1);
}
//==============================================================================


//==============================================================================
void SSD1306_SetVCOMHDeselectLevel(uint8_t Code)
{
  Code &= 0x70;
  SendCommand(CMD_SetVCOMHDeselectLevel, &Code, 1);
}
//==============================================================================


//==============================================================================
// ��������� ��������� ������ (���������� ����������� �������
//==============================================================================
void SSD1306_DeactivateScroll(void)
{
  SendCommand(CMD_DeactivateScroll, 0, 0);
}
//==============================================================================


//==============================================================================
// ��������� ��������� ������� � ����� ���
//==============================================================================
void SSD1306_Sleep(void)
{
  SendCommand(CMD_Sleep, 0, 0);
}
//==============================================================================


//==============================================================================
// ��������� ������� ������� �� ������ ���
//==============================================================================
void SSD1306_Wake(void)
{
  SendCommand(CMD_Wake, 0, 0);
}
//==============================================================================


//==============================================================================
// ��������� �������� ��� ������� ������� (���� ����������)
//==============================================================================
void SSD1306_AllPixOn(void)
{
  SendCommand(CMD_AllPixOn, 0, 0);
}
//==============================================================================


//==============================================================================
// ��������� ��������� ���� ������� � ������� �� ���� �������� �� ������ ����� � ssd1306
//==============================================================================
void SSD1306_AllPixRAM(void)
{
  SendCommand(CMD_AllPixRAM, 0, 0);
}
//==============================================================================


//==============================================================================
// ��������� �������� �������� �������
//==============================================================================
void SSD1306_SetInverseOn(void)
{
  SendCommand(CMD_SetInverseOn, 0, 0);
}
//==============================================================================


//==============================================================================
// ��������� ��������� �������� �������
//==============================================================================
void SSD1306_SetInverseOff(void)
{
  SendCommand(CMD_SetInverseOff, 0, 0);
}
//==============================================================================


//==============================================================================
// ��������� ������������� �������� ������������� (0-255)
//==============================================================================
void SSD1306_SetContrast(uint8_t Value)
{
  SendCommand(CMD_SetContrast, &Value, 1);
}
//==============================================================================


//==============================================================================
// ��������� ������������� ������� �� ����������� ssd1306
//==============================================================================
void SSD1306_Init(uint8_t width, uint8_t height)
{
  Width = width;
  Height = height;
   
  // ����� ����������� ������� ssd1306 ������ Reset
  HAL_GPIO_WritePin(SPI_nCS_OLED_GPIO_Port, SPI_nCS_OLED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET);
  HAL_Delay(10);

  // ��� ������� ������������� ssd1306
  SSD1306_Sleep();
  SSD1306_SetDisplayClockDivider(1, 8);
  SSD1306_SetMultiplexRatio(Height);
  SSD1306_SetDisplayOffset(0);
  SSD1306_SetDisplayStartLine(0);
  SSD1306_ChargePumpSetting(1);
  SSD1306_SetMemAdressingMode(SSD1306_Adressing_Vertical);
  SSD1306_SetSegmentRemap(1);        // *������ ����������� ���������� ������� �� ������ ����� (���������/�����������)
  SSD1306_SetCOMoutScanDirection(1); // *�������������� ������������ �� ������� (������ �� ���������)

  if ((Width == 128) && (Height == 32))
      SSD1306_SetCOMPinsConfig(0, 0);
  else if ((Width == 128) && (Height == 64))
      SSD1306_SetCOMPinsConfig(1, 0);
  else if ((Width == 96) && (Height == 16))
      SSD1306_SetCOMPinsConfig(0, 0);

  SSD1306_SetContrast(255);
  SSD1306_SetPrechargePeriod(2, 2);
  SSD1306_SetVCOMHDeselectLevel(0x40);
  SSD1306_AllPixRAM();
  SSD1306_SetInverseOff();
  SSD1306_DeactivateScroll();
  SSD1306_Wake();
}
//==============================================================================
