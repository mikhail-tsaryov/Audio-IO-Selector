#ifndef _INFO_OUTPUT_H
#define _INFO_OUTPUT_H

#include "main.h"
#include <string.h>
#include "usart.h"
#include "ext_flash.h"

void SerialInfoOutput_PrintWelcomeInfo(void);
void SerialInfoOutput_PrintGoodbyeInfo(void);
void SerialInfoOutput_PrintSavedParameters(uint32_t *pAddr);
void SerialInfoOutput_PrintLoadedParameters(void);

#endif