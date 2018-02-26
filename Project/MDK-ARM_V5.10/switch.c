#include "switch.h"

/**
  * @brief  ����IO��ʼ��
  * @param  None
  * @retval None
	* @note   �������ӵ�GPIOA.0���͵�ƽ�����ߵ�ƽ��
  */
static void switch1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
       
  /* ʹ��GPIOAʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

  /* ����GPIOA.00Ϊ�������� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  ����IO��ʼ��
  * @param  None
  * @retval None
	* @note   �������ӵ�GPIOA.0���͵�ƽ�����ߵ�ƽ��
  */
static void switch2_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
       
  /* ʹ��GPIOAʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

  /* ����GPIOA.00Ϊ�������� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void swtich_init(void)
{
	switch1_init();
	switch2_init();
}

static uint8_t switch1_scan(void)
{
	uint8_t switch1_value;
	uint8_t switch1_state = 0;
	static uint8_t switch1_value_buff[SWITCH_VALUE_NUM] = {SWITCH_OUT};
	uint8_t i;
	
	for(i=0;i<SWITCH_VALUE_NUM-1;i++)
	{
		switch1_value_buff[i] = switch1_value_buff[i+1];
	}
	switch1_value_buff[SWITCH_VALUE_NUM-1] = SWITCH1_SCAN;
	
	for(i=0;i<SWITCH_VALUE_NUM-1;i++)
	{
		if(switch1_value_buff[i] != switch1_value_buff[i+1])
		{
			switch1_state = 1;
		}
	}	
	if(switch1_state == 0)
	{
		switch1_value = switch1_value_buff[0];
	}
//	switch1_value = SWITCH1_SCAN;
	return switch1_value;
}

static uint8_t switch2_scan(void)
{
	uint8_t switch2_value;
	uint8_t switch2_state = 0;
	static uint8_t switch2_value_buff[SWITCH_VALUE_NUM] = {SWITCH_OUT};
	uint8_t i;
	
	for(i=0;i<SWITCH_VALUE_NUM-1;i++)
	{
		switch2_value_buff[i] = switch2_value_buff[i+1];
	}
	switch2_value_buff[SWITCH_VALUE_NUM-1] = SWITCH2_SCAN;
	
	for(i=0;i<SWITCH_VALUE_NUM-1;i++)
	{
		if(switch2_value_buff[i] != switch2_value_buff[i+1])
		{
			switch2_state = 1;
		}
	}	
	if(switch2_state == 0)
	{
		switch2_value = switch2_value_buff[0];
	}
//	switch2_value = SWITCH2_SCAN;
	return switch2_value;
}

uint8_t switch_scan(void)
{
	uint8_t switch1_value;
	uint8_t switch2_value;
	switch1_value = switch1_scan();
	switch2_value = switch2_scan();
    
    //return SWITCH_ON ;//test code
    
    
	if((SWITCH_OUT == switch1_value)&&(SWITCH_OUT == switch2_value))
	{
		return SWITCH_ON ;
	}
	if((SWITCH_OUT != switch1_value)&&(SWITCH_OUT == switch2_value))
	{
		return SWITCH1_ON ;
	}
	if((SWITCH_OUT == switch1_value)&&(SWITCH_OUT != switch2_value))
	{
		return SWITCH2_ON ;
	}
	if((SWITCH_OUT != switch1_value)&&(SWITCH_OUT != switch2_value))
	{
		return SWITCH_OFF ;
	}
	return 4;
}
