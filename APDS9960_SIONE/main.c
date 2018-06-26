#include "main.h"
#include "735X_IO.h"
#include "SC92F735x_C.h"
/*************************************************************
说明：
1、Options for Target‘Target1’：BL51 Locate->Code Range:0x100，烧录选项请选择DISRST，复位脚作为普通IO使用；
2、改变TEST的定义，可以分别测试对应的功能；
***************************************************************/

#define setbit(x,y) x|=(1<<y) //将X的第Y位置1
#define clrbit(x,y) x&=~(1<<y) //将X的第Y位清0

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
说明：Wonebyte()
写一个字节到1639
***************************************************************/
void Wonebyte(unsigned char input)
{
	unsigned char i;
	DIO_OUTPUT();
	_nop_();
	_nop_();
	STB_LOW();//保证STB为低电平，不依赖之前状态
	
	for(i = 0;i < 8;i++){
		CLK_LOW();
		if((input & 0x01) != 0){DIO_HIGHT();_nop_();}
		else	{DIO_LOW();_nop_();}
			
		CLK_HIGHT();
		input = input >> 1;	//准备送下一个BIT
	}
}

/*************************************************************
说明：Ronebyte()
从1639读一个byte
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
	STB_LOW();//保证STB为低电平，不依赖之前状态
	
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
说明：display2()
采用固定地址写一个byte
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
说明: SeriesWData()
连续写入多个数据 最多16byte  数据最多14byte
starAddres : 起始数据地址
*input： 数据
len: 长度
level :亮度级别
***************************************************************/
void SeriesWData(unsigned char starAddres,unsigned char *input,unsigned char len,unsigned char level)
{
	unsigned char i = 0;
	DIO_OUTPUT();
	//CLK_HIGHT();
	//DIO_HIGHT();
	//STB_HIGHT();	
	
	Wonebyte(MD_WRITE);//设置为自动加一方式
	STB_HIGHT();	
	Wonebyte(starAddres);
	
	//关键部分 每一byte数据分两次传到两个地址内去
	for(i = 0;i < len;i++)
	{
		Wonebyte(*input & 0x0f);//先发低地址
		Wonebyte((*input >> 4) & 0x0f);//再发高地址
		++input;
	}
	STB_HIGHT();	
	_nop_();_nop_();
	Wonebyte(level);//设置亮度
	STB_HIGHT();
	_nop_();_nop_();
	
}

/****************************************************************
清除所有数据
从第一位数码管(地址DIG0)开始连续清除8位数码管数据，并且关闭显示。
调用情况1：
由于上点过程中数码管显示的数据是随机的，可以先调用此函数对数码管清屏。
调用情况2：
如果系统运行过程当中显示的位数不一定（有时显示3位有时显示5位），
那么当从高位切换到低位的时候需要对多余的几位进行清显操作，
可一直接调用此ClearAll函数，也可以调用OneByteInData函数逐位写0清显
****************************************************************/
void ClearAll()
{
	unsigned char i;             //i——控制本次需要传多少个字节显示数据
			
	//通讯开始前通讯端口全部初始化为“1”
	DIO_OUTPUT();
	CLK_HIGHT();
	DIO_HIGHT();
	STB_HIGHT();	                   

	Wonebyte(MD_WRITE);            //传数据设置命令，设置采用地址自动加1方式写显示数据，
	STB_HIGHT();                     //数据设置命令传完后需要将“STB”置“1”
	Wonebyte(DIG0);               //传起始地址
																//地址命令传完后，“STB”保持为“0”继续传需要显示的数据
	for(i=0;i<16;i++)           //datacount=2*grid=2*5=10???
		Wonebyte(0);              //在连续传显示的数据过程中，“STB”一直保持为”0“
				
	STB_HIGHT();                      //传完所有的显示数据后（最多14BYTE）后，将“STB置“1”
	Wonebyte(DISPLAY_OFF);               //传显示控制命令 
	STB_HIGHT();                      //显示控制命令传完后将“STB”置“1
}

/****************************************************************
按键扫描程序
key:存储按键按下的信息int形数据，最多一次可以得到两个按键按下的结果
key1或key2：4、8、64、128，每个变量可以指示4个按键按下，key1或key2里面允许组合两个按键，
****************************************************************/
void Key_Read(unsigned int *key)
{
	unsigned char key1 = 0,key2 = 0;
	STB_HIGHT();
	DIO_HIGHT();
	
	Wonebyte(MD_READKEY);//读按键
	clrbit(P2CON,0);
	setbit(P2PH,0);
	key1 = Ronebyte();
	key2 = Ronebyte();
	
	*key = key1 | (key2 << 8);
	STB_HIGHT();
	
}
/*************************************************************
说明：main()

***************************************************************/
void main()
{
	unsigned char x[]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	unsigned int key_test = 0;
	static unsigned i = 0;
	P20IH;	 
	P21PM;
	P26PM;
	P2 = 0x43;//2.0 2.1 2.6 置高
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