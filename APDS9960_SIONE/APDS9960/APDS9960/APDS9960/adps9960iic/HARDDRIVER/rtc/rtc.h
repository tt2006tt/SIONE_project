#ifndef _RTC_H
#define _RTC_H
#include "stm8l15x.h"



void RTC_Periph_Init(void);
void RTC_restart(unsigned short s);
void EnterHalt(unsigned short s);


#endif