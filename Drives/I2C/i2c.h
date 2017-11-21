
#ifndef __I2C_H
#define __I2C_H
#include "stm32f10x.h"
#include "bitband.h"

#define I2C_SPEED_1K		5000	//���ݴ������ٶ����ã����ﴦ�����ٶ���72MHz

//I2C�˿ڶ���
#define I2C_SCL    GPIOout(GPIOB, 12)	//SCL--PB12
#define I2C_SDA    GPIOout(GPIOB, 13)	//SDA--PB13
#define READ_SDA   GPIOin( GPIOB, 13)	//����SDA

//����SDA:PB13�������
#define SDA_IN()  {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=8<<20;}		//�ο�stm32�Ĵ�������
#define SDA_OUT() {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=3<<20;}

typedef enum
{
	I2C_SUCCESS = 0,
	I2C_TIMEOUT,
	I2C_ERROR,
}I2C_StatusTypeDef;

extern uint32_t i2c_speed;	//I2C�����ٶ� = I2C_SPEED_1K / i2c_speed

/* ---------------------------����I2CЭ���д��ʱ����------------------------------*/
void I2C_Init(void);				//��ʼ��I2C��IO��				 
void I2C_Start(void);				//����I2C��ʼ�ź�
void I2C_Stop(void);				//����I2Cֹͣ�ź�
uint8_t I2C_Wait_ACK(void);	//I2C�ȴ�ACK�ź�
void I2C_ACK(void);					//I2C����ACK�ź�
void I2C_NACK(void);				//I2C������ACK�ź�
void I2C_Send_Byte(uint8_t data);		//I2C����һ���ֽ�
uint8_t I2C_Read_Byte(uint8_t ack);	//I2C��ȡһ���ֽ�
uint16_t I2C_SetSpeed(uint16_t speed);//����I2C�ٶ�(1Kbps~400Kbps,speed��λ��Kbps)

/* ---------------------------���²����Ƿ�װ�õ�I2C��д����--------------------------- */

//���嵽ĳһ������������ϸ�Ķ�������������I2C���ֵ�˵������ΪĳЩ������I2C�Ķ�д������
//��һЩ���죬����Ĵ��������ھ��������I2C�����У�������֤OK�ģ�
I2C_StatusTypeDef I2C_WriteOneByte(uint8_t DevAddr, uint8_t DataAddr, uint8_t Data);//��I2C���豸д��һ���ֽ�

I2C_StatusTypeDef I2C_ReadOneByte(uint8_t DevAddr, uint8_t DataAddr, uint8_t* Data);//��I2C���豸��ȡһ���ֽ�

I2C_StatusTypeDef I2C_WriteBit(uint8_t DevAddr, uint8_t DataAddr, uint8_t Bitx, uint8_t BitSet);

I2C_StatusTypeDef single_write(uint8_t DevAddr, uint16_t DataAddr, uint8_t pData);									//��I2C���豸��д��һ���ֽ�

I2C_StatusTypeDef double_write(uint8_t DevAddr, uint16_t DataAddr, uint16_t pData);									//��I2C���豸����д��2���ֽ�

I2C_StatusTypeDef multi_write(uint8_t DevAddr, uint16_t DataAddr, uint32_t pData);									//��I2C���豸����д��4���ֽ�

I2C_StatusTypeDef single_read(uint8_t DevAddr, uint16_t DataAddr, uint8_t* pData);									//��I2C�豸�϶�ȡһ���ֽ�

I2C_StatusTypeDef multi_read(uint8_t DevAddr, uint16_t DataAddr, uint8_t* pData, uint32_t Num);			//��I2C�豸������ȡNum���ֽ�


#endif

/********************* (C) COPYRIGHT 2014 WWW.UCORTEX.COM **********END OF FILE**********/

