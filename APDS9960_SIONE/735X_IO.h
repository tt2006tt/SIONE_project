#ifndef _732X_IO_H
#define _732X_IO_H

#include "SC92F735x_C.H"

#define		BS(SFR,NUM)		(SFR|=(1<<NUM))             //位置1
#define		BC(SFR,NUM)		(SFR&=~(1<<NUM))	    //位清0

#define		SET(SFR,NUM)	(SFR = NUM)

//=========================强推挽IO设置=========================================//

#define	 P00PM	 BS(P0CON,0)	 //设置P00为强推挽IO
#define	 P01PM	 BS(P0CON,1)	 //设置P01为强推挽IO
#define	 P02PM	 BS(P0CON,2)	 //设置P02为强推挽IO
#define	 P03PM	 BS(P0CON,3)	 //设置P03为强推挽IO
#define	 P04PM	 BS(P0CON,4)	 //设置P04为强推挽IO
#define	 P05PM	 BS(P0CON,5)	 //设置P05为强推挽IO
#define	 P06PM	 BS(P0CON,6)	 //设置P06为强推挽IO
#define	 P07PM	 BS(P0CON,7)	 //设置P07为强推挽IO

#define	 P10PM	 BS(P1CON,0)	 //设置P10为强推挽IO
#define	 P11PM	 BS(P1CON,1)	 //设置P11为强推挽IO
#define	 P12PM	 BS(P1CON,2)	 //设置P12为强推挽IO
#define	 P13PM	 BS(P1CON,3)	 //设置P13为强推挽IO
#define	 P14PM	 BS(P1CON,4)	 //设置P14为强推挽IO
#define	 P15PM	 BS(P1CON,5)	 //设置P15为强推挽IO
#define	 P16PM	 BS(P1CON,6)	 //设置P16为强推挽IO
#define	 P17PM	 BS(P1CON,7)	 //设置P17为强推挽IO

#define	 P20PM	 BS(P2CON,0)	 //设置P20为强推挽IO
#define	 P21PM	 BS(P2CON,1)	 //设置P21为强推挽IO
#define	 P22PM	 BS(P2CON,2)	 //设置P22为强推挽IO
#define	 P23PM	 BS(P2CON,3)	 //设置P23为强推挽IO
#define	 P24PM	 BS(P2CON,4)	 //设置P24为强推挽IO
#define	 P25PM	 BS(P2CON,5)	 //设置P25为强推挽IO
#define	 P26PM	 BS(P2CON,6)	 //设置P26为强推挽IO
#define	 P27PM	 BS(P2CON,7)	 //设置P27为强推挽IO

#define	 P50PM	 BS(P5CON,0)	 //设置P50为强推挽IO
#define	 P51PM	 BS(P5CON,1)	 //设置P51为强推挽IO

//=========================高阻态IO设置=========================================//

#define	 P00IM	 (BC(P0CON,0),BC(P0PH,0))	 //设置P00为高阻态IO
#define	 P01IM	 (BC(P0CON,1),BC(P0PH,1))	 //设置P01为高阻态IO
#define	 P02IM	 (BC(P0CON,2),BC(P0PH,2))	 //设置P02为高阻态IO
#define	 P03IM	 (BC(P0CON,3),BC(P0PH,3))	 //设置P03为高阻态IO
#define	 P04IM	 (BC(P0CON,4),BC(P0PH,4))	 //设置P04为高阻态IO
#define	 P05IM	 (BC(P0CON,5),BC(P0PH,5))	 //设置P05为高阻态IO
#define	 P06IM	 (BC(P0CON,6),BC(P0PH,6))	 //设置P06为高阻态IO
#define	 P07IM	 (BC(P0CON,7),BC(P0PH,7))	 //设置P07为高阻态IO

#define	 P10IM	 (BC(P1CON,0),BC(P1PH,0))	 //设置P10为高阻态IO
#define	 P11IM	 (BC(P1CON,1),BC(P1PH,1))	 //设置P11为高阻态IO
#define	 P12IM	 (BC(P1CON,2),BC(P1PH,2))	 //设置P12为高阻态IO
#define	 P13IM	 (BC(P1CON,3),BC(P1PH,3))	 //设置P13为高阻态IO
#define	 P14IM	 (BC(P1CON,4),BC(P1PH,4))	 //设置P14为高阻态IO
#define	 P15IM	 (BC(P1CON,5),BC(P1PH,5))	 //设置P15为高阻态IO
#define	 P16IM	 (BC(P1CON,6),BC(P1PH,6))	 //设置P16为高阻态IO
#define	 P17IM	 (BC(P1CON,7),BC(P1PH,7))	 //设置P17为高阻态IO

#define	 P20IM	 (BC(P2CON,0),BC(P2PH,0))	 //设置P20为高阻态IO
#define	 P21IM	 (BC(P2CON,1),BC(P2PH,1))	 //设置P21为高阻态IO
#define	 P22IM	 (BC(P2CON,2),BC(P2PH,2))	 //设置P22为高阻态IO
#define	 P23IM	 (BC(P2CON,3),BC(P2PH,3))	 //设置P23为高阻态IO
#define	 P24IM	 (BC(P2CON,4),BC(P2PH,4))	 //设置P24为高阻态IO
#define	 P25IM	 (BC(P2CON,5),BC(P2PH,5))	 //设置P25为高阻态IO
#define	 P26IM	 (BC(P2CON,6),BC(P2PH,6))	 //设置P26为高阻态IO
#define	 P27IM	 (BC(P2CON,7),BC(P2PH,7))	 //设置P27为高阻态IO

#define	 P50IM	 (BC(P5CON,0),BC(P5PH,0))	 //设置P50为高阻态IO
#define	 P51IM	 (BC(P5CON,1),BC(P5PH,1))	 //设置P51为高阻态IO

//=========================带上拉IO设置=========================================//

#define	 P00IH	 (BC(P0CON,0),BS(P0PH,0))	 //设置P00为带上拉IO
#define	 P01IH	 (BC(P0CON,1),BS(P0PH,1))	 //设置P01为带上拉IO
#define	 P02IH	 (BC(P0CON,2),BS(P0PH,2))	 //设置P02为带上拉IO
#define	 P03IH	 (BC(P0CON,3),BS(P0PH,3))	 //设置P03为带上拉IO
#define	 P04IH	 (BC(P0CON,4),BS(P0PH,4))	 //设置P04为带上拉IO
#define	 P05IH	 (BC(P0CON,5),BS(P0PH,5))	 //设置P05为带上拉IO
#define	 P06IH	 (BC(P0CON,6),BS(P0PH,6))	 //设置P06为带上拉IO
#define	 P07IH	 (BC(P0CON,7),BS(P0PH,7))	 //设置P07为带上拉IO

#define	 P10IH	 (BC(P1CON,0),BS(P1PH,0))	 //设置P10为带上拉IO
#define	 P11IH	 (BC(P1CON,1),BS(P1PH,1))	 //设置P11为带上拉IO
#define	 P12IH	 (BC(P1CON,2),BS(P1PH,2))	 //设置P12为带上拉IO
#define	 P13IH	 (BC(P1CON,3),BS(P1PH,3))	 //设置P13为带上拉IO
#define	 P14IH	 (BC(P1CON,4),BS(P1PH,4))	 //设置P14为带上拉IO
#define	 P15IH	 (BC(P1CON,5),BS(P1PH,5))	 //设置P15为带上拉IO
#define	 P16IH	 (BC(P1CON,6),BS(P1PH,6))	 //设置P16为带上拉IO
#define	 P17IH	 (BC(P1CON,7),BS(P1PH,7))	 //设置P17为带上拉IO

#define	 P20IH	 (BC(P2CON,0),BS(P2PH,0))	 //设置P20为带上拉IO
#define	 P21IH	 (BC(P2CON,1),BS(P2PH,1))	 //设置P21为带上拉IO
#define	 P22IH	 (BC(P2CON,2),BS(P2PH,2))	 //设置P22为带上拉IO
#define	 P23IH	 (BC(P2CON,3),BS(P2PH,3))	 //设置P23为带上拉IO
#define	 P24IH	 (BC(P2CON,4),BS(P2PH,4))	 //设置P24为带上拉IO
#define	 P25IH	 (BC(P2CON,5),BS(P2PH,5))	 //设置P25为带上拉IO
#define	 P26IH	 (BC(P2CON,6),BS(P2PH,6))	 //设置P26为带上拉IO
#define	 P27IH	 (BC(P2CON,7),BS(P2PH,7))	 //设置P27为带上拉IO

#define	 P50IH	 (BC(P5CON,0),BS(P5PH,0))	 //设置P50为带上拉IO
#define	 P51IH	 (BC(P5CON,1),BS(P5PH,1))	 //设置P51为带上拉IO

#endif
