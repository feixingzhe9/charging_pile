
#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"
#include "bitband.h"	//使用位带操作，一定要包含此头文件

#define LED1		GPIOout(GPIOB, 12)	//定义PB12位带操作
#define LED2		GPIOout(GPIOB, 13)	//定义PB13位带操作
#define LED3		GPIOout(GPIOB, 14)	//定义PB14位带操作
#define LED4		GPIOout(GPIOB, 15)	//定义PB15位带操作

#define SYS_LED		GPIOout(GPIOB, 1)	//定义PB1位带操作

void sysled_init(void);				//系统灯
void display_led_init(void);	//LED IO初始化
void display_led_on(uint8_t index);		//LED亮几颗就填几
void power_led_init(void);			//中间power灯
void IndicatorLed(uint32_t tick);

#endif

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
