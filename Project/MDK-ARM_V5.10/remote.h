#ifndef __RED_H
#define __RED_H 
#include "stm32f10x.h"   
#include "bitband.h"
#include "usart.h"
#include "tools.h"

#define RDATA GPIOin(GPIOB,7)	 //红外数据输入脚

#define START 					0
#define STRAIGHT 				1
#define TURN_LEFT				2
#define TURN_RIGHT			3
#define TURN_WHIRL			4

//红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
//我们选用的遥控器识别码为0
#define REMOTE_ID1 						0x59    		//left
#define REMOTE_ID2 						0x16 				//right

#define REMOTE_NONE 					0x00 		   	//未接收到电量
#define REMOTE_ID_POWER_33 		0xcc    		//电量低于33%
#define REMOTE_ID_POWER_66 		0x66 		   	//电量低于66%
#define REMOTE_ID_POWER_99 		0x99    		//电量低于99%
#define REMOTE_ID_POWER_100 	0xFF 		   	//电量满

#define REMOTE_ID_POWER_ON 		0x5A    		//上电
#define REMOTE_ID_POWER_OFF 	0xA5 		   	//下电

#define REDUCE_RANGE_1		400
#define REDUCE_RANGE_2		200

#define NAVIGATION_1   	1
#define NAVIGATION_2	 	2
#define NAVIGATION_3	 	3

#define V_1				140//120
#define W_1				140//120//100

#define V_2				60//70
#define W_2				40

#define V_3				20//30
#define W_3				10//15

extern uint8_t navigation_mode;
extern uint8_t RmtCnt;	//按键按下的次数

void Remote_Init(void);    //红外传感器接收头引脚初始化
u8 Remote_Scan(void);	  
u8 remote_calculate(u8 scan_value);
#endif















