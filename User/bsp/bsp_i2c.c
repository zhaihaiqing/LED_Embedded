/**
  ******************************************************************************
  * @file    Project/BSP/bsp_i2c.c 
  * @author  zhaihaiqing
  * @version V1.0.0
  * @date    5-April-2020
  * @brief   Ӳ��I2C
  ******************************************************************************
  */
#define I2C1_CLOCKSPEED		100000	//I2C1ʱ���ٶ�  

#define I2C1_SCL_PIN		GPIO_Pin_6
#define I2C1_SCL_PIN_PORT	GPIOB
#define I2C1_SDA_PIN		GPIO_Pin_7
#define I2C1_SDA_PIN_PORT	GPIOB

#define I2C2_SCL_PIN		GPIO_Pin_10
#define I2C2_SCL_PIN_PORT	GPIOB
#define I2C2_SDA_PIN		GPIO_Pin_11
#define I2C2_SDA_PIN_PORT	GPIOB

#include "main.h"

void I2C1_init(void)
{
	I2C_InitTypeDef I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable I2C1 ��GPIO clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Reset I2C1 peripheral */
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,  ENABLE);
	/* Release reset signal of I2C1 IP */
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

	//���������ӵ���Ӧ�˿���
	GPIO_PinAFConfig(I2C1_SCL_PIN_PORT, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(I2C1_SDA_PIN_PORT, GPIO_PinSource7, GPIO_AF_I2C1);

	/* Configure I2C1 pins: SCL and SDA */
	GPIO_InitStructure.GPIO_Pin =  I2C1_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���踴��ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//��©���
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//������
	GPIO_Init(I2C1_SCL_PIN_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  I2C1_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���踴��ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//��©���
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//������
	GPIO_Init(I2C1_SDA_PIN_PORT, &GPIO_InitStructure);

	I2C_DeInit(I2C1);
	//I2C_SoftwareResetCmd(I2C1, ENABLE);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed =  I2C1_CLOCKSPEED;

	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_ITConfig(I2C1, I2C_IT_ERR, ENABLE);
	I2C_Cmd(I2C1, ENABLE);
}


#define I2C_FLAG_TimeOut  		0xffff     	//��ʱ���� 0x5000
#define I2C1_Device1_ADDRESS7   	0x30    	//��ַ

uint8_t I2C1_readS(uint8_t device_addr , uint8_t reg_addr , uint8_t * px, uint16_t len)
{
	unsigned int timeout;
	if(len==0) return 0;  //����Ϊ0ֱ�ӷ���
	
	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && (timeout--));
	if(!timeout)return 1;
	
	/* ������ʼλ */
	I2C_GenerateSTART(I2C1, ENABLE);
	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����  
	while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--));	/*EV5,��ģʽ*/

	/*����������ַ(д)*/
	I2C_Send7bitAddress(I2C1,  device_addr, I2C_Direction_Transmitter);
	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����  
	while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (timeout--));

//	/*���͵�ַ,,��ַ�������֣��ߵ�ַ���ֽڵ�ַ���͵�ַ��ҳ��ַ*/
//	I2C_SendData(I2C1, (addr & 0xff00) >> 8); //���͸�8λ��ַ
//	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
//	while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));/*�����ѷ���*/
	I2C_SendData(I2C1, reg_addr ); //���͵�8λ��ַ
	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
	while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));/*�����ѷ���*/

	/*��ʼλ*/
	I2C_GenerateSTART(I2C1, ENABLE);
	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
	while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--));

	/*������*/
	I2C_Send7bitAddress(I2C1, device_addr, I2C_Direction_Receiver);
	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
	while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) && (timeout--));
	while (len)
	{
		if(len == 1)
		{
			I2C_AcknowledgeConfig(I2C1, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
			I2C_GenerateSTOP(I2C1, ENABLE);			//����ֹͣλ
		}

		timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
		while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) && (timeout--)); /* EV7 */
		*px = I2C_ReceiveData(I2C1);
		px++;
		/* Decrement the read bytes counter */
		len--;
  }
  //�ٴ�����Ӧ��ģʽ
  I2C_AcknowledgeConfig(I2C1, ENABLE);
}	

uint8_t I2C1_writeS(uint8_t device_addr , uint8_t reg_addr , uint8_t * px, uint16_t len)
{
	unsigned int timeout;
	
	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && (timeout--));
	if(!timeout)return 1;
	
	I2C_GenerateSTART(I2C1, ENABLE);
	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����  
	while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--));	/*EV5,��ģʽ*/
	if(!timeout)return 1;
	
	/*����������ַ(д)*/
	I2C_Send7bitAddress(I2C1,  device_addr, I2C_Direction_Transmitter);
	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����  
	while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (timeout--));
	if(!timeout)return 1;
	
	I2C_SendData(I2C1, reg_addr ); //���͵�8λ��ַ
	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
	while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));/*�����ѷ���*/
	if(!timeout)return 1;
	
	while(len--)  
	{
		I2C_SendData(I2C1, *px); 
		px++; 
		timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
		while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));
		if(!timeout)return 1;
	}
	
	I2C_GenerateSTOP(I2C1, ENABLE);
    return 0;
}















