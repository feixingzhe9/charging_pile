/*******************************************************************************
 * @name    : LED控制驱动文件
 * @author  : 布谷鸟
 * @web     : WWW.UCORTEX.COM
 * @version : V1.1
 * @date    : 2014-04-03
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : 配置STM32 PB12作为输出口，用来控制LED指示灯
 * ---------------------------------------------------------------------------- 
 * @copyright
 *
 * UCORTEX版权所有，违者必究！例程源码仅供大家参考，旨在交流学习和节省开发时间，
 * 对于因为参考本文件内容，导致您的产品直接或间接受到破坏，或涉及到法律问题，作
 * 者不承担任何责任。对于使用过程中发现的问题，如果您能在WWW.UCORTEX.COM网站反
 * 馈给我们，我们会非常感谢，并将对问题及时研究并作出改善。例程的版本更新，将不
 * 做特别通知，请您自行到WWW.UCORTEX.COM下载最新版本，谢谢。
 * 对于以上声明，UCORTEX保留进一步解释的权利！
 * ----------------------------------------------------------------------------
 * @description
 * LED -> PB12
 *-----------------------------------------------------------------------------
 * @history
 * ----------------------------------------------------------------------------
 * 更改时间：2014-04-03    更改人：布谷鸟
 * 版本记录：V1.0
 * 更改内容：新建
 * ----------------------------------------------------------------------------
 * 更改时间：2014-04-03    更改人：布谷鸟
 * 版本记录：V1.1
 * 更改内容：改为位带操作模式
 * ----------------------------------------------------------------------------
 *
 ******************************************************************************/
#include "led.h"
#include "global.h"


void sysled_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
       
  /* 使能GPIOB时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /* 配置GPIOB.12为推挽输出模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
//	GPIO_SetBits(GPIOB, GPIO_Pin_1);//设置GPIOB.12为高电平，关闭LED
	SYS_LED = 0;
}

/**
  * @brief  LED IO初始化
  * @param  None
  * @retval None
	* @note   LED链接到GPIOB.12，低电平亮，高电平灭
  */
void display_led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
       
  /* 使能GPIOB时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /* 配置GPIOB.12为推挽输出模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);//设置GPIOB.12为高电平，关闭LED
	GPIO_ResetBits(GPIOB, GPIO_Pin_13);//设置GPIOB.13为高电平，关闭LED
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);//设置GPIOB.14为高电平，关闭LED
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);//设置GPIOB.15为高电平，关闭LED
}

/**
  * @brief  点亮几颗LED
  * @param  None
  * @retval None
	* @note   LED链接到GPIOB.12，低电平亮，高电平灭
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
       
  /* 使能GPIOB时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /* 配置GPIOB.12为推挽输出模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_7);//设置GPIOA.7为高电平，关闭LED
}

void IndicatorLedOn(void)
{
    SYS_LED = 1;
}
void IndicatorLedOff(void)
{
    SYS_LED = 0;
}

#define INDICATOR_LED_WAITING_PERIOD                400/10
#define INDICATOR_LED_ERROR_PERIOD                150/10
#define INDICATOR_LED_POWER_ON_DELAY_TIME   10000/10  
void IndicatorLed(uint32_t tick)
{
    static uint32_t start_tick = 0;
    static uint8_t cnt = 0;
    
//    static uint8_t power_on_delay_flag = 0;
//    if(power_on_delay_flag == 0)
//    {
//        if(tick < INDICATOR_LED_POWER_ON_DELAY_TIME)
//        {
//            return ;
//        }
//        power_on_delay_flag = 1;
//    }
 
    if(start_tick == 0)
    {
        start_tick = tick;   
    }
    if(tick - start_tick >= INDICATOR_LED_WAITING_PERIOD)
    {
        if(cnt++ % 2)
        {
            IndicatorLedOn();
        }
        else
        {
            IndicatorLedOff();
        }
        start_tick = tick;
    }
    
    if(sys_status == STATUS_WAITING)
    {       
        LED1 = 1; 
    }  

}
/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
