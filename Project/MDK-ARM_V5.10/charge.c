#include "charge.h"

void charge_ctl_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
       
  /* ʹ��GPIOBʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /* ����GPIOB.12Ϊ�������ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);

}

void charge_ctl_on(void)
{
	CHARGE = 1;
}

void charge_ctl_off(void)
{
	CHARGE = 0;
}
