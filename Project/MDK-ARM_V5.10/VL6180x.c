#include "VL6180x.h"
#include "string.h"
#include "i2c.h"
#include "usart.h"


enum 
{
  HAL_OK       = 0x00,
  HAL_ERROR    = 0x01,
  HAL_BUSY     = 0x02,
  HAL_TIMEOUT  = 0x03
};

void GPIO0_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//使能GPIOB外设时钟
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_2);//根据以上配置初始化GPIO

	//设置SCL和SDA空闲状态为高电平
//	I2C_SCL = 1;
//	I2C_SDA = 1;
//	
//	I2C_SetSpeed(400);//设置I2C访问速度为400Kbps
}

void VL6180x_init(void)
{
	GPIO0_Init();
	GPIOout(GPIOC, 2) = 1;

	single_write( VL6180X_ADDRESS, 0x0207, 0x01);
	single_write( VL6180X_ADDRESS, 0x0208, 0x01);
	single_write( VL6180X_ADDRESS, 0x0096, 0x00);
	single_write( VL6180X_ADDRESS, 0x0097, 0x54);
	single_write( VL6180X_ADDRESS, 0x00e3, 0x00);
	single_write( VL6180X_ADDRESS, 0x00e4, 0x04);
	single_write( VL6180X_ADDRESS, 0x00e5, 0x02);
	single_write( VL6180X_ADDRESS, 0x00e6, 0x01);
	single_write( VL6180X_ADDRESS, 0x00e7, 0x03);
	single_write( VL6180X_ADDRESS, 0x00f5, 0x02);
	single_write( VL6180X_ADDRESS, 0x00d9, 0x05);

	multi_write(	VL6180X_ADDRESS,0x00da,0x00CF03F7);

	single_write( VL6180X_ADDRESS, 0x009f, 0x00);
	single_write( VL6180X_ADDRESS, 0x00a3, 0x28);
	single_write( VL6180X_ADDRESS, 0x00b7, 0x00);
	single_write( VL6180X_ADDRESS, 0x00bb, 0x28);
	single_write( VL6180X_ADDRESS, 0x00b2, 0x09);
	single_write( VL6180X_ADDRESS, 0x00ca, 0x09);
	single_write( VL6180X_ADDRESS, 0x0198, 0x01);
	single_write( VL6180X_ADDRESS, 0x01b0, 0x17);
	single_write( VL6180X_ADDRESS, 0x01ad, 0x00);
	single_write( VL6180X_ADDRESS, 0x00ff, 0x05);
	single_write( VL6180X_ADDRESS, 0x0100, 0x05);
	single_write( VL6180X_ADDRESS, 0x0199, 0x05);
	single_write( VL6180X_ADDRESS, 0x01a6, 0x1b);
	single_write( VL6180X_ADDRESS, 0x01ac, 0x3e);
	single_write( VL6180X_ADDRESS, 0x01a7, 0x1f);
	single_write( VL6180X_ADDRESS, 0x0030, 0x00);
	single_write( VL6180X_ADDRESS, 0x0011, 0x10);
	single_write( VL6180X_ADDRESS, 0x010a, 0x30);
	single_write( VL6180X_ADDRESS, 0x003f, 0x46);
	single_write( VL6180X_ADDRESS, 0x0031, 0xFF);
	single_write( VL6180X_ADDRESS, 0x0040, 0x63);
	single_write( VL6180X_ADDRESS, 0x002e, 0x01);
	single_write( VL6180X_ADDRESS, 0x002c, 0xff);
	single_write( VL6180X_ADDRESS, 0x001b, 0x04);
	single_write( VL6180X_ADDRESS, 0x003e, 0x31);
	single_write( VL6180X_ADDRESS, 0x0014, 0x24);

	single_write(	VL6180X_ADDRESS, 0x001C, 0x28);
	double_write(	VL6180X_ADDRESS, 0x0022, 0x0060);
	double_write(	VL6180X_ADDRESS, 0x0096, 0x00fd);
	single_write( VL6180X_ADDRESS, 0x024, 0x08);
//    single_write( VL6180X_ADDRESS, 0x002d, 0x02);
//    single_write( VL6180X_ADDRESS, 0x0025, 0xff);
//    double_write_I2C1(	VL6180X_ADDRESS, 0x0026, 0x0000);
/***********************offset自动使能********************************/
	single_write( VL6180X_ADDRESS, 0x002d, 0x13);
	double_write(	VL6180X_ADDRESS, 0x0026, 0x000a);
/***********************offset自动使能********************************/
	
	single_write( VL6180X_ADDRESS, 0x016, 0x00);


	single_write( VL6180X_ADDRESS, 0x03f, 0x46);
	single_write( VL6180X_ADDRESS, 0x019, 0xc8);
	single_write( VL6180X_ADDRESS, 0x01A, 0x0a);
	double_write( VL6180X_ADDRESS, 0x0040, 0x0063);
	single_write( VL6180X_ADDRESS, 0x003e, 0x14);
	single_write( VL6180X_ADDRESS, 0x003f, 0x40);
	single_write( VL6180X_ADDRESS, 0x003c, 0x00);
	single_write( VL6180X_ADDRESS, 0x003A, 0xff);
	single_write( VL6180X_ADDRESS, 0x015, 0x07);
//		single_write( VL6180X_ADDRESS, 0x01C, 0x3f);
//		double_write_I2C1( VL6180X_ADDRESS, 0x022, 0x004a);
//		single_write( VL6180X_ADDRESS, 0x011, 0x30);
//		single_write( VL6180X_ADDRESS, 0x015, 0x07);
//		single_write( VL6180X_ADDRESS, 0x018, 0x01);

/**********没检测到*******/
//		single_write( VL6180X_ADDRESS, 0x01AC, 0x3e);
//		single_write( VL6180X_ADDRESS, 0x0f2, 0x01);
//		single_write( VL6180X_ADDRESS, 0x015, 0x01);
//		single_write( VL6180X_ADDRESS, 0x018, 0x01);

	single_write( VL6180X_ADDRESS, 0x01AC, 0x3c);
	single_write( VL6180X_ADDRESS, 0x0f2, 0x05);
//		single_write( VL6180X_ADDRESS, 0x015, 0x01);
//		single_write( VL6180X_ADDRESS, 0x018, 0x01);
/**********检测到*************/

//		single_write( VL6180X_ADDRESS, 0x015, 0x01);
//		single_write( VL6180X_ADDRESS, 0x018, 0x01);
	
/***********按下改变量2程键*******************/

//    double_write_I2C1(	VL6180X_ADDRESS, 0x0096, 0x007f);
//		single_write( VL6180X_ADDRESS, 0x024, 0x04);
			
/***********按下改变量3程键*******************/

	double_write( VL6180X_ADDRESS, 0x0096, 0x0054);
	single_write( VL6180X_ADDRESS, 0x024, 0x02);

	
/********************************************/
}

uint8_t VL6180x_start(void)
{
	uint8_t status;
	uint8_t pdata1;
	status = single_read(VL6180X_ADDRESS,0x000,&pdata1);
//		printf("status = %d\r\n",status);
	if(status != I2C_SUCCESS)
	{
		return 1;
	}
	single_write(VL6180X_ADDRESS, 0x015, 0x01);
	while(single_read(VL6180X_ADDRESS,RESULT__RANGE_STATUS,&pdata1));
	single_write(VL6180X_ADDRESS, 0x018, 0x01);
	
	return 0;
}


uint16_t range_value = MAX_RANGE;

uint8_t VL6180x_end(void)
{
	uint8_t status;
	uint8_t pdata1 = 0;
	uint16_t pdata_3 = 0;
		
	int alpha =(int)(0.85*(1<<16));    /* range distance running average cofs */
  static uint16_t range = 0;             /* range average distance */
	
	status = single_read(VL6180X_ADDRESS,0x000,&pdata1);
//		printf("status = %d\r\n",status);
	if(status != I2C_SUCCESS)
	{
		range_value = MAX_RANGE;
		return 1;
	}
	
	single_read(VL6180X_ADDRESS,RESULT__INTERRUPT_STATUS_GPIO,&pdata1);
//		printf("RESULT__INTERRUPT_STATUS_GPIO = %d\r\n",pdata);
	if(pdata1 == 4)
	{
		single_read(VL6180X_ADDRESS,RESULT__RANGE_VAL,&pdata1);
		if(pdata1 == 255)
		{
			range_value = MAX_RANGE;
			return 0;
		}
		pdata_3 = 3 * pdata1;
		range = ((range * alpha + pdata_3 * ((1 << 16) - alpha)) >> 16 );
		range_value = range + 24;
		if(range_value >= MAX_RANGE)
		{
			range_value = MAX_RANGE;
		}
//			pdata1 = 0;
//			pdata_3 = 0;
	}
	return 0;
}








