
#include "main.h"


#define EEPROM_ADDR				0x00				//�洢λ���׵�ַ
#define EEPROM_DEVICE_ADDR		0xA0				//EEPROM I2C������ַ

#define EE_TYPE 				AT24C64		//�źź궨��
#define EEPROM_PAGESIZE			AT24C64_PAGESIZE   	//AT24Cҳ��С


void EE_Init(void)
{
	i2cb_port_init();
}

/*********************************************************************
*	����	��	��AT24CXXָ����ַ����һ������
*	����	��	ReadAddr--��ʼ�����ĵ�ַ
*	����	��	����������
**********************************************************************/
uint8_t EE_ReadS(uint16_t addr,uint8_t *pBuffer,uint16_t Length)
{				  
	unsigned int timeout=0;

	u8 temp=0;
	
	if(Length == 0) return 1;	//����Ϊ0��ֱ�ӷ���
	rt_enter_critical();
	
    i2cb_port_start();
	i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);			//����������ַ
	i2cb_port_wait_ack();
	if(EE_TYPE>AT24C16)
	{
		i2cb_port_send_byte((uint8_t)((addr & 0xff00) >>8));		//���͸ߵ�ַ
		i2cb_port_wait_ack();		
	}   
    i2cb_port_send_byte((uint8_t)(addr & 0x00ff));					//���͵͵�ַ
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
	i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);			//����������ַ
	i2cb_port_wait_ack();
	if(EE_TYPE>AT24C16)
	{
		i2cb_port_send_byte((uint8_t)((addr & 0xff00) >>8));		//���͸ߵ�ַ
		i2cb_port_wait_ack();		
	}   
    i2cb_port_send_byte((uint8_t)(addr & 0x00ff));					//���͵͵�ַ
	i2cb_port_wait_ack(); 		
	
	i2cb_port_send_byte(data);							//�����ֽ�							   
	i2cb_port_wait_ack();  		    	   
    i2cb_port_stop();									//����һ��ֹͣ����
	rt_exit_critical();
}

/*********************************************************************
*	����	��	��AT24CXX��ҳ��д������
*	����	��	ReadAddr--��ʼ�����ĵ�ַ��LengthΪд�볤�ȣ�EEPROMÿ��
				д��ʱ��Ҫ��һҳ����ɣ�����һҳ�����ݣ��ᱻд����һҳ
				�Ŀ�ʼλ�ã�������ݸ��ǡ�
				
*	����	��	
**********************************************************************/
void EE_WritePage(uint16_t addr,uint8_t *pBuffer,uint16_t Length)
{	
	if(Length == 0) return;
	
	rt_enter_critical();
	
    i2cb_port_start();
	
	i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);			//����������ַ
	i2cb_port_wait_ack();
	if(EE_TYPE>AT24C16)
	{
		i2cb_port_send_byte((uint8_t)((addr & 0xff00) >>8));		//���͸ߵ�ַ
		i2cb_port_wait_ack();		
	}   
    i2cb_port_send_byte((uint8_t)(addr & 0x00ff));					//���͵͵�ַ
	i2cb_port_wait_ack(); 	
	
	
	while(Length--)
	{
		i2cb_port_send_byte(*pBuffer);
		pBuffer++;
		i2cb_port_wait_ack();  	
	}	   
    i2cb_port_stop();									//����һ��ֹͣ����
	rt_exit_critical();
	rt_thread_mdelay(12);	//AT24Cxxϵ�� EEPROM���д����10ms,��ʱ12ms��֤�����ܱ���ʱд��
}



void EE_WriteS(uint16_t addr,uint8_t *pBuffer,uint16_t Length)
{
	uint8_t temp=0;
	
	if(Length == 0) return;
	
	temp=addr % EEPROM_PAGESIZE;
	
	/*		�����жϵ�ַ��ʼλ�ã��Ƚ�����������д��		*/
	if(temp)
	{
		temp=EEPROM_PAGESIZE-temp;
		
		if(Length >= temp)
		{
			EE_WritePage(addr, pBuffer, temp);
			Length -= temp;
			addr += temp;
			pBuffer += temp;
		}
		else
		{
			EE_WritePage(addr, pBuffer, Length);
			Length = 0;
		}
	}
	/*		Ȼ����ҳ����д��		*/
	while(Length)
	{
		if(Length >= EEPROM_PAGESIZE)
		{
			EE_WritePage(addr, pBuffer, EEPROM_PAGESIZE);
			Length -= EEPROM_PAGESIZE;
			addr += EEPROM_PAGESIZE;
			pBuffer += EEPROM_PAGESIZE;
		}
		else
		{
			EE_WritePage(addr, pBuffer, Length);
			Length = 0;
		}
	}
	
}




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





