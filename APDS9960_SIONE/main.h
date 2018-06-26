#ifndef _MAIN_H_
#define _MAIN_H_


#include"intrins.h"

//����ģʽ
#define MD_WRITE					0x40	//������ַģʽ ���16byte ����14byte
#define MD_FIX						0x44	//�̶���ַģʽ
#define MD_READKEY				0x42	//��ȡ����ģʽ
#define MD_TEST						0x45	//�ڲ�����ģʽ ���ICӲ��

#define DISPLAY_LEVEL1		0X88
#define DISPLAY_LEVEL2		0X89
#define DISPLAY_LEVEL4		0X8a
#define DISPLAY_LEVEL10		0X8b
#define DISPLAY_LEVEL11		0X8c
#define DISPLAY_LEVEL12		0X8d
#define DISPLAY_LEVEL13		0X8e
#define DISPLAY_LEVEL14		0X8f
#define	DISPLAY_OFF				0X80


//λʹ��
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