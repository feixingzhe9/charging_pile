#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "remote.h"
#include "switch.h"
#include "global.h"
#include "tools.h"
#include "charge.h"
#include "led.h"

int main(void)
{
    uint32_t cnt = 0;
	Delay_Init();			//��ʱ��ʼ��
	
	TIM1_CH1N_PWM_Init(1895,0);				//72000/(1895+1) = 37.99K		power_LED��ʼ��
	
	COM_Init(COM1, 115200);						//����1��ʼ��
	COM1_DMA_init(115200);
	
	Timer1_Init(9999, 7199);				//�����жϷ�ʱ������Ƶ�� = 72000/(7199+1) = 10kHz��������(9999+1)Ϊ1s//ok
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�		
	
	TIM4_CH4_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//��
	
	TIM4_CH3_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//��	
	
	Remote_Init();			//������ճ�ʼ��	
	
	swtich_init();
	
	sysled_init();
	display_led_init();
	display_led_on(0);
	
	charge_ctl_init();
	charge_ctl_off();
	
	while(1)
	{
		delay_ms(10);
		cnt++;
		update_status();													//��������״̬
		deal_with_status();												//��ͬ״̬�µĿ��ƴ���
		deal_with_light();												//��ͬ״̬�µĵƹ⴦��
        IndicatorLed(cnt);
	}

}

