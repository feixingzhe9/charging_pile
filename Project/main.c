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
	Delay_Init();			//延时初始化
	
	TIM1_CH1N_PWM_Init(1895,0);				//72000/(1895+1) = 37.99K		power_LED初始化
	
	COM_Init(COM1, 115200);						//串口1初始化
	COM1_DMA_init(115200);
	
	Timer1_Init(9999, 7199);				//用于中断分时，计数频率 = 72000/(7199+1) = 10kHz，计数到(9999+1)为1s//ok
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级		
	
	TIM4_CH4_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//右
	
	TIM4_CH3_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//左	
	
	Remote_Init();			//红外接收初始化	
	
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
		update_status();													//更新所有状态
		deal_with_status();												//不同状态下的控制处理
		deal_with_light();												//不同状态下的灯光处理
        IndicatorLed(cnt);
	}

}

