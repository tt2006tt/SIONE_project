#ifndef _LED_H
#define _LED_H
#include "stm8l15x.h"

#define LED1_ON()   GPIO_ResetBits(GPIOC,GPIO_Pin_7)
#define LED1_OFF()  GPIO_SetBits(GPIOC,GPIO_Pin_7)



//declaration
void LED_Init(void);
#endif