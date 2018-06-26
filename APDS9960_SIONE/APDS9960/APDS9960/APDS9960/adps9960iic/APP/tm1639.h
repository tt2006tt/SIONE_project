#ifndef _TM1639_H_
#define _TM1639_H_
#include "stm8l15x.h"
#include "ADPS9960.h"
#include <stdio.h>
#include <string.h>


#define setbit(x,y) x|=(1<<y) //将X的第Y位置1
#define clrbit(x,y) x&=~(1<<y) //将X的第Y位清0



#define TM1639_PORT  GPIOB
#define TM1639_DIO_PIN  GPIO_Pin_0
#define TM1639_CLK_PIN  GPIO_Pin_1
#define TM1639_STB_PIN  GPIO_Pin_2


#define CLK_HIGHT()		TM1639_PORT->ODR |= TM1639_CLK_PIN
#define CLK_LOW()		TM1639_PORT->ODR &= (uint8_t)(~TM1639_CLK_PIN)

#define DIO_HIGHT()		TM1639_PORT->ODR |= TM1639_DIO_PIN
#define DIO_LOW()		TM1639_PORT->ODR &= (uint8_t)(~TM1639_DIO_PIN);
#define DIO_OUTPUT()	GPIO_Init(TM1639_PORT, TM1639_DIO_PIN, GPIO_Mode_Out_PP_High_Fast)
#define DIO_INPUT()		GPIO_Init(TM1639_PORT, TM1639_DIO_PIN, GPIO_Mode_In_PU_No_IT )
#define DIO_READ()      ((BitStatus)(TM1639_PORT->IDR & TM1639_DIO_PIN))


#define STB_HIGHT()		TM1639_PORT->ODR |= TM1639_STB_PIN
#define STB_LOW()		TM1639_PORT->ODR &= (uint8_t)(~TM1639_STB_PIN)

#define BIT(x)	(1 << (x))


//工作模式
#define MD_WRITE					0x40	//连续地址模式 最多16byte 数据14byte
#define MD_FIX						0x44	//固定地址模式
#define MD_READKEY					0x42	//读取按键模式
#define MD_TEST						0x45	//内部测试模式 检测IC硬件

#define DISPLAY_LEVEL1		0X88
#define DISPLAY_LEVEL2		0X89
#define DISPLAY_LEVEL4		0X8a
#define DISPLAY_LEVEL10		0X8b
#define DISPLAY_LEVEL11		0X8c
#define DISPLAY_LEVEL12		0X8d
#define DISPLAY_LEVEL13		0X8e
#define DISPLAY_LEVEL14		0X8f
#define	DISPLAY_OFF			0X80


//位使能
#define DIG0 0xc0
#define DIG1 0xc2
#define DIG2 0xc4
#define DIG3 0xc6
#define DIG4 0xc8
#define DIG5 0xca
#define DIG6 0xcc
#define DIG7 0xce




void Wonebyte(unsigned char input);
unsigned char Ronebyte(void);
void display2(unsigned char adress,unsigned char input);
void SeriesWData(unsigned char starAddres,unsigned char *input,unsigned char len,unsigned char level);
void ClearAll();
void Key_Read(unsigned int *key);
void Tm1639_Init(void);



#endif
