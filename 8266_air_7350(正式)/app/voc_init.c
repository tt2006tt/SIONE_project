#include "include\voc_init.h"



extern unsigned char test_staus;
/*************************************************************
说明：
TEST==TEST_PWM
1、P00,P01,P02，P25，P26，P27为PWM输出，Fsys频率，周期为99+1=100；PWMDTY0为50，PWMDTY1为25，PWMDTY2为10，PWMDTY3为50，PWMDTY4为25，PWMDTY5为10；
2、PWM频率计算公式f=PWM时钟源/（PWMPRD+1）；占空比=PWMDTYx/（PWMPRD+1）;
***************************************************************/

void PWMwork(void)
{
/*
  //7320使用
	PWMCON = 0X30;  		//PWM1\PWM2输出到I/O，Fsys频率；PWM0\不输出
 	PWMCFG = 0x06;			//不反向，P00,P01,P02输出 pwm1 输出到2.6 PWM2 输出到2.7
 	PWMPRD = 99;			//PWM周期=(99+1)			 
 //	PWMDTY0 = 50;			//PWM0的Duty = 50/100  =1/2
 	PWMDTY1 = 100;//25;			//PWM1的Duty = 25/100  =1/4 
 	PWMDTY2 = 100;//10;			//PWM2的Duty = 10/100  =1/10
	//PWMDTYA = 0x15;			//PWM2/PWM1/PWM0 duty微调，为PWMDTY设定值加1
 	PWMCON |= 0x80;			//开始PWM工作
	*/
	

	//7350使用
	PWMCON  = 0X00;  	//PWM0\PWM1\PWM2不输出到I/O，失能，选择Fsys频率；
 	PWMCFG0 = 0x06;		//PWM0\PWM1\PWM2不反向，PWM4\PWM5输出到I/O PWM3\不使能
	PWMCFG1 = 0x00;   //PWM3\PWM4\PWM5不反向
 	PWMPRD  = 199;			//PWM周期=(99+1)			 
 	PWMDTY4 = 200;			//PWM4的Duty = 25/100  =1/4
 	PWMDTY5 = 200;			//PWM5的Duty = 10/100  =1/10
 	PWMCON |= 0x80;		//启动PWM工作
	
}

/*************************************************************
说明：
控制LED 亮度 PWM1、2 是用在7320上  若需用7250 则是PWM4、5
***************************************************************/

void LED_Ctrl(unsigned char duty,unsigned char no)
{
	switch(no)
	{
		case GREEN: PWMDTY4 = duty;
		
		break;
		case BLUE:	PWMDTY5 = duty;
		
		break;
		case RED:if(duty == 0)P1 = 0x00;//开灯
							else P1 = 0x04;//关灯
		
		break;
	}
}
/*************************************************************
说明：
TEST==TEST_ADC
1、ADCwork的参数包括（输入通道AIN0~AIN9或测量1/4VDD电压VDD_DIV4，采样速度填0为2M，填1为333K）
0~50 表示 0~5.0V
***************************************************************/



unsigned long ADCwork(unsigned char Channel,unsigned char Clk)
{
  unsigned int SetAdcIo; 
	unsigned long AdcValue;
	ADCCON   = ADCCON & (~0X1F) | Channel | (Clk<<5) ;	//配置ADC输入通道和采集时钟频率
	ADCCON|=0x80;								   //打开ADC模块电源
	SetAdcIo = (0x0001 << Channel) & (~0Xfe00) ;   
	ADCCFG0  = (unsigned char)SetAdcIo;			   //配置为ADC输入
	ADCCFG1  = (unsigned char)(SetAdcIo >> 8);

	ADCCON|=0x40;							 //启动ADC采集
	while(!(ADCCON&0x10));	         		 //等待 ADC转换完成;
	AdcValue=(ADCVH<<4)+(ADCVL>>4);
	
	return (AdcValue * 50/4095);
}


/*************************************************************
说明：
TEST==TEST_TIMER
1、定时器为向上溢出方式，T定时计算公式=Tx时钟源*（满量程值+1-预装载值）
***************************************************************/

#define		tst0		P00

void Timerwork(void)
{
	TMCON=0x07;							//------111 ;Timer0、Tiemr1和Tiemr2选择时钟Fsys
	TMOD=0x31;  						//0011 0001  ;Timer0工作方式1	Timer1 不工作  16位计数器
//T0设置	
	// 0x4480
	TH0= (65536 - 48000)/256;      			//如果主频为24M和时钟Fsys则24M*48000= 2ms;   
	TL0= (65536 - 48000)%256;							//TH0= 0x44; TL0 = 0x80; 也可以这样写
 	TR0=0;					
  ET0=1;								//Timer0使能
  TR0=1;								//启动Timer0
	
	EA=1;
}

void timer0()interrupt 1				//Timer0向量号 1  2ms进入一次中断
{
	WDTCON |= 0x10;		    //清看门狗	
	
	TH0= (65536 - 48000)/256;      			//再次装入初值 
	TL0= (65536 - 48000)%256;		
	if(++Cnt_10ms >= 5 && (flag_10ms == 0))
	{
		Cnt_10ms = 0;
		flag_10ms = 1;
	}
	tst0 =~tst0;
}



/*************************************************************
说明：

1、key检测
***************************************************************/
void KeyPress(void)
{
	static unsigned int Cnt_key = 0;
	static unsigned int Cnt_KeyUP = 0;
  static unsigned char flag_keyup = 0;
	//p2.1
	if((P2 & 0x02) == 0x00)//按键按下
	{
		Cnt_key ++;
	}
	else
	{
		if(Cnt_key >4 && Cnt_key < 50)
		{
			flag_keyup ++;
		}
		
		if(flag_keyup)
		{
			Cnt_KeyUP ++;
		}
		if(flag_keyup == 3 && Cnt_KeyUP > 50)
		{//如果连按三次
        Cnt_KeyUP = 0;
        flag_keyup = 0;
				//灭灯
				LED_Ctrl(200,GREEN);
				LED_Ctrl(200,RED);
				LED_Ctrl(200,BLUE);
				ProgramPara.Cnt_Minite = 0;
        ProgramPara.SubMode = 0x03; //Mode_Test
        //Mode = Mode_Test;

    }
		if(flag_keyup != 1 && flag_keyup != 3 && Cnt_KeyUP > 50)
		{
        flag_keyup = 0;
        Cnt_KeyUP = 0;
    }
    Cnt_key = 0;
	}
}

/*************************************************************
说明：
1、内部有128BYTE的EEPROM可以存数据。
***************************************************************/


unsigned char ReadData;

void EepromWrite(unsigned char Data,unsigned int Addr,unsigned char eHoldtimer)		//Data为写入数据，Addr为写入地址，eHoldtimer为写入时间；
{
	IAPDAT = Data;														  //配置写入数据
	IAPADH = (unsigned char)((Addr >> 8));								   //配置写入地址
  IAPADL = (unsigned char)Addr;
	EA=0;																   //先关总中断
	IAPADE = MOVC_EEPROM;												   //MOVC指向EEPROM
  IAPKEY = 240 ;
	IAPCTL = eHoldtimer;												   //启动写入
    _nop_();															   //至少插入4个_nop_();以上
    _nop_();
    _nop_();
    _nop_();
    _nop_();
	IAPADE = MOVC_ROM;													   //MOVC指向ROM
	EA=1; 																   //开总中断
}

unsigned char EepromRead(unsigned char Addr)
{
  unsigned char code *IapAddr=0x00;								//配置读的初始指针
	unsigned char Temp;
	EA=0;	 														//先关总中断
	IAPADE = MOVC_EEPROM;											//MOVC指向EEPROM
	Temp = *(IapAddr+Addr);											//读Addr地址的数据
	IAPADE = MOVC_ROM;												//MOVC指向ROM
	EA=1;															//开总中断
	return Temp;
}

