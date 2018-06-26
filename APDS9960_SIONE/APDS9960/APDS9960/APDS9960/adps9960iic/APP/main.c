#include "stm8l15x.h"
#include "ADPS9960.h"
#include <stdio.h>
#include <string.h>
#include "tm1639.h"
/*******************************************************************************
**�������ƣ�void delay(unsigned int ms)     Name: void delay(unsigned int ms)
**���������������ʱ
**��ڲ�����unsigned int ms   ��������ʱ��ֵ
**�������
*******************************************************************************/
int i = 0;
int y = 0;
extern uint16_t Cnt_time;
extern uint8_t led_sta;
void InitTimer2(void){


  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE);//����TIM2

  TIM2_TimeBaseInit(TIM2_Prescaler_1,TIM2_CounterMode_Up,(16000 * 2));//����Ƶ 2����һ�ν����ж�
  TIM2_ClearFlag(TIM2_FLAG_Update);
  TIM2_ARRPreloadConfig(ENABLE);
  TIM2_ITConfig(TIM2_IT_Update, ENABLE);//ʹ�����ϼ�������ж�
  TIM2_Cmd(ENABLE);  
}



void Usart1Init(void)
{	//SYSCFG_REMAPDeInit();
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1,ENABLE);
    //GPIO_Init(GPIOC, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Slow);//TXD
   // GPIO_Init(GPIOC, GPIO_Pin_6, GPIO_Mode_In_PU_No_IT);//RXD
    USART_DeInit(USART1);       //��λUART1 
    /*uart1 ������9600��8bit(����)������rxd*/
    USART_Init(USART1,(uint32_t)4800,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,(USART_Mode_Rx|USART_Mode_Tx) );
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    
    USART_Cmd(USART1,ENABLE);
   
}

int fputc(int ch, FILE *f)
{      
    while(USART_GetFlagStatus(USART1 , USART_FLAG_TXE) == 0);        //�жϷ������ݼĴ����Ƿ�Ϊ��   
    USART_SendData8(USART1 , (unsigned char)ch);                     //���ͼĴ���д������      
    return ch;
}

uint8_t proximity_data = 0;
uint8_t proximity_data_count = 0;
uint16_t ambient_light = 0;
void main()
{

  unsigned char x[]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//�ڲ�����Ƶ
  
  
  CLK_ClockSecuritySystemEnable();
  CLK_HSICmd(ENABLE);
  //Soft_I2C_Int();
  InitTimer2();
  //CLK_PeripheralClockConfig (CLK_Peripheral_I2C1,ENABLE);//����IIC1ʱ��
  //I2C_DeInit(I2C1);
  //I2C_Init(I2C1,I2C_MAX_FAST_FREQ, 0x39, I2C_Mode_I2C,I2C_DutyCycle_2, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);
  //I2C_Cmd(I2C1,ENABLE);
  enableInterrupts();//�����ж�
  WWDG->WR = WWDG_WR_RESET_VALUE;
  WWDG->CR = (uint8_t)((uint8_t)(WWDG_CR_WDGA | WWDG_CR_T6) | (uint8_t)0x7E);
  WWDG->WR = (uint8_t)((uint8_t)(~WWDG_CR_WDGA) & (uint8_t)(WWDG_CR_T6 | 0x7F));
  Usart1Init();
  Tm1639_Init();
  SeriesWData(0xc0,x,8,DISPLAY_LEVEL14);
  printf("ADSP9960 Gesture TEST.");
  APDS9960init();
  setProximityGain(PGAIN_2X);
  enableProximitySensor(FALSE);
  enableLightSensor(FALSE);
  delay(800);
  while(1)
{   
	readProximity(&proximity_data);
	//printf("%d\r\n",proximity_data);
	if(proximity_data > 40)
	{
		proximity_data_count++;
		if(proximity_data_count >= 20){
			SeriesWData(0xc0,x,8,DISPLAY_LEVEL14);
			proximity_data_count = 0;
			Cnt_time = 0;
			led_sta = 0;
		}
	}
	if(led_sta){
		SeriesWData(0xc0,x,8,DISPLAY_OFF);
	}
	delay(50);
	/*
    readAmbientLight(&ambient_light);
    printf("Ambient: %d\r\n",ambient_light);			
    

    
    delay(250);
    */
  }    
  
}
