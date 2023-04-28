
#include "main.h"


#define EEPROM_ADDR				0x00				//存储位置首地址
#define EEPROM_DEVICE_ADDR		0xA0				//EEPROM I2C器件地址

#define EE_TYPE 				AT24C64		//信号宏定义
#define EEPROM_PAGESIZE			AT24C64_PAGESIZE   	//AT24C页大小


void EE_Init(void)
{
	i2cb_port_init();
}

/*********************************************************************
*	功能	：	在AT24CXX指定地址读出一个数据
*	参数	：	ReadAddr--开始读数的地址
*	返回	：	读到的数据
**********************************************************************/
uint8_t EE_ReadS(uint16_t addr,uint8_t *pBuffer,uint16_t Length)
{				  
	unsigned int timeout=0;

	u8 temp=0;
	
	if(Length == 0) return 1;	//长度为0则直接返回
	rt_enter_critical();
	
    i2cb_port_start();
	i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);			//发送器件地址
	i2cb_port_wait_ack();
	if(EE_TYPE>AT24C16)
	{
		i2cb_port_send_byte((uint8_t)((addr & 0xff00) >>8));		//发送高地址
		i2cb_port_wait_ack();		
	}   
    i2cb_port_send_byte((uint8_t)(addr & 0x00ff));					//发送低地址
	i2cb_port_wait_ack(); 	
	
	i2cb_port_start();
	i2cb_port_send_byte(EEPROM_DEVICE_ADDR | 0x01);								//进入接收模式
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
    i2cb_port_stop();											//产生一个停止条件
	rt_exit_critical();
	return 0;
}

/*********************************************************************
*	功能	：	在AT24CXX指定地址写入一个数据
*	参数	：
				WriteAddr  --写入数据的目的地址    
				DataToWrite--要写入的数据
********************************************************************************************/
void EE_WriteOneByte(uint16_t addr,uint8_t data)
{			
	rt_enter_critical();
	
    i2cb_port_start(); 
	i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);			//发送器件地址
	i2cb_port_wait_ack();
	if(EE_TYPE>AT24C16)
	{
		i2cb_port_send_byte((uint8_t)((addr & 0xff00) >>8));		//发送高地址
		i2cb_port_wait_ack();		
	}   
    i2cb_port_send_byte((uint8_t)(addr & 0x00ff));					//发送低地址
	i2cb_port_wait_ack(); 		
	
	i2cb_port_send_byte(data);							//发送字节							   
	i2cb_port_wait_ack();  		    	   
    i2cb_port_stop();									//产生一个停止条件
	rt_exit_critical();
}

/*********************************************************************
*	功能	：	在AT24CXX的页内写入数据
*	参数	：	ReadAddr--开始读数的地址，Length为写入长度，EEPROM每次
				写入时需要在一页内完成，超过一页的数据，会被写在这一页
				的开始位置，造成数据覆盖。
				
*	返回	：	
**********************************************************************/
void EE_WritePage(uint16_t addr,uint8_t *pBuffer,uint16_t Length)
{	
	if(Length == 0) return;
	
	rt_enter_critical();
	
    i2cb_port_start();
	
	i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);			//发送器件地址
	i2cb_port_wait_ack();
	if(EE_TYPE>AT24C16)
	{
		i2cb_port_send_byte((uint8_t)((addr & 0xff00) >>8));		//发送高地址
		i2cb_port_wait_ack();		
	}   
    i2cb_port_send_byte((uint8_t)(addr & 0x00ff));					//发送低地址
	i2cb_port_wait_ack(); 	
	
	
	while(Length--)
	{
		i2cb_port_send_byte(*pBuffer);
		pBuffer++;
		i2cb_port_wait_ack();  	
	}	   
    i2cb_port_stop();									//产生一个停止条件
	rt_exit_critical();
	rt_thread_mdelay(12);	//AT24Cxx系列 EEPROM最大写周期10ms,延时12ms保证数据能被及时写入
}



void EE_WriteS(uint16_t addr,uint8_t *pBuffer,uint16_t Length)
{
	uint8_t temp=0;
	
	if(Length == 0) return;
	
	temp=addr % EEPROM_PAGESIZE;
	
	/*		首先判断地址起始位置，先将不完整部分写入		*/
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
	/*		然后将整页部分写入		*/
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
//* Description    : EEPROM全片擦除程序，
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
//* Description    : EEPROM校验程序，向EEPROM中写入8个数据，读出，检查是否一致
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





