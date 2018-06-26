#include "main.h"
#include "735X_IO.h"
#include "SC92F735x_C.h"
/*************************************************************
˵����
1��Options for Target��Target1����BL51 Locate->Code Range:0x100����¼ѡ����ѡ��DISRST����λ����Ϊ��ͨIOʹ�ã�
2���ı�TEST�Ķ��壬���Էֱ���Զ�Ӧ�Ĺ��ܣ�
***************************************************************/

#define setbit(x,y) x|=(1<<y) //��X�ĵ�Yλ��1
#define clrbit(x,y) x&=~(1<<y) //��X�ĵ�Yλ��0

#define CLK_HIGHT()	setbit(P2,1)
#define CLK_LOW()		clrbit(P2,1)

#define DIO_HIGHT()	setbit(P2,0)
#define DIO_LOW()		clrbit(P2,0)
#define DIO_OUTPUT()	P20PM
#define DIO_INPUT()		P20IH


#define STB_HIGHT()	setbit(P2,6)
#define STB_LOW()		clrbit(P2,6)

#define BIT(x)	(1 << (x))

/*************************************************************
˵����Wonebyte()
дһ���ֽڵ�1639
***************************************************************/
void Wonebyte(unsigned char input)
{
	unsigned char i;
	DIO_OUTPUT();
	_nop_();
	_nop_();
	STB_LOW();//��֤STBΪ�͵�ƽ��������֮ǰ״̬
	
	for(i = 0;i < 8;i++){
		CLK_LOW();
		if((input & 0x01) != 0){DIO_HIGHT();_nop_();}
		else	{DIO_LOW();_nop_();}
			
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
	//DIO_INPUT();
	clrbit(P2CON,0);
	setbit(P2PH,0);
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	STB_LOW();//��֤STBΪ�͵�ƽ��������֮ǰ״̬
	
	for(i = 0;i < 8;i++){
		CLK_LOW();
		out = out >> 1;
		if(P2 & 0x01){
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
	_nop_();_nop_();
	Wonebyte(adress);
	Wonebyte(input);
	STB_HIGHT();
	_nop_();_nop_();

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
	_nop_();_nop_();
	Wonebyte(level);//��������
	STB_HIGHT();
	_nop_();_nop_();
	
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
	clrbit(P2CON,0);
	setbit(P2PH,0);
	key1 = Ronebyte();
	key2 = Ronebyte();
	
	*key = key1 | (key2 << 8);
	STB_HIGHT();
	
}
/*************************************************************
˵����main()

***************************************************************/
void main()
{
	unsigned char x[]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	unsigned int key_test = 0;
	static unsigned i = 0;
	P20IH;	 
	P21PM;
	P26PM;
	P2 = 0x43;//2.0 2.1 2.6 �ø�
	SeriesWData(0xc0,x,8,DISPLAY_LEVEL14);
	while(1)
	{
		SeriesWData(0xc0,x,8,DISPLAY_LEVEL14);
	/*
		Key_Read(&key_test);
		if(key_test == 4 ||key_test == 8 || key_test == 64 || key_test == 128)
		{
			if(i == 0)
			{
				i = 1;
				SeriesWData(0x00,x,1,DISPLAY_LEVEL14);
			}
			else
			{
				i = 0;
				SeriesWData(0x00,x,1,DISPLAY_OFF);
			}
			
		}
		*/
		_nop_();
	}
		

	
	
}