#ifndef _BUTTONS_H
#define _BUTTONS_H

#include "main.h"
#include "font.h"
#include <string.h>
#include "disp1color.h"
#include "usart.h"
#include "display.h"

extern uint32_t SaveStartAddr;

void ShortPressPowerButton_Handler(void);
void ShortPressInputButton_Handler(void);
void ShortPressOutputButton_Handler(void);

void LongPressPowerButton_Handler(void);
void LongPressInputButton_Handler(void);
void LongPressOutputButton_Handler(void);

void ShortButtonPresses_Pooling(void);
void LongButtonPresses_Pooling(void);

#endif