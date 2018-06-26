#ifndef _MAIN_H_
#define _MAIN_H_


#include"intrins.h"

//工作模式
#define MD_WRITE					0x40	//连续地址模式 最多16byte 数据14byte
#define MD_FIX						0x44	//固定地址模式
#define MD_READKEY				0x42	//读取按键模式
#define MD_TEST						0x45	//内部测试模式 检测IC硬件

#define DISPLAY_LEVEL1		0X88
#define DISPLAY_LEVEL2		0X89
#define DISPLAY_LEVEL4		0X8a
#define DISPLAY_LEVEL10		0X8b
#define DISPLAY_LEVEL11		0X8c
#define DISPLAY_LEVEL12		0X8d
#define DISPLAY_LEVEL13		0X8e
#define DISPLAY_LEVEL14		0X8f
#define	DISPLAY_OFF				0X80


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

#endif