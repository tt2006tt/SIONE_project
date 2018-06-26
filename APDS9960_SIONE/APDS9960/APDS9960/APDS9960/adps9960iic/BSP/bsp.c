#include "bsp.h"

 

void BSP_Init(void)
{
 
 LED_Init( );//初始化LED
 //LCD_GLASS_Init( );//lcd初始化
 
 RTC_Periph_Init( );
 KEY_Init( );
 ADC_Periph_Init( );
 MyUart_Init( );
 
 enableInterrupts();//使能中断  
}