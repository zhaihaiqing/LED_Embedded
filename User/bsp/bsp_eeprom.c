
#include "main.h"


#define EEPROM_ADDR				0x00				//�洢λ���׵�ַ
#define EEPROM_DEVICE_ADDR		0xA0				//EEPROM I2C������ַ

#define EE_TYPE 				AT24C64		//�źź궨��
#define EEPROM_PAGESIZE			AT24C64_PAGESIZE   	//AT24Cҳ��С


//void EE_Init(void)
//{
//	i2c_port_init();
//}


///****************************************************
//**������:I2C_Standby_24C
//**����:24C�Ƿ�׼������д����ж�
//**ע������:�������������Ϊ:��æ
//****************************************************/
//void I2C_Standby_24C(void)      
//{
//  unsigned short int SR1_Tmp=0;
//  unsigned int timeout=0;
//  timeout = I2C_LONG_TimeOut;//���ó�ʱ����
//  do
//  {
//    /*��ʼλ*/
//    I2C_GenerateSTART(I2C1, ENABLE);
//    /*��SR1*/
//    SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
//    /*������ַ(д)*/
//    I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Transmitter);
//		if(timeout == (I2C_LONG_TimeOut - 1000))I2C1_Configuration();
//  }while((!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002)) && (timeout--));

//  /**/
//  I2C_ClearFlag(I2C1, I2C_FLAG_AF);
//  /*����ֹͣλ*/    
//  I2C_GenerateSTOP(I2C1, ENABLE);
//}


/*********************************************************************
*	����	��	��AT24CXXָ����ַ����һ������
*	����	��	ReadAddr--��ʼ�����ĵ�ַ
*	����	��	����������
********************************************************************************************/
uint8_t EE_ReadS(uint16_t addr,uint8_t *pBuffer,uint16_t Length)
{				  
	unsigned int timeout=0;

	u8 temp=0;
	
	if(Length == 0) return 1;	//����Ϊ0��ֱ�ӷ���
	rt_enter_critical();
	
    i2cb_port_start();
	if(EE_TYPE>AT24C16)
	{
		i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);				//����������ַ
		i2cb_port_wait_ack();
		i2cb_port_send_byte( (uint8_t)((addr & 0xff00) >>8));			//���͸ߵ�ַ	    
	}
	else
	{
		i2cb_port_send_byte( EEPROM_DEVICE_ADDR & 0xfe);		//����������ַ,д���� 	 
	}		
		  
	i2cb_port_wait_ack(); 
    i2cb_port_send_byte((uint8_t)(addr & 0x00ff));						//���͵͵�ַ
	i2cb_port_wait_ack();
	
	i2cb_port_start();
	i2cb_port_send_byte(EEPROM_DEVICE_ADDR | 0x01);								//�������ģʽ
	i2cb_port_wait_ack();
	
	while(Length)
	{		
		*pBuffer = i2cb_port_read_byte();
		pBuffer++;
		Length--;
		if(Length)
		{
			i2cb_port_ack();
		}
		else
		{
			i2cb_port_nack();
		}
	}
    i2cb_port_stop();											//����һ��ֹͣ����
	rt_exit_critical();
	return 0;
}

/*********************************************************************
*	����	��	��AT24CXXָ����ַд��һ������
*	����	��
				WriteAddr  --д�����ݵ�Ŀ�ĵ�ַ    
				DataToWrite--Ҫд�������
********************************************************************************************/
void EE_WriteOneByte(uint16_t addr,uint8_t data)
{			
	rt_enter_critical();
	
    i2cb_port_start(); 
	if(EE_TYPE>AT24C16)
	{
		i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);			//����������ַ
		i2cb_port_wait_ack();
		i2cb_port_send_byte((uint8_t)((addr & 0xff00) >>8));		//���͸ߵ�ַ	  
	}
	else
	{
		i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);		//����������ַ0XA0,д���� 
	}		
			 
	i2cb_port_wait_ack();	   
    i2cb_port_send_byte((uint8_t)(addr & 0x00ff));					//���͵͵�ַ
	i2cb_port_wait_ack(); 	
	
	i2cb_port_send_byte(data);							//�����ֽ�							   
	i2cb_port_wait_ack();  		    	   
    i2cb_port_stop();									//����һ��ֹͣ����
	rt_exit_critical();
} 

void EE_WritePage(uint16_t addr,uint8_t *pBuffer,uint16_t Length)
{
//	rt_enter_critical();
//	
//    i2cb_port_start(); 
//	if(EE_TYPE>AT24C16)
//	{
//		i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);			//����������ַ
//		i2cb_port_wait_ack();
//		i2cb_port_send_byte((uint8_t)((addr & 0xff00) >>8));		//���͸ߵ�ַ	  
//	}
//	else
//	{
//		i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);		//����������ַ0XA0,д���� 
//	}		
//			 
//	i2cb_port_wait_ack();	   
//    i2cb_port_send_byte((uint8_t)(addr & 0x00ff));					//���͵͵�ַ
//	i2cb_port_wait_ack(); 	
//	
//	i2cb_port_send_byte(data);							//�����ֽ�
//	
//	i2cb_port_wait_ack();  		    	   
//    i2cb_port_stop();									//����һ��ֹͣ����
//	rt_exit_critical();
	
	
}

void EE_WriteS(uint16_t addr,uint8_t *pBuffer,uint16_t Length)
{
	
	
	
}

/*************************************************
**������:I2C_PageWrite_24C
**����:д��һҳ(����)  ���32byte
*************************************************/
//unsigned char EE_PageWrite(uint16_t addr, uint8_t * pBuffer, uint16_t Length)
//{
//	unsigned int timeout;
//	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
//	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && (timeout--));
//	if(!timeout)return ERROR;

//	/*��ʼλ*/
//	I2C_GenerateSTART(I2C1, ENABLE);
//	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
//	while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--));
//	if(!timeout)return ERROR;	

//	/*������ַ(д)*/
//	I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Transmitter);
//	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
//	while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (timeout--));
//	if(!timeout)return ERROR;

//	/*д��ֵַ*/
//	I2C_SendData(I2C1, (addr & 0xff00) >> 8); //���͸�8λ��ַ
//	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����  
//	while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));/*�����ѷ���*/
//	if(!timeout)return ERROR;
//	I2C_SendData(I2C1, addr & 0x00ff); //���͵�8λ��ַ
//	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����  
//	while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));/*�����ѷ���*/
//	if(!timeout)return ERROR;

//	while(Length--)  
//	{
//		I2C_SendData(I2C1, *pBuffer); 
//		pBuffer++; 
//		timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
//		while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));
//		if(!timeout)return ERROR;
//	}
//	/*ֹͣλ*/
//	I2C_GenerateSTOP(I2C1, ENABLE);
//	return SUCCESS;
//}


/***************************************************
**������:I2C1_ReadS_24C
**����:��ȡEEPROM����ֽڣ����8192
**
***************************************************/
//void EE_ReadS(uint16_t addr ,uint8_t * pBuffer,uint16_t Length)
//{
//	unsigned int timeout;
//	if(Length==0)return;  //����Ϊ0ֱ�ӷ���
//	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
//	I2C_Standby_24C();
//	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && (timeout--));

//	/*����1�ֽ�1Ӧ��ģʽ*/
//	I2C_AcknowledgeConfig(I2C1, ENABLE);

//	/* ������ʼλ */
//	I2C_GenerateSTART(I2C1, ENABLE);
//	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����  
//	while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--));/*EV5,��ģʽ*/

//	/*����������ַ(д)*/
//	I2C_Send7bitAddress(I2C1,  EEPROM_ADDR, I2C_Direction_Transmitter);
//	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����  
//	while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (timeout--));

//	/*���͵�ַ,,��ַ�������֣��ߵ�ַ���ֽڵ�ַ���͵�ַ��ҳ��ַ*/
//	I2C_SendData(I2C1, (addr & 0xff00) >> 8); //���͸�8λ��ַ
//	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
//	while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));/*�����ѷ���*/
//	I2C_SendData(I2C1, addr & 0x00ff); //���͵�8λ��ַ
//	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
//	while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));/*�����ѷ���*/

//	/*��ʼλ*/
//	I2C_GenerateSTART(I2C1, ENABLE);
//	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
//	while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--));

//	/*������*/
//	I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Receiver);
//	timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
//	while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) && (timeout--));
//	while (Length)
//	{
//		if(Length==1)
//		{
//			I2C_AcknowledgeConfig(I2C1, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
//			I2C_GenerateSTOP(I2C1, ENABLE);			//����ֹͣλ
//		}

//		timeout = I2C_FLAG_TimeOut;//���ó�ʱ����
//		while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) && (timeout--)); /* EV7 */
//		*pBuffer = I2C_ReceiveData(I2C1);
//		pBuffer++;
//		/* Decrement the read bytes counter */
//		Length--;
//	}
//	//�ٴ�����Ӧ��ģʽ
//	I2C_AcknowledgeConfig(I2C1, ENABLE);
//}
/*************************************************
**������:I2C_WriteS_24C
**����:ҳд��24C
*************************************************/
//unsigned char EE_WriteS(uint16_t addr,uint8_t * pBuffer,  uint16_t Length)
//{
//	unsigned char temp;
//	//1.�Ȱ�ҳ������Ĳ���д��
//	temp=addr % I2C_PAGESIZE;
//	I2C_Standby_24C(); //�ж��Ƿ�æ
//	if(temp)
//	{
//		temp=I2C_PAGESIZE-temp;
//		if(Length >= temp)
//		{
//			I2C_PageWrite_24C(addr,pBuffer,  temp);
//			Length-=temp;
//			addr+=temp;
//			pBuffer+=temp;
//		}
//    else
//    {
//		if(I2C_PageWrite_24C(addr,pBuffer,  Length) == ERROR )return ERROR;
//		Length = 0;
//    }
//		I2C_Standby_24C(); //�ж��Ƿ�æ
//	}
//	//2.��ҳ����ĵط���ʼд
//	while(Length)
//	{
//		if(Length>=I2C_PAGESIZE)
//		{
//			if(I2C_PageWrite_24C(addr,pBuffer,  I2C_PAGESIZE) == ERROR )return ERROR; //
//			Length-=I2C_PAGESIZE;
//			addr+=I2C_PAGESIZE;
//			pBuffer+=I2C_PAGESIZE;
//			I2C_Standby_24C();  //�ж��Ƿ�æ
//		}
//		else
//		{
//			if(I2C_PageWrite_24C(addr,pBuffer,  Length) == ERROR )return ERROR;
//			Length=0;
//			I2C_Standby_24C(); //�ж��Ƿ�æ
//		}
//	}
//}


///*******************************************************************************
//* Function Name  : EEPROM_Erase
//* Description    : EEPROMȫƬ��������
//* Input          : None
//* Output         : None
//* Return         : None
//*******************************************************************************/
//void EEPROM_EraseFullChip(void)
//{
//	unsigned char  DATA_temp=0x00;
//	I2C_WriteS_24C(0x0000,&DATA_temp,16384);
//	printf("Erase EEPROM OK!\r\n");
//}

///*******************************************************************************
//* Function Name  : EEPROM_Check
//* Description    : EEPROMУ�������EEPROM��д��8�����ݣ�����������Ƿ�һ��
//* Input          : None
//* Output         : None
//* Return         : None
//*******************************************************************************/
//void EEPROM_Check(void)
//{
//	unsigned char counti=0,countj=0;
//	unsigned char I2C_WriteData[8]={0x00,0x01,0x02,0x03,0x04,0x05,0x07,0x08};
//	unsigned char I2C_ReadData[8];
//	printf("Write EEPROM!\r\n");
//	for(counti=0;counti<8;counti++)
//	{
//		printf("0x%x\r\n",I2C_WriteData[counti]);
//	}
//	I2C_WriteS_24C(0x0000,I2C_WriteData,8);
//	Delay(200);
//	printf("Read EEPROM!\r\n");
//	I2C1_ReadS_24C(0x0000,I2C_ReadData,8);
//	for(counti=0;counti<8;counti++)
//	{
//		printf("0x%x\r\n",I2C_ReadData[counti]);
//	}
//	for(counti=0;counti<8;counti++)
//	{
//		if(I2C_ReadData[counti]==I2C_WriteData[counti])
//		{
//			countj++;
//		}
//	}
//	if(countj==8)
//		printf("EEPROM is OK !\r\n");
//	else
//		printf("EEPROM Data wrong !\r\n");
//}





