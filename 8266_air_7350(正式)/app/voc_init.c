#include "include\voc_init.h"



extern unsigned char test_staus;
/*************************************************************
˵����
TEST==TEST_PWM
1��P00,P01,P02��P25��P26��P27ΪPWM�����FsysƵ�ʣ�����Ϊ99+1=100��PWMDTY0Ϊ50��PWMDTY1Ϊ25��PWMDTY2Ϊ10��PWMDTY3Ϊ50��PWMDTY4Ϊ25��PWMDTY5Ϊ10��
2��PWMƵ�ʼ��㹫ʽf=PWMʱ��Դ/��PWMPRD+1����ռ�ձ�=PWMDTYx/��PWMPRD+1��;
***************************************************************/

void PWMwork(void)
{
/*
  //7320ʹ��
	PWMCON = 0X30;  		//PWM1\PWM2�����I/O��FsysƵ�ʣ�PWM0\�����
 	PWMCFG = 0x06;			//������P00,P01,P02��� pwm1 �����2.6 PWM2 �����2.7
 	PWMPRD = 99;			//PWM����=(99+1)			 
 //	PWMDTY0 = 50;			//PWM0��Duty = 50/100  =1/2
 	PWMDTY1 = 100;//25;			//PWM1��Duty = 25/100  =1/4 
 	PWMDTY2 = 100;//10;			//PWM2��Duty = 10/100  =1/10
	//PWMDTYA = 0x15;			//PWM2/PWM1/PWM0 duty΢����ΪPWMDTY�趨ֵ��1
 	PWMCON |= 0x80;			//��ʼPWM����
	*/
	

	//7350ʹ��
	PWMCON  = 0X00;  	//PWM0\PWM1\PWM2�������I/O��ʧ�ܣ�ѡ��FsysƵ�ʣ�
 	PWMCFG0 = 0x06;		//PWM0\PWM1\PWM2������PWM4\PWM5�����I/O PWM3\��ʹ��
	PWMCFG1 = 0x00;   //PWM3\PWM4\PWM5������
 	PWMPRD  = 199;			//PWM����=(99+1)			 
 	PWMDTY4 = 200;			//PWM4��Duty = 25/100  =1/4
 	PWMDTY5 = 200;			//PWM5��Duty = 10/100  =1/10
 	PWMCON |= 0x80;		//����PWM����
	
}

/*************************************************************
˵����
����LED ���� PWM1��2 ������7320��  ������7250 ����PWM4��5
***************************************************************/

void LED_Ctrl(unsigned char duty,unsigned char no)
{
	switch(no)
	{
		case GREEN: PWMDTY4 = duty;
		
		break;
		case BLUE:	PWMDTY5 = duty;
		
		break;
		case RED:if(duty == 0)P1 = 0x00;//����
							else P1 = 0x04;//�ص�
		
		break;
	}
}
/*************************************************************
˵����
TEST==TEST_ADC
1��ADCwork�Ĳ�������������ͨ��AIN0~AIN9�����1/4VDD��ѹVDD_DIV4�������ٶ���0Ϊ2M����1Ϊ333K��
0~50 ��ʾ 0~5.0V
***************************************************************/



unsigned long ADCwork(unsigned char Channel,unsigned char Clk)
{
  unsigned int SetAdcIo; 
	unsigned long AdcValue;
	ADCCON   = ADCCON & (~0X1F) | Channel | (Clk<<5) ;	//����ADC����ͨ���Ͳɼ�ʱ��Ƶ��
	ADCCON|=0x80;								   //��ADCģ���Դ
	SetAdcIo = (0x0001 << Channel) & (~0Xfe00) ;   
	ADCCFG0  = (unsigned char)SetAdcIo;			   //����ΪADC����
	ADCCFG1  = (unsigned char)(SetAdcIo >> 8);

	ADCCON|=0x40;							 //����ADC�ɼ�
	while(!(ADCCON&0x10));	         		 //�ȴ� ADCת�����;
	AdcValue=(ADCVH<<4)+(ADCVL>>4);
	
	return (AdcValue * 50/4095);
}


/*************************************************************
˵����
TEST==TEST_TIMER
1����ʱ��Ϊ���������ʽ��T��ʱ���㹫ʽ=Txʱ��Դ*��������ֵ+1-Ԥװ��ֵ��
***************************************************************/

#define		tst0		P00

void Timerwork(void)
{
	TMCON=0x07;							//------111 ;Timer0��Tiemr1��Tiemr2ѡ��ʱ��Fsys
	TMOD=0x31;  						//0011 0001  ;Timer0������ʽ1	Timer1 ������  16λ������
//T0����	
	// 0x4480
	TH0= (65536 - 48000)/256;      			//�����ƵΪ24M��ʱ��Fsys��24M*48000= 2ms;   
	TL0= (65536 - 48000)%256;							//TH0= 0x44; TL0 = 0x80; Ҳ��������д
 	TR0=0;					
  ET0=1;								//Timer0ʹ��
  TR0=1;								//����Timer0
	
	EA=1;
}

void timer0()interrupt 1				//Timer0������ 1  2ms����һ���ж�
{
	WDTCON |= 0x10;		    //�忴�Ź�	
	
	TH0= (65536 - 48000)/256;      			//�ٴ�װ���ֵ 
	TL0= (65536 - 48000)%256;		
	if(++Cnt_10ms >= 5 && (flag_10ms == 0))
	{
		Cnt_10ms = 0;
		flag_10ms = 1;
	}
	tst0 =~tst0;
}



/*************************************************************
˵����

1��key���
***************************************************************/
void KeyPress(void)
{
	static unsigned int Cnt_key = 0;
	static unsigned int Cnt_KeyUP = 0;
  static unsigned char flag_keyup = 0;
	//p2.1
	if((P2 & 0x02) == 0x00)//��������
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
		{//�����������
        Cnt_KeyUP = 0;
        flag_keyup = 0;
				//���
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
˵����
1���ڲ���128BYTE��EEPROM���Դ����ݡ�
***************************************************************/


unsigned char ReadData;

void EepromWrite(unsigned char Data,unsigned int Addr,unsigned char eHoldtimer)		//DataΪд�����ݣ�AddrΪд���ַ��eHoldtimerΪд��ʱ�䣻
{
	IAPDAT = Data;														  //����д������
	IAPADH = (unsigned char)((Addr >> 8));								   //����д���ַ
  IAPADL = (unsigned char)Addr;
	EA=0;																   //�ȹ����ж�
	IAPADE = MOVC_EEPROM;												   //MOVCָ��EEPROM
  IAPKEY = 240 ;
	IAPCTL = eHoldtimer;												   //����д��
    _nop_();															   //���ٲ���4��_nop_();����
    _nop_();
    _nop_();
    _nop_();
    _nop_();
	IAPADE = MOVC_ROM;													   //MOVCָ��ROM
	EA=1; 																   //�����ж�
}

unsigned char EepromRead(unsigned char Addr)
{
  unsigned char code *IapAddr=0x00;								//���ö��ĳ�ʼָ��
	unsigned char Temp;
	EA=0;	 														//�ȹ����ж�
	IAPADE = MOVC_EEPROM;											//MOVCָ��EEPROM
	Temp = *(IapAddr+Addr);											//��Addr��ַ������
	IAPADE = MOVC_ROM;												//MOVCָ��ROM
	EA=1;															//�����ж�
	return Temp;
}

