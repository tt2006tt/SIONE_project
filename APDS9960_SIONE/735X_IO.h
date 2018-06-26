#ifndef _732X_IO_H
#define _732X_IO_H

#include "SC92F735x_C.H"

#define		BS(SFR,NUM)		(SFR|=(1<<NUM))             //λ��1
#define		BC(SFR,NUM)		(SFR&=~(1<<NUM))	    //λ��0

#define		SET(SFR,NUM)	(SFR = NUM)

//=========================ǿ����IO����=========================================//

#define	 P00PM	 BS(P0CON,0)	 //����P00Ϊǿ����IO
#define	 P01PM	 BS(P0CON,1)	 //����P01Ϊǿ����IO
#define	 P02PM	 BS(P0CON,2)	 //����P02Ϊǿ����IO
#define	 P03PM	 BS(P0CON,3)	 //����P03Ϊǿ����IO
#define	 P04PM	 BS(P0CON,4)	 //����P04Ϊǿ����IO
#define	 P05PM	 BS(P0CON,5)	 //����P05Ϊǿ����IO
#define	 P06PM	 BS(P0CON,6)	 //����P06Ϊǿ����IO
#define	 P07PM	 BS(P0CON,7)	 //����P07Ϊǿ����IO

#define	 P10PM	 BS(P1CON,0)	 //����P10Ϊǿ����IO
#define	 P11PM	 BS(P1CON,1)	 //����P11Ϊǿ����IO
#define	 P12PM	 BS(P1CON,2)	 //����P12Ϊǿ����IO
#define	 P13PM	 BS(P1CON,3)	 //����P13Ϊǿ����IO
#define	 P14PM	 BS(P1CON,4)	 //����P14Ϊǿ����IO
#define	 P15PM	 BS(P1CON,5)	 //����P15Ϊǿ����IO
#define	 P16PM	 BS(P1CON,6)	 //����P16Ϊǿ����IO
#define	 P17PM	 BS(P1CON,7)	 //����P17Ϊǿ����IO

#define	 P20PM	 BS(P2CON,0)	 //����P20Ϊǿ����IO
#define	 P21PM	 BS(P2CON,1)	 //����P21Ϊǿ����IO
#define	 P22PM	 BS(P2CON,2)	 //����P22Ϊǿ����IO
#define	 P23PM	 BS(P2CON,3)	 //����P23Ϊǿ����IO
#define	 P24PM	 BS(P2CON,4)	 //����P24Ϊǿ����IO
#define	 P25PM	 BS(P2CON,5)	 //����P25Ϊǿ����IO
#define	 P26PM	 BS(P2CON,6)	 //����P26Ϊǿ����IO
#define	 P27PM	 BS(P2CON,7)	 //����P27Ϊǿ����IO

#define	 P50PM	 BS(P5CON,0)	 //����P50Ϊǿ����IO
#define	 P51PM	 BS(P5CON,1)	 //����P51Ϊǿ����IO

//=========================����̬IO����=========================================//

#define	 P00IM	 (BC(P0CON,0),BC(P0PH,0))	 //����P00Ϊ����̬IO
#define	 P01IM	 (BC(P0CON,1),BC(P0PH,1))	 //����P01Ϊ����̬IO
#define	 P02IM	 (BC(P0CON,2),BC(P0PH,2))	 //����P02Ϊ����̬IO
#define	 P03IM	 (BC(P0CON,3),BC(P0PH,3))	 //����P03Ϊ����̬IO
#define	 P04IM	 (BC(P0CON,4),BC(P0PH,4))	 //����P04Ϊ����̬IO
#define	 P05IM	 (BC(P0CON,5),BC(P0PH,5))	 //����P05Ϊ����̬IO
#define	 P06IM	 (BC(P0CON,6),BC(P0PH,6))	 //����P06Ϊ����̬IO
#define	 P07IM	 (BC(P0CON,7),BC(P0PH,7))	 //����P07Ϊ����̬IO

#define	 P10IM	 (BC(P1CON,0),BC(P1PH,0))	 //����P10Ϊ����̬IO
#define	 P11IM	 (BC(P1CON,1),BC(P1PH,1))	 //����P11Ϊ����̬IO
#define	 P12IM	 (BC(P1CON,2),BC(P1PH,2))	 //����P12Ϊ����̬IO
#define	 P13IM	 (BC(P1CON,3),BC(P1PH,3))	 //����P13Ϊ����̬IO
#define	 P14IM	 (BC(P1CON,4),BC(P1PH,4))	 //����P14Ϊ����̬IO
#define	 P15IM	 (BC(P1CON,5),BC(P1PH,5))	 //����P15Ϊ����̬IO
#define	 P16IM	 (BC(P1CON,6),BC(P1PH,6))	 //����P16Ϊ����̬IO
#define	 P17IM	 (BC(P1CON,7),BC(P1PH,7))	 //����P17Ϊ����̬IO

#define	 P20IM	 (BC(P2CON,0),BC(P2PH,0))	 //����P20Ϊ����̬IO
#define	 P21IM	 (BC(P2CON,1),BC(P2PH,1))	 //����P21Ϊ����̬IO
#define	 P22IM	 (BC(P2CON,2),BC(P2PH,2))	 //����P22Ϊ����̬IO
#define	 P23IM	 (BC(P2CON,3),BC(P2PH,3))	 //����P23Ϊ����̬IO
#define	 P24IM	 (BC(P2CON,4),BC(P2PH,4))	 //����P24Ϊ����̬IO
#define	 P25IM	 (BC(P2CON,5),BC(P2PH,5))	 //����P25Ϊ����̬IO
#define	 P26IM	 (BC(P2CON,6),BC(P2PH,6))	 //����P26Ϊ����̬IO
#define	 P27IM	 (BC(P2CON,7),BC(P2PH,7))	 //����P27Ϊ����̬IO

#define	 P50IM	 (BC(P5CON,0),BC(P5PH,0))	 //����P50Ϊ����̬IO
#define	 P51IM	 (BC(P5CON,1),BC(P5PH,1))	 //����P51Ϊ����̬IO

//=========================������IO����=========================================//

#define	 P00IH	 (BC(P0CON,0),BS(P0PH,0))	 //����P00Ϊ������IO
#define	 P01IH	 (BC(P0CON,1),BS(P0PH,1))	 //����P01Ϊ������IO
#define	 P02IH	 (BC(P0CON,2),BS(P0PH,2))	 //����P02Ϊ������IO
#define	 P03IH	 (BC(P0CON,3),BS(P0PH,3))	 //����P03Ϊ������IO
#define	 P04IH	 (BC(P0CON,4),BS(P0PH,4))	 //����P04Ϊ������IO
#define	 P05IH	 (BC(P0CON,5),BS(P0PH,5))	 //����P05Ϊ������IO
#define	 P06IH	 (BC(P0CON,6),BS(P0PH,6))	 //����P06Ϊ������IO
#define	 P07IH	 (BC(P0CON,7),BS(P0PH,7))	 //����P07Ϊ������IO

#define	 P10IH	 (BC(P1CON,0),BS(P1PH,0))	 //����P10Ϊ������IO
#define	 P11IH	 (BC(P1CON,1),BS(P1PH,1))	 //����P11Ϊ������IO
#define	 P12IH	 (BC(P1CON,2),BS(P1PH,2))	 //����P12Ϊ������IO
#define	 P13IH	 (BC(P1CON,3),BS(P1PH,3))	 //����P13Ϊ������IO
#define	 P14IH	 (BC(P1CON,4),BS(P1PH,4))	 //����P14Ϊ������IO
#define	 P15IH	 (BC(P1CON,5),BS(P1PH,5))	 //����P15Ϊ������IO
#define	 P16IH	 (BC(P1CON,6),BS(P1PH,6))	 //����P16Ϊ������IO
#define	 P17IH	 (BC(P1CON,7),BS(P1PH,7))	 //����P17Ϊ������IO

#define	 P20IH	 (BC(P2CON,0),BS(P2PH,0))	 //����P20Ϊ������IO
#define	 P21IH	 (BC(P2CON,1),BS(P2PH,1))	 //����P21Ϊ������IO
#define	 P22IH	 (BC(P2CON,2),BS(P2PH,2))	 //����P22Ϊ������IO
#define	 P23IH	 (BC(P2CON,3),BS(P2PH,3))	 //����P23Ϊ������IO
#define	 P24IH	 (BC(P2CON,4),BS(P2PH,4))	 //����P24Ϊ������IO
#define	 P25IH	 (BC(P2CON,5),BS(P2PH,5))	 //����P25Ϊ������IO
#define	 P26IH	 (BC(P2CON,6),BS(P2PH,6))	 //����P26Ϊ������IO
#define	 P27IH	 (BC(P2CON,7),BS(P2PH,7))	 //����P27Ϊ������IO

#define	 P50IH	 (BC(P5CON,0),BS(P5PH,0))	 //����P50Ϊ������IO
#define	 P51IH	 (BC(P5CON,1),BS(P5PH,1))	 //����P51Ϊ������IO

#endif
