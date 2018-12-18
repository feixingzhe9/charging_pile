#include "tools.h"

#define ID_FILTER_SIZE  3
uint8_t id_arry[ID_FILTER_SIZE] = {0};
static uint8_t ID_filter(uint8_t get_ID)
{
	uint8_t i;
	uint8_t id_value;
//	uint8_t level_1 = 0;
//	uint8_t level_2 = 0;
//	uint8_t level_3 = 0;
//	uint8_t level_4= 0;
//    uint8_t level_5= 0;
    uint8_t level[5] = {0};
    
    uint8_t max = 0; 
    uint8_t index = 0; 
	
	for(i = 0; i < ID_FILTER_SIZE - 1; i++)
	{
		id_arry[i] = id_arry[i+1];
	}
	id_arry[ID_FILTER_SIZE - 1] = get_ID;
	
	for(i = 0; i < ID_FILTER_SIZE; i++)
	{
		switch(id_arry[i])
		{
			case REMOTE_ID_POWER_1:
				level[0]++;
				break;
			
			case REMOTE_ID_POWER_2:
				level[1]++;
				break;
			
			case REMOTE_ID_POWER_3:
				level[2]++;
			break;
	
			case REMOTE_ID_POWER_4:
				level[3]++;
			break;
            
            case REMOTE_ID_POWER_MAX:
				level[4]++;
			break;
			
			default:
				break;
		}
	}

    for (i = 0; i < 5; i++) 
    {
        if(level[i] > max)
        {
            max = level[i];
            index = i;
        }
    }

//    if(level[index] > 3)
    {
        switch(index)
        {
            case 0:
                id_value = REMOTE_ID_POWER_1;
                break;
            case 1:
                id_value = REMOTE_ID_POWER_2;
                break;
            case 2:
                id_value = REMOTE_ID_POWER_3;
                break;
            case 3:
                id_value = REMOTE_ID_POWER_4;
                break;
            case 4:
                id_value = REMOTE_ID_POWER_MAX;
                break;
        }
    }
//    else
//    {
//        id_value = 0;
//    }
	return id_value;
}

static void clear_id_array(void)
{
    uint8_t i;
    for(i = 0; i < ID_FILTER_SIZE; i++)
    {
        id_arry[i] = 0;
    }
}

void update_status(void)
{
	uint8_t light_ID = 0;
    uint8_t light_ID_tmp = 0;
	uint8_t switch_status;
    uint32_t adc_data = 0;
    static uint16_t adc_data_tmp[10] = {0};
    static uint8_t adc_index = 0;
    static uint32_t get_remote_id_time_out_cnt = 0;
    uint8_t i = 0;
    
//	static uint8_t time_out = 0;
	
	switch_status = switch_scan();										//获取光电开关读值
//	printf("%d\r\n",switch_status);
    
	switch(switch_status)
	{
		case SWITCH_ON:
			light_ID_tmp = Remote_Scan();													//获取红外读值
            get_remote_id_time_out_cnt++;
			if(light_ID_tmp != 0)
			{
//                printf("get remote id = %d\r\n", light_ID);
				light_ID = ID_filter(light_ID_tmp);
                printf("remote id 0x%x, after filter: 0x%x\r\n", light_ID_tmp, light_ID);
				if(light_ID == REMOTE_ID_POWER_MAX)
				{
					sys_status = STATUS_FULL;
					printf("sys_status = STATUS_FULL\r\n");
					sys_power = REMOTE_ID_POWER_MAX;
				}
				else
				{
					sys_status = STATUS_PLUS;
					sys_power = light_ID;
				}
//				time_out = 0;
                get_remote_id_time_out_cnt = 0;
			}
			else
			{
                if(get_remote_id_time_out_cnt >= 3000)
                {
//                    sys_power = REMOTE_NONE;
//					sys_status = STATUS_ERR;
//                    err_state = ERR_TIMEOUT;
                }
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
            clear_id_array();
			break;
		
		case SWITCH_OFF:
			sys_power = REMOTE_NONE;
			sys_status = STATUS_WAITING;
            clear_id_array();
//			printf("sys_status = STATUS_WAITING\r\n");
			break;
		
		default:
			break;
	}
    
    
    adc_data_tmp[adc_index] = Get_Adc(ADC1,4);
    if(adc_index < sizeof(adc_data_tmp)/sizeof(adc_data_tmp[0]) - 1)
    {
        adc_index++;
    }
    else
    {
        adc_index = 0;
    }
    for(i = 0; i < sizeof(adc_data_tmp)/sizeof(adc_data_tmp[0]); i++)
    {
        adc_data += adc_data_tmp[i];
    }
    adc_data = adc_data / (sizeof(adc_data_tmp)/sizeof(adc_data_tmp[0]));
    if(adc_data > 30)
    {
        FAN_CTRL = 1;
    }
    if(adc_data < 15)
    {
        FAN_CTRL = 0;
    }
}

void deal_with_status(void)
{
	static uint8_t i_cnt = 0;
    static uint8_t test_cnt = 0;
	switch(sys_status)
	{
		case STATUS_WAITING:
			charge_ctl_off();//关闭充电
			TIM4_CH4_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//右
			TIM4_CH3_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//左	
			if(i_cnt == 0)
			{	
				SendData1(REMOTE_ID1,REMOTE_ID1);
			}
			if(i_cnt == 4)
			{
				SendData2(REMOTE_ID2,REMOTE_ID2);
			}
			Remote_Init();			//红外接收初始化	
			i_cnt = (i_cnt+1)%8;

            if(timer_1s_flag == TIME_UP)
			{
				timer_1s_flag = TIME_NOT_UP;
                printf("STATUS_WAITING . . .\r\n");
            }
			break;
		
		case STATUS_PLUS:
			charge_ctl_on();//打开充电
			if(timer_1s_flag == TIME_UP)
			{
//				printf("TIME_UP\r\n");
				timer_1s_flag = TIME_NOT_UP;
				TIM4_CH4_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//右
				TIM4_CH3_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//左	
				SendData1(REMOTE_ID_POWER_ON,REMOTE_ID_POWER_ON);
                if(test_cnt++ % 2 == 0)
                {
                    delay_ms(50);
                    Remote_Init();
                }
                printf("STATUS_PLUS, charging . . .\r\n");
			}
			break;
		
		case STATUS_FULL:
			charge_ctl_on();//任然打开充电
			if(timer_1s_flag == TIME_UP)
			{
				timer_1s_flag = TIME_NOT_UP;
				
				TIM4_CH4_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//右
				TIM4_CH3_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K			//左	
				SendData1(REMOTE_ID_POWER_OFF,REMOTE_ID_POWER_OFF);
				if(test_cnt++ % 2 == 0)
                {
                    delay_ms(50);
                    Remote_Init();
                }
                printf("STATUS_FULL, charging full \r\n");
			}
			break;
		
		case STATUS_ERR:
			charge_ctl_off();//关闭充电
            if(timer_1s_flag == TIME_UP)
			{
				timer_1s_flag = TIME_NOT_UP;
                if(test_cnt++ % 2 == 0)
                {
                    delay_ms(50);
                    Remote_Init();
                }
                printf("STATUS_ERR: switch status is %d \r\n", switch_scan());
                if(switch_scan() == SWITCH_ON)
                {
                    printf("Switches are ALL ON, but CAN NOT get right remote id ! ! ! \r\n");
                }
            }
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
			power_ligh_style(STATUS_WAITING);//电源灯变色
			break;
		
		case STATUS_PLUS:
			if(light_1s_flag == TIME_UP)
			{
				light_1s_flag = TIME_NOT_UP;
#if 0               
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
#else
                led_num++;
                switch(sys_power)
				{
					case REMOTE_ID_POWER_1:
						display_led_on((led_num)%2);
//						printf("33\r\n");
						break;
					
					case REMOTE_ID_POWER_2:
						display_led_on((led_num)%2 + 1);
//						printf("66\r\n");
						break;
					
					case REMOTE_ID_POWER_3:
						display_led_on((led_num)%2 + 2);
//						printf("99\r\n");
						break;
                    case REMOTE_ID_POWER_4:
						display_led_on((led_num)%2 + 3);
//						printf("99\r\n");
						break;
                    case REMOTE_ID_POWER_MAX:
						display_led_on(4);
//						printf("99\r\n");
						break;
				}
//                led_num++;
//                if(sys_power < 25)
//                {
//                    display_led_on((led_num)%2);
//                }
//                else if(sys_power < 50) 
//                {
//                    display_led_on((led_num)%2 + 1);
//                }
//                else if(sys_power < 75)
//                {
//                    display_led_on((led_num)%2 + 2);
//                }
//                else if(sys_power < 100)
//                {
//                    display_led_on((led_num)%2 + 3);
//                }
//                else if(sys_power == 100)
//                {
//                    display_led_on(4);
//                }
#endif               
			}
			power_ligh_style(STATUS_PLUS);//电源灯变色
			break;
		
		case STATUS_FULL:
			display_led_on(4);
			power_ligh_style(STATUS_FULL);//电源灯变色
			break;
		
		case STATUS_ERR:
			display_led_on(0);
			power_ligh_style(STATUS_ERR);//电源灯变色
			break;
		
		default:
			break;
	}	
}


