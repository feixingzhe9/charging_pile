#ifndef __CHARGE_H
#define __CHARGE_H
#include "stm32f10x.h"
#include "bitband.h"	//ʹ��λ��������һ��Ҫ������ͷ�ļ�

#define CHARGE		GPIOout(GPIOA, 2)	

void charge_ctl_init(void);			//�����Ƴ�ʼ��
void charge_ctl_on(void);				//�������
void charge_ctl_off(void);				//�رճ��

#endif
