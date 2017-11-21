
#include "stm32f10x.h"
#include "bitband.h"	//ʹ��λ������ǰ���Ȱ�����ͷ�ļ�
#include "global.h"

#define SWITCH_VALUE_NUM		3

#define SWITCH1_SCAN		GPIOin(GPIOA, 0)	
#define SWITCH2_SCAN		GPIOin(GPIOA, 1)	

#define SWITCH_IN			0
#define SWITCH_OUT		1

#define MOTOR1_DIR		GPIOout(GPIOB, 7)	//����PB12λ������

void swtich_init(void);

uint8_t switch_scan(void);
