
#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"
#include "bitband.h"	//ʹ��λ��������һ��Ҫ������ͷ�ļ�

#define LED1		GPIOout(GPIOB, 12)	//����PB12λ������
#define LED2		GPIOout(GPIOB, 13)	//����PB13λ������
#define LED3		GPIOout(GPIOB, 14)	//����PB14λ������
#define LED4		GPIOout(GPIOB, 15)	//����PB15λ������

#define SYS_LED		GPIOout(GPIOB, 1)	//����PB1λ������

void sysled_init(void);				//ϵͳ��
void display_led_init(void);	//LED IO��ʼ��
void display_led_on(uint8_t index);		//LED�����ž��
void power_led_init(void);			//�м�power��
void IndicatorLed(uint32_t tick);

#endif

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
