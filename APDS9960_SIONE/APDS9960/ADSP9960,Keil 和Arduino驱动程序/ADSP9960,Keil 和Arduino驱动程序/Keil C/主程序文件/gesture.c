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
int isr_flag = 0;
void handleGesture(void); 
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
	PA3_EXTI_Config();
	APDS9960_i2c_Init();
	USART_INIT();
	Uart_Puts("ADSP9960 Gesture TEST.");
	ADSP_9960_Init();
	enableGestureSensor(true);
	Delay(10000);
	isr_flag = 0;
  while (1)
  {
		if(isr_flag ==1)
  	{
		
			handleGesture();
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0)
			{
				ADSP_9960_Init();
				enableGestureSensor(true);
			}
			isr_flag = 0;	
		}
  }
}

void handleGesture() 
	{
    if (isGestureAvailable() ) {
    switch (readGesture() ) {
      case DIR_UP:
			{
        Uart_Puts("UP");
        break;
			}
      case DIR_DOWN:
			{
        Uart_Puts("DOWN");
        break;
			}
      case DIR_LEFT:
			{
        Uart_Puts("LEFT");
        break;
			}
      case DIR_RIGHT:
			{
        Uart_Puts("RIGHT");
        break;
			}
      case DIR_NEAR:
			{
        Uart_Puts("NEAR");
        break;
			}
      case DIR_FAR:
			{
        Uart_Puts("FAR");
        break;
			}
      default:
        Uart_Puts("NONE");
    }
  }
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
