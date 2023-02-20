
#include "main.h"


#define EEPROM_ADDR				0x00				//存储位置首地址
#define EEPROM_DEVICE_ADDR		0xA0				//EEPROM I2C器件地址

#define EE_TYPE 				AT24C64		//信号宏定义
#define EEPROM_PAGESIZE			AT24C64_PAGESIZE   	//AT24C页大小


//void EE_Init(void)
//{
//	i2c_port_init();
//}


///****************************************************
//**函数名:I2C_Standby_24C
//**功能:24C是否准备好再写入的判断
//**注意事项:本函数可以理解为:判忙
//****************************************************/
//void I2C_Standby_24C(void)      
//{
//  unsigned short int SR1_Tmp=0;
//  unsigned int timeout=0;
//  timeout = I2C_LONG_TimeOut;//设置超时变量
//  do
//  {
//    /*起始位*/
//    I2C_GenerateSTART(I2C1, ENABLE);
//    /*读SR1*/
//    SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
//    /*器件地址(写)*/
//    I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Transmitter);
//		if(timeout == (I2C_LONG_TimeOut - 1000))I2C1_Configuration();
//  }while((!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002)) && (timeout--));

//  /**/
//  I2C_ClearFlag(I2C1, I2C_FLAG_AF);
//  /*发送停止位*/    
//  I2C_GenerateSTOP(I2C1, ENABLE);
//}


/*********************************************************************
*	功能	：	在AT24CXX指定地址读出一个数据
*	参数	：	ReadAddr--开始读数的地址
*	返回	：	读到的数据
********************************************************************************************/
uint8_t EE_ReadS(uint16_t addr,uint8_t *pBuffer,uint16_t Length)
{				  
	unsigned int timeout=0;

	u8 temp=0;
	
	if(Length == 0) return 1;	//长度为0则直接返回
	rt_enter_critical();
	
    i2cb_port_start();
	if(EE_TYPE>AT24C16)
	{
		i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);				//发送器件地址
		i2cb_port_wait_ack();
		i2cb_port_send_byte( (uint8_t)((addr & 0xff00) >>8));			//发送高地址	    
	}
	else
	{
		i2cb_port_send_byte( EEPROM_DEVICE_ADDR & 0xfe);		//发送器件地址,写数据 	 
	}		
		  
	i2cb_port_wait_ack(); 
    i2cb_port_send_byte((uint8_t)(addr & 0x00ff));						//发送低地址
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
	if(EE_TYPE>AT24C16)
	{
		i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);			//发送器件地址
		i2cb_port_wait_ack();
		i2cb_port_send_byte((uint8_t)((addr & 0xff00) >>8));		//发送高地址	  
	}
	else
	{
		i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);		//发送器件地址0XA0,写数据 
	}		
			 
	i2cb_port_wait_ack();	   
    i2cb_port_send_byte((uint8_t)(addr & 0x00ff));					//发送低地址
	i2cb_port_wait_ack(); 	
	
	i2cb_port_send_byte(data);							//发送字节							   
	i2cb_port_wait_ack();  		    	   
    i2cb_port_stop();									//产生一个停止条件
	rt_exit_critical();
} 

void EE_WritePage(uint16_t addr,uint8_t *pBuffer,uint16_t Length)
{
//	rt_enter_critical();
//	
//    i2cb_port_start(); 
//	if(EE_TYPE>AT24C16)
//	{
//		i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);			//发送器件地址
//		i2cb_port_wait_ack();
//		i2cb_port_send_byte((uint8_t)((addr & 0xff00) >>8));		//发送高地址	  
//	}
//	else
//	{
//		i2cb_port_send_byte(EEPROM_DEVICE_ADDR & 0xfe);		//发送器件地址0XA0,写数据 
//	}		
//			 
//	i2cb_port_wait_ack();	   
//    i2cb_port_send_byte((uint8_t)(addr & 0x00ff));					//发送低地址
//	i2cb_port_wait_ack(); 	
//	
//	i2cb_port_send_byte(data);							//发送字节
//	
//	i2cb_port_wait_ack();  		    	   
//    i2cb_port_stop();									//产生一个停止条件
//	rt_exit_critical();
	
	
}

void EE_WriteS(uint16_t addr,uint8_t *pBuffer,uint16_t Length)
{
	
	
	
}

/*************************************************
**函数名:I2C_PageWrite_24C
**功能:写入一页(以内)  最大32byte
*************************************************/
//unsigned char EE_PageWrite(uint16_t addr, uint8_t * pBuffer, uint16_t Length)
//{
//	unsigned int timeout;
//	timeout = I2C_FLAG_TimeOut;//设置超时变量
//	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && (timeout--));
//	if(!timeout)return ERROR;

//	/*起始位*/
//	I2C_GenerateSTART(I2C1, ENABLE);
//	timeout = I2C_FLAG_TimeOut;//设置超时变量
//	while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--));
//	if(!timeout)return ERROR;	

//	/*器件地址(写)*/
//	I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Transmitter);
//	timeout = I2C_FLAG_TimeOut;//设置超时变量
//	while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (timeout--));
//	if(!timeout)return ERROR;

//	/*写地址值*/
//	I2C_SendData(I2C1, (addr & 0xff00) >> 8); //发送高8位地址
//	timeout = I2C_FLAG_TimeOut;//设置超时变量  
//	while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));/*数据已发送*/
//	if(!timeout)return ERROR;
//	I2C_SendData(I2C1, addr & 0x00ff); //发送低8位地址
//	timeout = I2C_FLAG_TimeOut;//设置超时变量  
//	while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));/*数据已发送*/
//	if(!timeout)return ERROR;

//	while(Length--)  
//	{
//		I2C_SendData(I2C1, *pBuffer); 
//		pBuffer++; 
//		timeout = I2C_FLAG_TimeOut;//设置超时变量
//		while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));
//		if(!timeout)return ERROR;
//	}
//	/*停止位*/
//	I2C_GenerateSTOP(I2C1, ENABLE);
//	return SUCCESS;
//}


/***************************************************
**函数名:I2C1_ReadS_24C
**功能:读取EEPROM多个字节，最大8192
**
***************************************************/
//void EE_ReadS(uint16_t addr ,uint8_t * pBuffer,uint16_t Length)
//{
//	unsigned int timeout;
//	if(Length==0)return;  //长度为0直接返回
//	timeout = I2C_FLAG_TimeOut;//设置超时变量
//	I2C_Standby_24C();
//	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && (timeout--));

//	/*允许1字节1应答模式*/
//	I2C_AcknowledgeConfig(I2C1, ENABLE);

//	/* 发送起始位 */
//	I2C_GenerateSTART(I2C1, ENABLE);
//	timeout = I2C_FLAG_TimeOut;//设置超时变量  
//	while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--));/*EV5,主模式*/

//	/*发送器件地址(写)*/
//	I2C_Send7bitAddress(I2C1,  EEPROM_ADDR, I2C_Direction_Transmitter);
//	timeout = I2C_FLAG_TimeOut;//设置超时变量  
//	while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (timeout--));

//	/*发送地址,,地址分两部分，高地址是字节地址，低地址是页地址*/
//	I2C_SendData(I2C1, (addr & 0xff00) >> 8); //发送高8位地址
//	timeout = I2C_FLAG_TimeOut;//设置超时变量
//	while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));/*数据已发送*/
//	I2C_SendData(I2C1, addr & 0x00ff); //发送低8位地址
//	timeout = I2C_FLAG_TimeOut;//设置超时变量
//	while ((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));/*数据已发送*/

//	/*起始位*/
//	I2C_GenerateSTART(I2C1, ENABLE);
//	timeout = I2C_FLAG_TimeOut;//设置超时变量
//	while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--));

//	/*器件读*/
//	I2C_Send7bitAddress(I2C1, EEPROM_ADDR, I2C_Direction_Receiver);
//	timeout = I2C_FLAG_TimeOut;//设置超时变量
//	while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) && (timeout--));
//	while (Length)
//	{
//		if(Length==1)
//		{
//			I2C_AcknowledgeConfig(I2C1, DISABLE);	//最后一位后要关闭应答的
//			I2C_GenerateSTOP(I2C1, ENABLE);			//发送停止位
//		}

//		timeout = I2C_FLAG_TimeOut;//设置超时变量
//		while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) && (timeout--)); /* EV7 */
//		*pBuffer = I2C_ReceiveData(I2C1);
//		pBuffer++;
//		/* Decrement the read bytes counter */
//		Length--;
//	}
//	//再次允许应答模式
//	I2C_AcknowledgeConfig(I2C1, ENABLE);
//}
/*************************************************
**函数名:I2C_WriteS_24C
**功能:页写入24C
*************************************************/
//unsigned char EE_WriteS(uint16_t addr,uint8_t * pBuffer,  uint16_t Length)
//{
//	unsigned char temp;
//	//1.先把页不对齐的部分写入
//	temp=addr % I2C_PAGESIZE;
//	I2C_Standby_24C(); //判断是否忙
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
//		I2C_Standby_24C(); //判断是否忙
//	}
//	//2.从页对齐的地方开始写
//	while(Length)
//	{
//		if(Length>=I2C_PAGESIZE)
//		{
//			if(I2C_PageWrite_24C(addr,pBuffer,  I2C_PAGESIZE) == ERROR )return ERROR; //
//			Length-=I2C_PAGESIZE;
//			addr+=I2C_PAGESIZE;
//			pBuffer+=I2C_PAGESIZE;
//			I2C_Standby_24C();  //判断是否忙
//		}
//		else
//		{
//			if(I2C_PageWrite_24C(addr,pBuffer,  Length) == ERROR )return ERROR;
//			Length=0;
//			I2C_Standby_24C(); //判断是否忙
//		}
//	}
//}


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





