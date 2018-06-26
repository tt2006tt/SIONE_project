#include "tm1639.h"



void Tm1639_Init(void)

{
	GPIO_Init(TM1639_PORT, TM1639_DIO_PIN, GPIO_Mode_Out_PP_High_Fast);
	GPIO_Init(TM1639_PORT, TM1639_STB_PIN, GPIO_Mode_Out_PP_High_Fast);
	GPIO_Init(TM1639_PORT, TM1639_CLK_PIN, GPIO_Mode_Out_PP_High_Fast);
}

/*************************************************************
˵����Wonebyte()
дһ���ֽڵ�1639
***************************************************************/
void Wonebyte(unsigned char input)
{
	unsigned char i;
	DIO_OUTPUT();
	nop();
	nop();
	STB_LOW();//��֤STBΪ�͵�ƽ��������֮ǰ״̬
	
	for(i = 0;i < 8;i++){
		CLK_LOW();
		if((input & 0x01) != 0){DIO_HIGHT();nop();}
		else	{DIO_LOW();nop();}
			
		CLK_HIGHT();
		input = input >> 1;	//׼������һ��BIT
	}
}

/*************************************************************
˵����Ronebyte()
��1639��һ��byte
***************************************************************/
unsigned char Ronebyte(void)
{
	unsigned char i,out = 0;
	DIO_INPUT();
	nop();
	nop();
	nop();
	nop();
	STB_LOW();//��֤STBΪ�͵�ƽ��������֮ǰ״̬
	
	for(i = 0;i < 8;i++){
		CLK_LOW();
		out = out >> 1;
		if(DIO_READ()){
			out = out | 0x80;
		}
			
		CLK_HIGHT();
	}
	return (out);
}

/*************************************************************
˵����display2()
���ù̶���ַдһ��byte
***************************************************************/
void display2(unsigned char adress,unsigned char input)
{
	DIO_OUTPUT();
	CLK_HIGHT();
	DIO_HIGHT();
	STB_HIGHT();	
	
	Wonebyte(MD_FIX);
	STB_HIGHT();
	nop();nop();
	Wonebyte(adress);
	Wonebyte(input);
	STB_HIGHT();
	nop();nop();

}

/*************************************************************
˵��: SeriesWData()
����д�������� ���16byte  �������14byte
starAddres : ��ʼ���ݵ�ַ
*input�� ����
len: ����
level :���ȼ���
***************************************************************/
void SeriesWData(unsigned char starAddres,unsigned char *input,unsigned char len,unsigned char level)
{
	unsigned char i = 0;
	DIO_OUTPUT();
	//CLK_HIGHT();
	//DIO_HIGHT();
	//STB_HIGHT();	
	
	Wonebyte(MD_WRITE);//����Ϊ�Զ���һ��ʽ
	STB_HIGHT();	
	Wonebyte(starAddres);
	
	//�ؼ����� ÿһbyte���ݷ����δ���������ַ��ȥ
	for(i = 0;i < len;i++)
	{
		Wonebyte(*input & 0x0f);//�ȷ��͵�ַ
		Wonebyte((*input >> 4) & 0x0f);//�ٷ��ߵ�ַ
		++input;
	}
	STB_HIGHT();	
	nop();nop();
	Wonebyte(level);//��������
	STB_HIGHT();
	nop();nop();
	
}

/****************************************************************
�����������
�ӵ�һλ�����(��ַDIG0)��ʼ�������8λ��������ݣ����ҹر���ʾ��
�������1��
�����ϵ�������������ʾ������������ģ������ȵ��ô˺����������������
�������2��
���ϵͳ���й��̵�����ʾ��λ����һ������ʱ��ʾ3λ��ʱ��ʾ5λ����
��ô���Ӹ�λ�л�����λ��ʱ����Ҫ�Զ���ļ�λ�������Բ�����
��һֱ�ӵ��ô�ClearAll������Ҳ���Ե���OneByteInData������λд0����
****************************************************************/
void ClearAll()
{
	unsigned char i;             //i�������Ʊ�����Ҫ�����ٸ��ֽ���ʾ����
			
	//ͨѶ��ʼǰͨѶ�˿�ȫ����ʼ��Ϊ��1��
	DIO_OUTPUT();
	CLK_HIGHT();
	DIO_HIGHT();
	STB_HIGHT();	                   

	Wonebyte(MD_WRITE);            //����������������ò��õ�ַ�Զ���1��ʽд��ʾ���ݣ�
	STB_HIGHT();                     //����������������Ҫ����STB���á�1��
	Wonebyte(DIG0);               //����ʼ��ַ
																//��ַ�����󣬡�STB������Ϊ��0����������Ҫ��ʾ������
	for(i=0;i<16;i++)           //datacount=2*grid=2*5=10???
		Wonebyte(0);              //����������ʾ�����ݹ����У���STB��һֱ����Ϊ��0��
				
	STB_HIGHT();                      //�������е���ʾ���ݺ����14BYTE���󣬽���STB�á�1��
	Wonebyte(DISPLAY_OFF);               //����ʾ�������� 
	STB_HIGHT();                      //��ʾ���������󽫡�STB���á�1
}

/****************************************************************
����ɨ�����
key:�洢�������µ���Ϣint�����ݣ����һ�ο��Եõ������������µĽ��
key1��key2��4��8��64��128��ÿ����������ָʾ4���������£�key1��key2���������������������
****************************************************************/
void Key_Read(unsigned int *key)
{
	unsigned char key1 = 0,key2 = 0;
	STB_HIGHT();
	DIO_HIGHT();
	
	Wonebyte(MD_READKEY);//������
	DIO_INPUT();
	key1 = Ronebyte();
	key2 = Ronebyte();
	
	*key = key1 | (key2 << 8);
	STB_HIGHT();
	
}

