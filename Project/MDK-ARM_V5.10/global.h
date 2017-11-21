#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "stm32f10x.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"

//power_state
#define POWER_ON						1		//正在供电
#define POWER_OFF						0		//停止供电

//timer_1s_flag
#define TIME_UP							1		//时间到
#define TIME_NOT_UP					0		//时间未到

//switch_flag
#define SWITCH_ON						0		//光电开关均触发
#define SWITCH1_ON					1		//光电开关1触发
#define SWITCH2_ON					2		//光电开关2触发
#define SWITCH_OFF					3		//光电开关均未触发

//err_flag
#define	ERR_NONE								0			//无错误
#define	ERR_TIMEOUT							1			//通讯超时
#define	ERR_SWITCH							2			//光电开关读值出错

//sys_status
#define STATUS_WAITING					0			//等待充电态
#define STATUS_PLUS							1			//正在充电态
#define STATUS_FULL							2			//充满态
#define STATUS_ERR							3			//错误态

extern uint8_t sys_status;
extern uint8_t sys_power;
extern uint8_t timer_1s_flag;
extern uint8_t status_1s_flag;
extern uint8_t light_1s_flag;
extern uint8_t err_state;
	
#endif
