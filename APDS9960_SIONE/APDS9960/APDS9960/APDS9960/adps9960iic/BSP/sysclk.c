#include "stm8l15x.h"
#include "sysclk.h"

void SYSCLK_Init()
{
  CLK_DeInit( );//复位所有时钟状态
  
  CLK_HSICmd(ENABLE);//使能HSI 其实复位之后自动启动并使用HSI
  
  CLK_HSEConfig(CLK_HSE_ON);//打开HSE 使用外部晶振
  
  CLK_LSICmd(DISABLE);//关闭LSI   38khz
  
  CLK_LSEConfig(CLK_LSE_ON);//打开LSE  使用外部晶振
  
  //CLK_SYSCLKSourceSwitchCmd(ENABLE);//使能时钟自动切换
  
  //CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);//选择HSE作为SYSCLK
  
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);//临时
   
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//16M的HSE 1分频，SYSCLK为16MHZ
  
  //当时钟切换成功之后关闭HSI
 // CLK_HSICmd(DISABLE);
  
  CLK_MainRegulatorCmd(ENABLE);// 使能主电压调节器
  
  CLK_ClockSecuritySystemEnable( );//使能时钟安全机制，当HSE有问题的时候自动切换到HSI
  
  CLK_ITConfig(CLK_IT_SWIF,ENABLE);//时钟切换中断开启，用于观察切换事件 
  
  CLK_HaltConfig(CLK_Halt_FastWakeup, DISABLE);//快速唤醒,此处不能使能，因为这样会使唤醒之后使用HSI为主时钟
  
  CLK_HaltConfig(CLK_Halt_SlowWakeup, DISABLE);//在active-halt模式，MVR处于打开状态
  
  while(PWR_GetFlagStatus(PWR_FLAG_VREFINTF) != SET);//只有当内部参考电压稳定的时候才可以修改以下的值
   
  //为了减少唤醒时间，使能快速唤醒之后，器件不需要等待内部参考电压的启动时间
  PWR_FastWakeUpCmd(ENABLE); 
  
  //使能超级低电压的功能，在HALT模式下自动关闭内部参考电压
  //唤醒的时候自动重启，缺点是唤醒事件比较长，但是前面的快速启动模式可以弥补
  PWR_UltraLowPowerCmd(ENABLE);
  
  PWR_PVDCmd(ENABLE);//使能电压检测器
  
  PWR_PVDITConfig(ENABLE);//使能电压检测器中断
  
  PWR_PVDLevelConfig(PWR_PVDLevel_1V85);//1.85V
 
}