#ifndef _UART_DEBUG_H
#define _UART_DEBUG_H

#include "main.h"
#include <string.h>
#include "usart.h"
#include "ext_flash.h"

void SerialInfoOutput_PrintWelcomeInfo(void);
void SerialInfoOutput_PrintSavedParameters(uint32_t *pAddr);
void SerialInfoOutput_PrintLoadedParameters(void);

#endif