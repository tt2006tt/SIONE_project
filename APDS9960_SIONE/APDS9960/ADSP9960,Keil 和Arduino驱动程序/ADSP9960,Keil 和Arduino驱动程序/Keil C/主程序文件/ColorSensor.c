/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    22-November-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/*******************************************/

#define LIGHT_INT_HIGH  1000 // High light level for interrupt
#define LIGHT_INT_LOW   20   // Low light level for interrupt

uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;
int isr_flag = 0;

void print_digital(uint number);
void print_crlf(void);
/*******************************************/

void PA3_EXTI_Config(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
  EXTI_InitTypeDef   EXTI_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;	

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource3);	
	
	// Configure EXTI3 line 
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
  /* Enable and set EXTI3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x05;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);		
}

int main(void)
{
	//PA3_EXTI_Config();
	APDS9960_i2c_Init();
	USART_INIT();
	Uart_Puts("ADSP9960 ColorSensor TEST.");
	ADSP_9960_Init();
	
  enableLightSensor(false);
	ADSP_Delay(500);
	
  while (1)
  {
		  readAmbientLight(&ambient_light);
			print_crlf();
			Uart_Puts("Ambient: ");
			print_digital(ambient_light);			
			
			readRedLight(&red_light);
			print_crlf();
			Uart_Puts(" R: ");
			print_digital(red_light);	
			
			readGreenLight(&green_light);
			print_crlf();
			Uart_Puts(" G: ");
			print_digital(green_light);	
			
			readBlueLight(&blue_light);
			print_crlf();
			Uart_Puts(" B: ");
			print_digital(blue_light);
		
			ADSP_Delay(1000);
			
		  
  }
}
/***************************************************************************************/
void print_digital(uint number)
{
  uchar temp_buff[5];
  uint temp;

  temp=number/10000;
  if(temp==0) temp_buff[0]=' ';
  else        temp_buff[0]=temp+0x30;

  temp=number%10000;
  temp=temp/1000;
  if(temp==0)
  {
  	if(temp_buff[0]==' ') temp_buff[1]=' ';
	else temp_buff[1]='0';
  }
  else        temp_buff[1]=temp+0x30;

  temp=number%1000;
  temp=temp/100;
  if(temp==0)
  {
  	if(temp_buff[1]==' ') temp_buff[2]=' ';
	else temp_buff[2]='0';
  }
  else        temp_buff[2]=temp+0x30;

  temp=number%100;
  temp=temp/10;
  if(temp==0)
  {
  	if(temp_buff[2]==' ') temp_buff[3]=' ';
	else temp_buff[3]='0';
  }
  else        temp_buff[3]=temp+0x30;

  temp=temp%10;
  temp_buff[4]=temp+0x30;

  Uart_Putc(temp_buff[0]);
  Uart_Putc(temp_buff[1]);
  Uart_Putc(temp_buff[2]);
  Uart_Putc(temp_buff[3]);
  Uart_Putc(temp_buff[4]);
}
/***************************************************************************************/
void print_crlf()
{
  Uart_Putc(0x0d);
  Uart_Putc(0x0a);	
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */
/********************************************************************************/



/********************************************************************************/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
