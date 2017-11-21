/*******************************************************************************
 * @name    : LED���������ļ�
 * @author  : ������
 * @web     : WWW.UCORTEX.COM
 * @version : V1.1
 * @date    : 2014-04-03
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : ����STM32 PB12��Ϊ����ڣ���������LEDָʾ��
 * ---------------------------------------------------------------------------- 
 * @copyright
 *
 * UCORTEX��Ȩ���У�Υ�߱ؾ�������Դ�������Ҳο���ּ�ڽ���ѧϰ�ͽ�ʡ����ʱ�䣬
 * ������Ϊ�ο����ļ����ݣ��������Ĳ�Ʒֱ�ӻ����ܵ��ƻ������漰���������⣬��
 * �߲��е��κ����Ρ�����ʹ�ù����з��ֵ����⣬���������WWW.UCORTEX.COM��վ��
 * �������ǣ����ǻ�ǳ���л�����������⼰ʱ�о����������ơ����̵İ汾���£�����
 * ���ر�֪ͨ���������е�WWW.UCORTEX.COM�������°汾��лл��
 * ��������������UCORTEX������һ�����͵�Ȩ����
 * ----------------------------------------------------------------------------
 * @description
 * LED -> PB12
 *-----------------------------------------------------------------------------
 * @history
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-04-03    �����ˣ�������
 * �汾��¼��V1.0
 * �������ݣ��½�
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-04-03    �����ˣ�������
 * �汾��¼��V1.1
 * �������ݣ���Ϊλ������ģʽ
 * ----------------------------------------------------------------------------
 *
 ******************************************************************************/
#include "led.h"


void sysled_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
       
  /* ʹ��GPIOBʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /* ����GPIOB.12Ϊ�������ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
//	GPIO_SetBits(GPIOB, GPIO_Pin_1);//����GPIOB.12Ϊ�ߵ�ƽ���ر�LED
	SYS_LED = 0;
}

/**
  * @brief  LED IO��ʼ��
  * @param  None
  * @retval None
	* @note   LED���ӵ�GPIOB.12���͵�ƽ�����ߵ�ƽ��
  */
void display_led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
       
  /* ʹ��GPIOBʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /* ����GPIOB.12Ϊ�������ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);//����GPIOB.12Ϊ�ߵ�ƽ���ر�LED
	GPIO_ResetBits(GPIOB, GPIO_Pin_13);//����GPIOB.13Ϊ�ߵ�ƽ���ر�LED
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);//����GPIOB.14Ϊ�ߵ�ƽ���ر�LED
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);//����GPIOB.15Ϊ�ߵ�ƽ���ر�LED
}

/**
  * @brief  ��������LED
  * @param  None
  * @retval None
	* @note   LED���ӵ�GPIOB.12���͵�ƽ�����ߵ�ƽ��
  */
void display_led_on(uint8_t index)
{
	switch(index)
	{
		case 0:
			LED1 = 0;			
			LED2 = 0;		
			LED3 = 0;		
			LED4 = 0;		
			break;
		
		case 1:
			LED1 = 1;			
			LED2 = 0;		
			LED3 = 0;		
			LED4 = 0;	
			break;
		
		case 2:
			LED1 = 1;			
			LED2 = 1;		
			LED3 = 0;		
			LED4 = 0;	
			break;
		
		case 3:
			LED1 = 1;			
			LED2 = 1;		
			LED3 = 1;		
			LED4 = 0;	
			break;
		
		case 4:
			LED1 = 1;			
			LED2 = 1;		
			LED3 = 1;		
			LED4 = 1;	
			break;
	}
}

void power_led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
       
  /* ʹ��GPIOBʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /* ����GPIOB.12Ϊ�������ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_7);//����GPIOA.7Ϊ�ߵ�ƽ���ر�LED
}
/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
