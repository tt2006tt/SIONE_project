#include "main.h"

/************************************************/
void USART_INIT(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef 	NVIC_InitStructure;
	
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	
		RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE );						
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);     											 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;                 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);   

		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure);
		
		USART_Cmd(USART1, ENABLE);
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
}
/************************************************/
void Uart_Putc(unsigned char c)
{
	while(!((USART1->ISR)&(1<<7)));
	USART1->TDR=c;	
}
/************************************************/
void Uart_Puts(char *s )
{
	while (*s)
	Uart_Putc(*s++);
}
/************************************************/
void Uart_String_CRLF(char *s )
{
	while (*s)
	Uart_Putc(*s++);
	Uart_Putc(0x0d);
	Uart_Putc(0x0a);
}
/************************************************/
