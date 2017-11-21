/*******************************************************************************
 * @name    : ��ʱ������
 * @author  : ������
 * @web     : WWW.UCORTEX.COM
 * @version : V1.2
 * @date    : 2014-04-03
 * @MDK     : KEIL MDK4.72a & KEL MDK 5.10
 * @brief   : ����������ö�ʱ���ͱ�д��ʱ�жϷ������
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
 *
 *-----------------------------------------------------------------------------
 * @history
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-04-06    �����ˣ�������
 * �汾��¼��V1.0
 * �������ݣ��½�
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-05-02    �����ˣ�������
 * �汾��¼��V1.1
 * �������ݣ����TIM4_CH3 PWM�������
 * ----------------------------------------------------------------------------
 * ����ʱ�䣺2014-05-02    �����ˣ�������
 * �汾��¼��V1.2
 * �������ݣ����TIM2_CH1���벶������
 * ----------------------------------------------------------------------------
 *
 ******************************************************************************/
 
#include "timer.h" 

/**
  * @brief ��ʱ��3(TIM3)�жϳ�ʼ��
  * @param arr���Զ���װֵ��
  * @param psc��ʱ��Ԥ��Ƶ��
  * @retval none
  * @note
	*    ���ö�ʱ��3(TIM3)Ϊ���¼���ģʽ
  *    ��ʱ������Ƶ�� = PCLK / ( psc + 1 )
  *    ��ʱ���ж����� = ( arr + 1 )*( pac + 1) / PCLK
  */
void Timer1_Init(uint16_t arr, uint16_t psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʹ������ʱ��
	
	TIM_TimeBaseStructure.TIM_Period = arr;		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = psc;	//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);			//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ʹ��ARRԤװ�أ���ֹ���ϼ���ʱ�����¼��쳣�ӳ�
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );	//��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;						//TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;				//�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);	//��ʼ��NVIC�Ĵ���

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
}

//CaptureStatus : Bit15 - ��ɲ����־
//                Bit14 - ��׽�������ر�־
//                Bit13 - ��������־
//                Bit12 - ����λ
//                Bit11..0 - �����¼���������
__IO uint16_t CaptureStatus = 0;//����״̬
__IO uint16_t CaptureValue1 = 0;//������ʱ�Ĳ���ֵ
__IO uint16_t CaptureValue2 = 0;//�½���ʱ�Ĳ���ֵ

void TIM2_Capture_Init(uint16_t arr,uint16_t psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//��TIM2ʱ��

  //����PA0Ϊ�������룬ע�⿪������PA0�ⲿ��10K����������
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  //����TIM2�ж�ͨ��������ϵͳ�ж���Ϊ2����ռ���ȼ�Ϊ0�������ȼ�Ϊ2
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//�жϺ�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);//�������ò�����ʼ����Ӧ���ж�ͨ��
	
	//ע����ͬһ����Ŀ�У��ж��������һ���ģ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�


	  /* TIM2 ʱ��ģ���ʼ�� ---------------------
     ��ʱ������Ƶ��Fck_cnt = PCLK/(psc+1)
		 ��ʱ������Ƶ��Freq = PCLK/((psc+1)*(arr+1))
  ------------------------------------------------------------ */
	TIM_TimeBaseStructure.TIM_Period = arr;		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = psc;	//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���¼���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_ARRPreloadConfig(TIM2,ENABLE);//ʹ��ARRԤװ�أ���ֹ���ϼ���ʱ�����¼��쳣�ӳ�

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );	//��������ж�
	
	  /* TIM2 configuration: Input Capture mode ---------------------
     The external signal is connected to TIM2 CH1 pin (PA0)  
     The Rising edge is used as active edge,
     The TIM2 CCR1 is used to compute the frequency value 
  ------------------------------------------------------------ */

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
  
  /* TIM enable counter */
  TIM_Cmd(TIM2, ENABLE);

  /* Enable the CC1 Interrupt Request */
  TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	
}
/********************************************************************
* Function Name  : TIM2_PWM_Init(u16 arr,u16 psc)
* Function       : TIM2???CH1?PWM?????
* parameter      : arr - ?????
				   psc - ??????	
* Description    : ??f = 72M/[(psc+1)*(arr+1)]               
* Return         : void
//pc7 tim8ch2
*********************************************************************/
void TIM4_CH4_PWM_Init(u16 arr,u16 psc)
{  
	/*********ʹ�ܳ�ʼ����ʱ��1ʱ��*************/
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��4ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIOC��AFIO���ù���ʱ��   	
	
	/*IO����*/
//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
	
 	/***************��ʼ��GPIO*****************/
	//����PB9Ϊ�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PB9����ӦTIM4_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

 	/*************��ʱ����������***************/	
   //��ʼ��TIM8
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ʹ��ARRԤװ�أ���ֹ���ϼ���ʱ�����¼��쳣�ӳ�

 	/*************����ռ�ձȣ���TIM8_CH2���ó�pwm���***************/
	//��ʼ��TIM8_CH2 PWM���
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //�����������:TIM����Ƚϻ������Ը�
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//ʹ�ܻ��������
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //�����ò�����ʼ������TIM8 OC2

	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM8��CCR1�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM1
  TIM_CtrlPWMOutputs(TIM4,ENABLE);	
}

/**
  * @brief ����PWMռ�ձ�
  * @param duty : ռ�ձȣ���50%�������Ϊ50
  * @retval none
  * @note
	* 		duty = 100*CCR3/(ARR+1)
  */
void PWM_SetDuty1(uint8_t duty)
{
	uint16_t compare;
	compare = duty*(1985+1)/100;//����Ƚ�ֵ
	TIM4->CCR4 = compare;//���±Ƚ�ֵ
}

void SendData1(u8 SysNum, u8 KeyNum) 
{ 
	u8 i; 
	u32 data = 0;
//	u32 data = 0xED12BF40; 
	data = ((~KeyNum & 0xFF)<<24) + ((KeyNum & 0xFF)<<16) + ((~SysNum & 0xff)<< 8) + SysNum;

//	LED = 1;
//	TIM_Cmd(TIM4, ENABLE); 
	PWM_SetDuty1(70);
	delay_us(9000); 
	PWM_SetDuty1(100);
//	LED = 0;
//	TIM_Cmd(TIM4, DISABLE); 
	delay_us(4500);
	PWM_SetDuty1(70);
//	LED = 1;
//	TIM_Cmd(TIM4, ENABLE); 	
	
	for(i=0; i<32; i++) 
	{ 
		delay_us(560);//480
	PWM_SetDuty1(100);
//		LED = 0;
//		TIM_Cmd(TIM4, DISABLE); 
		
		if(((data >> i) & 0x00000001) == 0) 
		{   
			delay_us(560);   //0.56ms 620
		} 
		else 
		{  
			delay_us(1690); //1.69ms 
		} 
	PWM_SetDuty1(70);
//		LED = 1;
//		TIM_Cmd(TIM4, ENABLE); 
	} 
	PWM_SetDuty1(100);
	delay_us(560); 
	PWM_SetDuty1(70);
	delay_us(560); 
	PWM_SetDuty1(100);
//	TIM_Cmd(TIM4, DISABLE); 
//	  LED = 0;
} 

/********************************************************************
* Function Name  : TIM2_PWM_Init(u16 arr,u16 psc)
* Function       : TIM2???CH1?PWM?????
* parameter      : arr - ?????
				   psc - ??????	
* Description    : ??f = 72M/[(psc+1)*(arr+1)]               
* Return         : void
//pb8 tim4ch3
*********************************************************************/
void TIM4_CH3_PWM_Init(u16 arr,u16 psc)
{  
	/*********ʹ�ܳ�ʼ����ʱ��1ʱ��*************/
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIOB��AFIO���ù���ʱ��   	
	
	/*IO����*/
//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
	
 	/***************��ʼ��GPIO*****************/
	//����PB10Ϊ�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //PE10����ӦTIM1_CH2N
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

 	/*************��ʱ����������***************/	
   //��ʼ��TIM1
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ʹ��ARRԤװ�أ���ֹ���ϼ���ʱ�����¼��쳣�ӳ�

 	/*************����ռ�ձȣ���TIM1_CH2N���ó�pwm���***************/
	//��ʼ��TIM1_CH2N PWM���
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //�����������:TIM����Ƚϻ������Ը�
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//ʹ�ܻ��������
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //�����ò�����ʼ������TIM1 OC1

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM1
  TIM_CtrlPWMOutputs(TIM4,ENABLE);	
}


/**
  * @brief ����PWMռ�ձ�
  * @param duty : ռ�ձȣ���50%�������Ϊ50
  * @retval none
  * @note
	* 		duty = 100*CCR3/(ARR+1)
  */
void PWM_SetDuty2(uint8_t duty)
{
	uint16_t compare;
	compare = duty*(1985+1)/100;//����Ƚ�ֵ
	TIM4->CCR3 = compare;//���±Ƚ�ֵ
}


void SendData2(u8 SysNum, u8 KeyNum) 
{ 
	u8 i; 
	u32 data = 0;
//	u32 data = 0xED12BF40; 
	data = ((~KeyNum & 0xFF)<<24) + ((KeyNum & 0xFF)<<16) + ((~SysNum & 0xff)<< 8) + SysNum;

//	LED = 1;
//	TIM_Cmd(TIM4, ENABLE); 
	PWM_SetDuty2(70);
	delay_us(9000); 
	PWM_SetDuty2(100);
//	LED = 0;
//	TIM_Cmd(TIM4, DISABLE); 
	delay_us(4500);
	PWM_SetDuty2(70);
//	LED = 1;
//	TIM_Cmd(TIM4, ENABLE); 	
	
	for(i=0; i<32; i++) 
	{ 
		delay_us(560);//480
	PWM_SetDuty2(100);
//		LED = 0;
//		TIM_Cmd(TIM4, DISABLE); 
		
		if(((data >> i) & 0x00000001) == 0) 
		{   
			delay_us(560);   //0.56ms 620
		} 
		else 
		{  
			delay_us(1690); //1.69ms 
		} 
	PWM_SetDuty2(70);
//		LED = 1;
//		TIM_Cmd(TIM4, ENABLE); 
	} 
	PWM_SetDuty2(100);
	delay_us(560); 
	PWM_SetDuty2(70);
	delay_us(560); 
	PWM_SetDuty2(100);
//	TIM_Cmd(TIM4, DISABLE); 
//	  LED = 0;
} 

void TIM1_CH1N_PWM_Init(u16 arr,u16 psc)
{  
	/*********ʹ�ܳ�ʼ����ʱ��1ʱ��*************/
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIOB��AFIO���ù���ʱ��   	

	/*IO����*/
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);
	
 	/***************��ʼ��GPIO*****************/
	//����PB10Ϊ�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //PE10����ӦTIM1_CH2N
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO

 	/*************��ʱ����������***************/	
   //��ʼ��TIM1
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ʹ��ARRԤװ�أ���ֹ���ϼ���ʱ�����¼��쳣�ӳ�

 	/*************����ռ�ձȣ���TIM1_CH2N���ó�pwm���***************/
	//��ʼ��TIM1_CH2N PWM���
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //�����������:TIM����Ƚϻ������Ը�
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//ʹ�ܻ��������
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //�����ò�����ʼ������TIM1 OC1

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
  TIM_CtrlPWMOutputs(TIM1,ENABLE);	
}


/**
  * @brief ����PWMռ�ձ�
  * @param duty : ռ�ձȣ���50%�������Ϊ50
  * @retval none
  * @note
	* 		duty = 100*CCR3/(ARR+1)
  */
void PWM_SetDuty3(uint8_t duty)
{
	uint16_t compare;
	compare = duty*(1895+1)/100;//����Ƚ�ֵ
	TIM1->CCR1 = compare;//���±Ƚ�ֵ
}

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/
