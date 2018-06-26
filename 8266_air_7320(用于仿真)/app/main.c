#include "include\main.h"

/*************************************************************
˵����
1��Options for Target��Target1����BL51 Locate->Code Range:0x100����¼ѡ����ѡ��DISRST����λ����Ϊ��ͨIOʹ�ã�
2���ı�TEST�Ķ��壬���Էֱ���Զ�Ӧ�Ĺ��ܣ�
***************************************************************/

volatile unsigned char Cnt_10ms = 0;
volatile unsigned char flag_10ms = 0;
Typdef_ProgramPara ProgramPara = {0};

static unsigned char breath_switch = 0;
static unsigned char breath_i = 0;
void led_breath(void){
	if(ProgramPara.flag_Alarm == 0)
	{

    if(breath_switch == 0)
    {
		
		 LED_Ctrl(breath_i++,GREEN);
		 if(breath_i >= 200){
					breath_switch = 1;
			}
    }
    else if(breath_switch == 1)
    {

			LED_Ctrl(breath_i--,GREEN);
			if(breath_i == 0){
				 breath_switch = 0;
			}
    }
	}
	else if(ProgramPara.flag_Alarm == 2)
	{
		if(++breath_i > 100)
		{
			breath_i = 0;
			switch(breath_switch)
			{
				case 0:	LED_Ctrl(200,RED);breath_switch = 1;
				break;
				case 1:	LED_Ctrl(0,RED);breath_switch = 0;
				break;
			}
		}
	}
	else if(ProgramPara.flag_Alarm == 1)
	{
		
		if(++breath_i > 100)
		{
			breath_i = 0;
			switch(breath_switch)
			{
				case 0:	LED_Ctrl(200,RED);LED_Ctrl(200,GREEN);breath_switch = 1;
				break;
				case 1:	LED_Ctrl(0,RED);LED_Ctrl(0,GREEN);breath_switch = 0;
				break;
			}
		}
	}
	
}

void Detect(void)
{
	int test = 0;
	ProgramPara.Voltage = ADCwork(AIN0,1);
	test = ProgramPara.Voltage - ProgramPara.Voltage_Offset;
	if(test < 0)
		ProgramPara.Voltage = 0;
	else
		ProgramPara.Voltage = test;
	if(ProgramPara.Voltage <= 15)//С��2V
	{	
		LED_Ctrl(200,BLUE);
		LED_Ctrl(200,RED);
		ProgramPara.flag_Alarm = 0;
		ProgramPara.Cnt_Warning = 0;
	}
	else if((ProgramPara.Voltage > 15) && (ProgramPara.Voltage <= 30 ))//С��3V ����1.5V
	{
		if(++ProgramPara.Cnt_Warning > 100 )
		{
			ProgramPara.Cnt_Warning = 0;
			ProgramPara.flag_Alarm = 1;
			LED_Ctrl(200,RED);
		}
	}
	else if(ProgramPara.Voltage > 30)//����2.5V
	{
		if(++ProgramPara.Cnt_Warning > 100 )
		{
			LED_Ctrl(200,GREEN);
			LED_Ctrl(200,BLUE);
			ProgramPara.Cnt_Warning = 0;
			ProgramPara.flag_Alarm = 2;
		}
	}
}
/*************************************************************
˵����main()
�Լ�
***************************************************************/
unsigned char test_staus = 0;
void Mode_Test1(void)
{
	static unsigned char i = 0;
	
	if(++i > 100)
	{
		if(test_staus == 0)
		{
			test_staus = 1;
			LED_Ctrl(200,GREEN);
			LED_Ctrl(200,RED);
			LED_Ctrl(0,BLUE);
			i = 0;
		}
		else if(test_staus == 1)
		{
			test_staus = 2;
			LED_Ctrl(200,GREEN);
			LED_Ctrl(0,RED);
			LED_Ctrl(200,BLUE);
			i = 0;
		}
		else if(test_staus == 2)
		{
			test_staus = 3;
			LED_Ctrl(0,GREEN);
			LED_Ctrl(200,RED);
			LED_Ctrl(200,BLUE);
			i = 0;
		}
		else if(test_staus == 3)
		{
			test_staus = 0;
			breath_switch = 0;
			breath_i = 0;
			i = 0;

			//ProgramPara.Voltage_Offset = ADCwork(AIN0,1);
			EepromWrite(0x09,0,IAPHOLD1MS);//д��Ĭ�ϵı궨ֵ
			if(ProgramPara.Voltage_Offset > 30 || ProgramPara.Voltage_Offset < 5)
			{
				ProgramPara.SubMode = 0x05;//�Լ�ʧ��
				LED_Ctrl(200,GREEN);
				LED_Ctrl(200,RED);
				LED_Ctrl(0,BLUE);
			}
			else
			{
				ProgramPara.SubMode = 0x01;//�Լ�ɹ�
				LED_Ctrl(0,GREEN);
				LED_Ctrl(200,RED);
				LED_Ctrl(200,BLUE);
			}
			ProgramPara.Cnt_Minite = 0;
			BC(ProgramPara.flag_Time,0);
		}
		
	}
}

/*************************************************************
˵����Mode_Test()
�궨
***************************************************************/

void Mode_Test(void)
{
	static unsigned char i = 0;
	
	if(++i > 100)
	{
		if(test_staus == 0)
		{
			test_staus = 1;
			LED_Ctrl(0,GREEN);
			LED_Ctrl(200,RED);
			LED_Ctrl(200,BLUE);
			i = 0;
		}
		else if(test_staus == 1)
		{
			test_staus = 2;
			LED_Ctrl(200,GREEN);
			LED_Ctrl(0,RED);
			LED_Ctrl(200,BLUE);
			i = 0;
		}
		else if(test_staus == 2)
		{
			test_staus = 3;
			LED_Ctrl(200,GREEN);
			LED_Ctrl(200,RED);
			LED_Ctrl(0,BLUE);
			i = 0;
		}
		else if(test_staus == 3)
		{
			test_staus = 0;
			breath_switch = 0;
			breath_i = 0;
			i = 0;
			ProgramPara.Voltage_Offset = ADCwork(AIN0,1);
			EepromWrite(ProgramPara.Voltage_Offset,0,IAPHOLD1MS);
			ProgramPara.SubMode = 0x02;//�궨���
			LED_Ctrl(0,GREEN);
			LED_Ctrl(200,RED);
			LED_Ctrl(200,BLUE);
			ProgramPara.Cnt_Minite = 0;
			BC(ProgramPara.flag_Time,0);
		}
		
	}
}


/*************************************************************
˵����main()

***************************************************************/
void main()
{
//	static unsigned char Cnt_2Day = 0;
	static unsigned char Cnt_Hour = 0;
	static unsigned char Cnt_Day = 0;
	unsigned char i = 0;
	
	ProgramPara.SubMode = 0x01; //Mode_Init

//����P21Ϊ������IO key
	P21IH;
// P12Ϊǿ����IO ���ҳ�ʼ����
	P12PM;P1 = 0x04;
//����PWM
	PWMwork();
	LED_Ctrl(0,GREEN);
	LED_Ctrl(200,RED);
	LED_Ctrl(200,BLUE);
//��TIMER
	Timerwork();



//��ȡ����EEPROM 
	ProgramPara.Voltage_Offset = EepromRead(0);

	ProgramPara.SubMode = 0x01; //Mode_Init
	if((P2 & 0x02) == 0x00)//��������
	{
		for(i = 0;i< 200;i++);
		if((P2 & 0x02) == 0x00)//��������
		{
			ProgramPara.SubMode = 0x04; //Mode_test
			LED_Ctrl(200,GREEN);
			LED_Ctrl(200,RED);
			LED_Ctrl(200,BLUE);
		}
	}
	
	while(1)
	{
		while(!flag_10ms);//��������³���10msѭ��һ��
    flag_10ms = 0;
		KeyPress();
		if(++ProgramPara.Cnt_Minite >= CNT_MINITE){

			ProgramPara.Cnt_Minite = 0;
			
			BS(ProgramPara.flag_Time,0);

			if(++Cnt_Hour>= CNT_HOUR){
				Cnt_Hour = 0;
				//BS(ProgramPara.flag_Time,1); 
				if(++Cnt_Day >= CNT_DAY){
					Cnt_Day = 0;
					//BS(ProgramPara.flag_Time,2);
				}
			}
		}
		switch(ProgramPara.SubMode)
		{
			//��ʼ��
			
			case 0x01:		if(ProgramPara.flag_Time & 0x01)
										{
											BC(ProgramPara.flag_Time,0);
											ProgramPara.SubMode = 0x02; //Mode_Normal
										}
			break;
			//��ͨģʽ
			case 0x02:		led_breath();
										//���ADC
										Detect();
			break;
			//�궨
			case 0x03:		Mode_Test();
					
			break;
			//�����Լ��Լ�ģʽ
			case 0x04:		Mode_Test1();
					
			break;
			case 0x05:		LED_Ctrl(0,BLUE);
			break;
			default:ProgramPara.SubMode = 0x01;
			break;
			
		}
		

	}
	
}