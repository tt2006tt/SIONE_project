#ifndef _732X_IO_H
#define _732X_IO_H

#include "include\SC92F732x_C.H"

#define		BS(SFR,NUM)		(SFR|=(1<<NUM))             //λ��1
#define		BC(SFR,NUM)		(SFR&=~(1<<NUM))	    //λ��0

#define		SET(SFR,NUM)	(SFR = NUM)

//=========================ǿ����IO����=========================================//

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
