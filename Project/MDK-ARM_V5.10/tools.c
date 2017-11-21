#include "tools.h"

static uint8_t ID_filter(uint8_t get_ID)
{
	static uint8_t id_arry[10] = {0};
	uint8_t i;
	uint8_t id_value;
	uint8_t num_33 = 0;
	uint8_t num_66 = 0;
	uint8_t num_99 = 0;
	uint8_t num_100= 0;
	
	for(i=0;i<9;i++)
	{
		id_arry[i] = id_arry[i+1];
	}
	id_arry[9] = get_ID;
	
	for(i=0;i<10;i++)
	{
		switch(id_arry[i])
		{
			case REMOTE_ID_POWER_33:
				num_33++;
				break;
			
			case REMOTE_ID_POWER_66:
				num_66++;
				break;
			
			case REMOTE_ID_POWER_99:
				num_99++;
			break;
	
			case REMOTE_ID_POWER_100:
				num_100++;
			break;
			
			default:
				break;
		}
	}
	
	if(num_33 > num_66)
	{
		if(num_33 > num_99)
		{
			if(num_33 > num_100)
			{
				id_value = REMOTE_ID_POWER_33;
			}
			else
			{
				id_value = REMOTE_ID_POWER_100;
			}
		}
		else
		{
			if(num_99 > num_100)
			{
				id_value = REMOTE_ID_POWER_99;
			}
			else
			{
				id_value = REMOTE_ID_POWER_100;
			}
		}	
	}
	else
	{
		if(num_66 > num_99)
		{
			if(num_66 > num_100)
			{
				id_value = REMOTE_ID_POWER_66;
			}
			else
			{
				id_value = REMOTE_ID_POWER_100;
			}
		}
		else
		{
			if(num_99 > num_100)
			{
				id_value = REMOTE_ID_POWER_99;
			}
			else
			{
				id_value = REMOTE_ID_POWER_100;
			}
		}	
	}
	return id_value;
}

void update_status(void)
{
	uint8_t light_ID = 0;
	uint8_t switch_status;
//	static uint8_t time_out = 0;
	
	switch_status = switch_scan();										//��ȡ��翪�ض�ֵ
//	printf("%d\r\n",switch_status);
	switch(switch_status)
	{
		case SWITCH_ON:
			light_ID = Remote_Scan();													//��ȡ�����ֵ
			if(light_ID != 0)
			{
				light_ID = ID_filter(light_ID);
				if(light_ID == REMOTE_ID_POWER_100)
				{
					sys_status = STATUS_FULL;
//					printf("sys_status = STATUS_FULL\r\n");
					sys_power = REMOTE_ID_POWER_100;
				}
				else
				{
					sys_status = STATUS_PLUS;
					sys_power = light_ID;
				}
//				time_out = 0;
			}
			else
			{
				if((sys_status != STATUS_FULL)&&(sys_status != STATUS_PLUS))
				{
					sys_power = REMOTE_NONE;
					sys_status = STATUS_ERR;
					err_state = ERR_TIMEOUT;
				}
//				if(time_out > 20)
//				{
//					sys_power = REMOTE_NONE;
//					sys_status = STATUS_ERR;
//					err_state = ERR_TIMEOUT;
////					printf("sys_status = STATUS_ERR1\r\n");	
//					time_out = 0;
//				}
//				if(status_1s_flag == TIME_UP)
//				{
//					status_1s_flag = TIME_NOT_UP;
//					time_out++;
//				}
			}
			break;		
		
		case SWITCH1_ON:
		case SWITCH2_ON:
			sys_power = REMOTE_NONE;
			sys_status = STATUS_ERR;
			err_state = ERR_SWITCH;
//			printf("sys_status = STATUS_ERR2\r\n");	
			break;
		
		case SWITCH_OFF:
			sys_power = REMOTE_NONE;
			sys_status = STATUS_WAITING;
//			printf("sys_status = STATUS_WAITING\r\n");
			break;
		
		default:
			break;
	}
}

void deal_with_status(void)
{
	static uint8_t i_cnt = 0;
	switch(sys_status)
	{
		case STATUS_WAITING:
			charge_ctl_off();//�رճ��
			TIM4_CH4_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//��
			TIM4_CH3_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//��	
			if(i_cnt == 0)
			{	
				SendData1(REMOTE_ID1,REMOTE_ID1);
			}
			if(i_cnt == 4)
			{
				SendData2(REMOTE_ID2,REMOTE_ID2);
			}
			Remote_Init();			//������ճ�ʼ��	
			i_cnt = (i_cnt+1)%8;
			break;
		
		case STATUS_PLUS:
			charge_ctl_on();//�򿪳��
			if(timer_1s_flag == TIME_UP)
			{
//				printf("TIME_UP\r\n");
				timer_1s_flag = TIME_NOT_UP;
				TIM4_CH4_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//��
				TIM4_CH3_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//��	
				SendData1(REMOTE_ID_POWER_ON,REMOTE_ID_POWER_ON);
				Remote_Init();			//������ճ�ʼ��	
			}
			break;
		
		case STATUS_FULL:
			charge_ctl_on();//��Ȼ�򿪳��
			if(timer_1s_flag == TIME_UP)
			{
				timer_1s_flag = TIME_NOT_UP;
				
				TIM4_CH4_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//��
				TIM4_CH3_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//��	
				SendData1(REMOTE_ID_POWER_OFF,REMOTE_ID_POWER_OFF);
				Remote_Init();			//������ճ�ʼ��			
			}
			break;
		
		case STATUS_ERR:
			charge_ctl_off();//�رճ��
			break;
		
		default:
			break;
	}
}

static void power_ligh_style(uint8_t status)
{
	static uint8_t power_i=0;
	static uint8_t prev = 0;
	static uint8_t cnt = 0;
	switch(status)
	{
		case STATUS_WAITING:
			power_i = (power_i+1)%30 ;
			PWM_SetDuty3(power_i+5);
			break;
		
		case STATUS_PLUS:
			PWM_SetDuty3(10);
			break;
		
		case STATUS_FULL:
			PWM_SetDuty3(10);
			break;
		
		case STATUS_ERR:
			if(light_1s_flag == TIME_UP)
			{
				light_1s_flag = TIME_NOT_UP;
				cnt = (cnt+1)%err_state;
				if(cnt == 0)
				{
					cnt = 0;
					if(prev == 0)
					{
						prev = 1;
						PWM_SetDuty3(20);
					}
					else
					{
						prev = 0;
						PWM_SetDuty3(0);
					}
				}
			}
			break;
	}
}

void deal_with_light(void)
{
	static uint8_t led_num;
	
	switch(sys_status)
	{
		case STATUS_WAITING:
			display_led_on(0);
			power_ligh_style(STATUS_WAITING);//��Դ�Ʊ�ɫ
			break;
		
		case STATUS_PLUS:
			if(light_1s_flag == TIME_UP)
			{
				light_1s_flag = TIME_NOT_UP;
				switch(sys_power)
				{
					case REMOTE_ID_POWER_33:
						led_num = (led_num+1)%4;
						display_led_on(led_num+1);
//						printf("33\r\n");
						break;
					
					case REMOTE_ID_POWER_66:
						led_num = (led_num+1)%3;
						display_led_on(led_num+2);
//						printf("66\r\n");
						break;
					
					case REMOTE_ID_POWER_99:
						led_num = (led_num+1)%2;
						display_led_on(led_num+3);
//						printf("99\r\n");
						break;
				}
			}
			power_ligh_style(STATUS_PLUS);//��Դ�Ʊ�ɫ
			break;
		
		case STATUS_FULL:
			display_led_on(4);
			power_ligh_style(STATUS_FULL);//��Դ�Ʊ�ɫ
			break;
		
		case STATUS_ERR:
			display_led_on(0);
			power_ligh_style(STATUS_ERR);//��Դ�Ʊ�ɫ
			break;
		
		default:
			break;
	}	
}


/*
void deal_with_status(void)
{
	static uint8_t i_cnt = 0;
	static uint8_t led_num;
	switch(sys_status)
	{
		case STATUS_WAITING:
			//�رճ��
			TIM4_CH4_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//��
			TIM4_CH3_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//��	
			if(i_cnt == 0)
			{	
				SendData1(REMOTE_ID1,REMOTE_ID1);
			}
			if(i_cnt == 4)
			{
				SendData2(REMOTE_ID2,REMOTE_ID2);
			}
			Remote_Init();			//������ճ�ʼ��	
			i_cnt = (i_cnt+1)%8;
			display_led_on(0);
			break;
		
		case STATUS_PLUS:
			//�򿪳��
			if(timer_1s_flag == TIME_UP)
			{
//				printf("TIME_UP\r\n");
				timer_1s_flag = TIME_NOT_UP;
				TIM4_CH4_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//��
				TIM4_CH3_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//��	
				SendData1(REMOTE_ID_POWER_ON,REMOTE_ID_POWER_ON);
				Remote_Init();			//������ճ�ʼ��	
				switch(sys_power)
				{
					case REMOTE_ID_POWER_33:
						led_num = (led_num+1)%4;
						display_led_on(led_num+1);
//						printf("33\r\n");
						break;
					
					case REMOTE_ID_POWER_66:
						led_num = (led_num+1)%3;
						display_led_on(led_num+2);
//						printf("66\r\n");
						break;
					
					case REMOTE_ID_POWER_99:
						led_num = (led_num+1)%2;
						display_led_on(led_num+3);
//						printf("99\r\n");
						break;
				}
				
			}
			break;
		
		case STATUS_FULL:
			//�رճ��
			display_led_on(4);
			if(timer_1s_flag == TIME_UP)
			{
				timer_1s_flag = TIME_NOT_UP;
				
				TIM4_CH4_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//��
				TIM4_CH3_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//��	
				SendData1(REMOTE_ID_POWER_OFF,REMOTE_ID_POWER_OFF);
				Remote_Init();			//������ճ�ʼ��			
			}
			break;
		
		case STATUS_ERR:
			//�رճ��
			display_led_on(0);
			//��Դ�Ʊ�ɫ
			break;
		
		default:
			break;
	}
}
*/