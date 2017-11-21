#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "stm32f10x.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"

//power_state
#define POWER_ON						1		//���ڹ���
#define POWER_OFF						0		//ֹͣ����

//timer_1s_flag
#define TIME_UP							1		//ʱ�䵽
#define TIME_NOT_UP					0		//ʱ��δ��

//switch_flag
#define SWITCH_ON						0		//��翪�ؾ�����
#define SWITCH1_ON					1		//��翪��1����
#define SWITCH2_ON					2		//��翪��2����
#define SWITCH_OFF					3		//��翪�ؾ�δ����

//err_flag
#define	ERR_NONE								0			//�޴���
#define	ERR_TIMEOUT							1			//ͨѶ��ʱ
#define	ERR_SWITCH							2			//��翪�ض�ֵ����

//sys_status
#define STATUS_WAITING					0			//�ȴ����̬
#define STATUS_PLUS							1			//���ڳ��̬
#define STATUS_FULL							2			//����̬
#define STATUS_ERR							3			//����̬

extern uint8_t sys_status;
extern uint8_t sys_power;
extern uint8_t timer_1s_flag;
extern uint8_t status_1s_flag;
extern uint8_t light_1s_flag;
extern uint8_t err_state;
	
#endif
