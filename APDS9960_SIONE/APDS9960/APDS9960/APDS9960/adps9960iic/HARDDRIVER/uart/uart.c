#include "uart.h"
#include <stdio.h>

void MyUart_Init(void)
{
  
  GPIO_Init(GPIOC,GPIO_Pin_2,GPIO_Mode_In_PU_No_IT);//PC2 RX 
  GPIO_Init(GPIOC,GPIO_Pin_3,GPIO_Mode_Out_PP_High_Fast);//PC3  TX
  
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1,ENABLE);//使能串口时钟

  USART_Init(USART1,9600, USART_WordLength_8b, USART_StopBits_1,
      USART_Parity_No,(USART_Mode_TypeDef)(USART_Mode_Rx|USART_Mode_Tx));
                                  
  USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);//使能接受中断 
  USART_ITConfig(USART1,USART_IT_TXE, DISABLE);// 
  USART_ITConfig(USART1,USART_IT_TC, DISABLE);
  //USART_ITConfig(USART1,USART_IT_OR, DISABLE);//atention
  USART_ITConfig(USART1,USART_IT_IDLE, DISABLE);
  USART_ITConfig(USART1,USART_IT_ERR, DISABLE);

  USART_Cmd(USART1, ENABLE);                 
}

void UART1_SendString(char buf[])
{
   unsigned int i = 0;
   while(buf[i] != '\0') 
   { 
      
       while(((USART1->SR) & 0x80) == 0x00);
          USART_SendData8(USART1,buf[i++]); 
   }
}

void UART1_SendNumber(int num)
{
  char buf[20];//顶一个局部缓冲区
  
  sprintf((char *)buf,"%d\n",num);
  
  UART1_SendString(buf);
}
 
//重定向
int fputc(int ch, FILE *f)
{
  while(((USART1->SR) & 0x80) == 0x00);
          USART_SendData8(USART1,ch);
    return ch;
}

