#include "rtc.h"
#include "stm8l15x.h"


#define USE_LSE 

RTC_TimeTypeDef   RTC_TimeStr;
RTC_InitTypeDef   RTC_InitStr;

void RTC_Periph_Init(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//使能RTC时钟
#ifdef USE_LSE
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);//32.768K
#else
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);
#endif

  /* Configures the RTC wakeup timer_step = RTCCLK/16 = LSE/16 = 488.28125 us */
  RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
   
  RTC_ITConfig(RTC_IT_WUT, ENABLE);//使能唤醒中断
 
  RTC_WakeUpCmd(DISABLE);//必须先关闭才能写counter
  
  RTC_SetWakeUpCounter(1800);//30 mins
  
  RTC_WakeUpCmd(ENABLE);//使能唤醒
  
  RTC_InitStr.RTC_AsynchPrediv = 0x7F;//  1S
  RTC_InitStr.RTC_SynchPrediv = 0x00ff;
  
  RTC_Init(&RTC_InitStr);
}

void RTC_restart(unsigned short s)
{
  RTC_WakeUpCmd(DISABLE);
  RTC_SetWakeUpCounter(s);//30 mins
  RTC_WakeUpCmd(ENABLE);
}

void EnterHalt(unsigned short s)//以S为单位休眠
{
 RTC_restart(s);
 halt();  
}