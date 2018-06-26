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
    unsigned long  Voltage;          //传感器电压
    unsigned char  Cnt_Warning;        //计时器，警告状态  
    unsigned long  Cnt_Minite;       //计时器，分钟
    unsigned char   SubMode;          //子模式散转
    unsigned char   flag_Time;	//时间 bit0 分钟,bit1 小时, bit2 天。
    unsigned char 	flag_Alarm;         //报警标志位，当此位为 1 时 报警
    unsigned char 	Voltage_Offset;				 //校准偏移量
}Typdef_ProgramPara;




extern volatile unsigned char Cnt_10ms;
extern volatile unsigned char flag_10ms;
extern Typdef_ProgramPara ProgramPara;


#endif