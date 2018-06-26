#include "adc.h"
#include "stm8l15x.h"
#include "stm8l15x_dma.h"

#define DMA_BUF_SIZE 4

//半字位宽
volatile  unsigned short  ADC_ConvertedValue[DMA_BUF_SIZE];
volatile float ADC1_Channel1_Battery,ADC1_Channel_Vrefint;
#define ADC1_DR_Address    ((uint16_t)(0x5344) )//ADC1外设地址
  
void ADC_Periph_Init(void)
{
  u8 idx;
  
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);//使能ADC时钟 
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);//使能DMA1时钟
  
  GPIO_Init(GPIOA,GPIO_Pin_5,GPIO_Mode_In_FL_No_IT);//浮空输入
  
  ADC_Cmd(ADC1,ENABLE);//打开ADC
   
  for (idx = 0;idx < 50;idx++);//adc上电需要一段时间
       
  ADC_VrefintCmd(ENABLE);//使能内部参考电压
  
  ADC_DMACmd(ADC1,ENABLE);//使用ADC的DMA功能
  
  //连续转换模式
  ADC_Init(ADC1, ADC_ConversionMode_Continuous,ADC_Resolution_12Bit, ADC_Prescaler_1);
  
  ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_192Cycles);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels,ADC_SamplingTime_192Cycles);
                           
                            
  ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, ENABLE);//内部参考电压
  ADC_ChannelCmd(ADC1, ADC_Channel_1, ENABLE);//通道1   PA5
  
  SYSCFG_REMAPDMAChannelConfig(REMAP_DMA1Channel_ADC1ToChannel0);
 
  DMA_Init(DMA1_Channel0,//通道0
          (uint32_t)&ADC_ConvertedValue,//缓冲区地址
          (uint16_t)ADC1_DR_Address,//ADC数据寄存器地址
          DMA_BUF_SIZE,//缓冲区大小
          DMA_DIR_PeripheralToMemory,//外设到内存
          DMA_Mode_Circular,//循环转换
          DMA_MemoryIncMode_Inc,//内存地址自增
          DMA_Priority_High,//优先级为高
          DMA_MemoryDataSize_HalfWord);//内存数据是半字位宽
   
  DMA_SetCurrDataCounter(DMA1_Channel0,DMA_BUF_SIZE);//设置转速的数据个数 
 
  DMA_ITConfig(DMA1_Channel0,DMA_ITx_TC, ENABLE);//使能传输完成中断
  DMA_ITConfig(DMA1_Channel0,DMA_ITx_HT, DISABLE);
  
  DMA_Cmd(DMA1_Channel0,ENABLE);//使能通道0    
  
  DMA_GlobalCmd(ENABLE);//打开DMA
  
  ADC_SoftwareStartConv(ADC1);//software trigger
}
