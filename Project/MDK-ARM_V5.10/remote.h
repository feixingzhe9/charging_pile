#ifndef __RED_H
#define __RED_H 
#include "stm32f10x.h"   
#include "bitband.h"
#include "usart.h"
#include "tools.h"

#define RDATA GPIOin(GPIOB,7)	 //�������������

#define START 					0
#define STRAIGHT 				1
#define TURN_LEFT				2
#define TURN_RIGHT			3
#define TURN_WHIRL			4

//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
#define REMOTE_ID1 						0x59    		//left
#define REMOTE_ID2 						0x16 				//right

#define REMOTE_NONE 					0x00 		   	//δ���յ�����
#define REMOTE_ID_POWER_33 		0xcc    		//��������33%
#define REMOTE_ID_POWER_66 		0x66 		   	//��������66%
#define REMOTE_ID_POWER_99 		0x99    		//��������99%
#define REMOTE_ID_POWER_100 	0xFF 		   	//������

#define REMOTE_ID_POWER_ON 		0x5A    		//�ϵ�
#define REMOTE_ID_POWER_OFF 	0xA5 		   	//�µ�

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
extern uint8_t RmtCnt;	//�������µĴ���

void Remote_Init(void);    //���⴫��������ͷ���ų�ʼ��
u8 Remote_Scan(void);	  
u8 remote_calculate(u8 scan_value);
#endif















