#include "Led.h"


void LED_Init(void)
{
   
  GPIO_Init(GPIOC,GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Fast);
}