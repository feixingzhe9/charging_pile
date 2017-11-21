#ifndef __CHARGE_H
#define __CHARGE_H
#include "stm32f10x.h"
#include "bitband.h"	//使用位带操作，一定要包含此头文件

#define CHARGE		GPIOout(GPIOA, 2)	

void charge_ctl_init(void);			//充电控制初始化
void charge_ctl_on(void);				//开启充电
void charge_ctl_off(void);				//关闭充电

#endif
