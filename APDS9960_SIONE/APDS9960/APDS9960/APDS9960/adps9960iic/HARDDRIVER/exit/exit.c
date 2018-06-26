#include "exit.h"

#define KEY_GPIO_PORT              GPIOC
#define KEY_GPIO_PIN               GPIO_Pin_1


void KEY_Init(void)
{
   GPIO_Init(KEY_GPIO_PORT, KEY_GPIO_PIN, GPIO_Mode_In_FL_IT);/*!< Input floating, external interrupt */
   
   EXTI_SetPinSensitivity(EXTI_Pin_1,EXTI_Trigger_Falling);//
}