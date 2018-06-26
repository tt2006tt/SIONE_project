#ifndef _VOC_INIT_H
#define _VOC_INIT_H

#include "include\main.h"

#define MOVC_EEPROM 0X02   
#define MOVC_ROM    0X00	
#define GREEN				0
#define	RED					1
#define BLUE				2
#define AIN0 0
#define IAPHOLD1MS 0x0a
//typedef enum{ IAPHOLD4MS=0x02,IAPHOLD2MS=0x06,IAPHOLD1MS=0x0a }deHoldtimer;
//typedef enum{AIN0=0,AIN1,AIN2,AIN3,AIN4,AIN5,AIN6,AIN7,AIN8,AIN9,VDD_DIV4=0xf}edAdcChannel;

unsigned char EepromRead(unsigned char Addr);
void EepromWrite(unsigned char Data,unsigned int Addr,unsigned char eHoldtimer);
void Timerwork(void);
unsigned long ADCwork(unsigned char Channel,unsigned char Clk);
void PWMwork(void);
void KeyPress(void);
void LED_Ctrl(unsigned char duty,unsigned char no);

#endif