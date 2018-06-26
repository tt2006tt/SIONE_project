#ifndef _MAIN_H_
#define _MAIN_H_

#include "include\SC92F732X_C.h"
#include "include\732X_IO.h"
#include"intrins.h"
#include "include\voc_init.h"

#define DEBUG
#ifdef DEBUG
    #define CNT_MINITE 2000//20s
    #define CNT_HOUR   3 //1min
    #define CNT_DAY    60
    #define CNT_2DAY	  120
#else        
    #define CNT_MINITE 9000//1.5
    #define CNT_HOUR   40//1hour
    #define CNT_DAY    24//1day  
    #define CNT_2DAY		48	//48hour
#endif 




typedef struct
{
    unsigned long  Voltage;          //��������ѹ
    unsigned char  Cnt_Warning;        //��ʱ��������״̬  
    unsigned long  Cnt_Minite;       //��ʱ��������
    unsigned char   SubMode;          //��ģʽɢת
    unsigned char   flag_Time;	//ʱ�� bit0 ����,bit1 Сʱ, bit2 �졣
    unsigned char 	flag_Alarm;         //������־λ������λΪ 1 ʱ ����
    unsigned char 	Voltage_Offset;				 //У׼ƫ����
}Typdef_ProgramPara;




extern volatile unsigned char Cnt_10ms;
extern volatile unsigned char flag_10ms;
extern Typdef_ProgramPara ProgramPara;


#endif