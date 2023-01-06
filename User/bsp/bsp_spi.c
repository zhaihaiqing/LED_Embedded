
#include "main.h"

#define SPI_Wait_TimeOut  		0xffff     //超时常量 0x5000	

/*******************************************************************************
* Function Name  : SPI2_Configuration
* Description    : STM32 SPI1配置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_Configuration(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);													//打开外设时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE  , ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);                     						//设置为外设功能，此处不能合并
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;															//设置端口模式，
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                						//外设复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;														//推挽模式
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;														//默认下拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;													//GPIO翻转速度为25MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);																//将配置写入对应的GPIO寄存器中
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;												//设置端口模式，
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                						//外设复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;														//推挽模式
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;														//默认下拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;													//GPIO翻转速度为25MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);																//将配置写入对应的GPIO寄存器中

	//PE6=ADT7301_CS、PE15=FLASH_CS、
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_15;													//设置SPI片选
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;																//端口输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;																//推挽模式
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;																//默认上拉，禁止AD片选
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;															//GPIO翻转速度为2MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);																		//将配置写入对应的GPIO寄存器中 
	
	//PC13=ADT7301_CS
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;																	//设置SPI片选
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;																//端口输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;																//推挽模式
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;																//默认上拉，禁止AD片选
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;															//GPIO翻转速度为2MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);																		//将配置写入对应的GPIO寄存器中 
	
	
	
	/* SPI2 configuration */
	SPI_Cmd(SPI2, DISABLE); 
	SPI_I2S_DeInit(SPI2);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//串行同步时钟的第1个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;//定义波特率预分频的值:波特率预分频值为2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
	SPI_Cmd(SPI2, ENABLE); 
}

/*******************************************************************************
* Function Name  : SPI1_ReadWriteByte
* Description    : 从SPI总线上读取或写一个字节数据
* Input          : 写字节
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char SPI2_ReadWriteByte(unsigned char byte)
{
  unsigned int timeout = SPI_Wait_TimeOut;
 
  while ((SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) && (timeout--)); 	/*!< Loop while DR register in not emplty */
  SPI_I2S_SendData(SPI2, byte);/*!< Send byte through the SPI2 peripheral */
	
  timeout = SPI_Wait_TimeOut;
  while ((SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) && (timeout--));/*!< Wait to receive a byte */
 
  return SPI_I2S_ReceiveData(SPI2); /*!< Return the byte read from the SPI bus */
}


unsigned char SPI2_CS_Select(unsigned char cs)
{
	if(cs==ADT_CS)
	{
		FLASH_CS_H;
		SRAM_CS_H;
		ADT_CS_L;
		return SUCCESS;
	}
	else if(cs==FLASH_CS)
	{
		SRAM_CS_H;
		ADT_CS_H;
		FLASH_CS_L;
		return SUCCESS;
	}
	else if(cs==SRAM_CS)
	{
		ADT_CS_H;
		FLASH_CS_H;
		SRAM_CS_L;
		return SUCCESS;
	}
	else if(cs==SPI2_CS_ALL_H)
	{
		ADT_CS_H;
		FLASH_CS_H;
		SRAM_CS_H;
		return SUCCESS;
	}
	else
	{
		return ERROR;
	}
}












