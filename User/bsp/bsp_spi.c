
#include "main.h"

#define SPI_Wait_TimeOut  		0xffff     //超时常量 0x5000	

/*******************************************************************************
* Function Name  : SPI3_Config
* Description    : STM32 SPI1配置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI3_Config(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);													//打开外设时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF , ENABLE);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);                     						//设置为外设功能，此处不能合并
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
	
	DA_CS1_H();
	DA_CS2_H();
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;											//设置端口模式，
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                						//外设复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;														//推挽模式
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;														//默认下拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;													//GPIO翻转速度为100MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);																//将配置写入对应的GPIO寄存器中
	
	

	//CS
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;													//设置SPI片选
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;																//端口输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;																//推挽模式
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;																//默认上拉，禁止AD片选
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;															//GPIO翻转速度为100MHz
	GPIO_Init(GPIOF, &GPIO_InitStructure);																		//将配置写入对应的GPIO寄存器中 
	
	
	/* SPI3 configuration */
	SPI_Cmd(SPI3, DISABLE); 
	SPI_I2S_DeInit(SPI3);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//串行同步时钟的第1个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;//定义波特率预分频的值:波特率预分频值为2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI3, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
	SPI_Cmd(SPI3, ENABLE); 
}

/*******************************************************************************
* Function Name  : SPI1_ReadWriteByte
* Description    : 从SPI总线上读取或写一个字节数据
* Input          : 写字节
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char SPI3_ReadWriteByte(unsigned char byte)
{
  unsigned int timeout = SPI_Wait_TimeOut;
 
  while ((SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) && (timeout--)); 	/*!< Loop while DR register in not emplty */
  SPI_I2S_SendData(SPI3, byte);/*!< Send byte through the SPI2 peripheral */
	
  timeout = SPI_Wait_TimeOut;
  while ((SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET) && (timeout--));/*!< Wait to receive a byte */
 
  return SPI_I2S_ReceiveData(SPI3); /*!< Return the byte read from the SPI bus */
}

void SPI3_WriteByte(unsigned char byte)
{
  unsigned int timeout = SPI_Wait_TimeOut;
 
  while(  ( (SPI3->SR & SPI_I2S_FLAG_TXE) == RESET )  && (timeout--) );
  SPI3->DR = byte;
  SPI3->SR&=(1<<0);
	
  timeout = SPI_Wait_TimeOut;
  
  while(  ( (SPI3->SR & SPI_I2S_FLAG_BSY) == SPI_I2S_FLAG_BSY )  && (timeout--) );
}


void AD5542_WriteA( float val)
{
	unsigned short reg_val;
	
	if(val>1250)val=1250;
	
	reg_val=(uint16_t)(val*65535.0/2500.0);
	
	DA_CS1_L();
	
	//log_info("val:%f,reg_val:%d\r\n",val,reg_val);
	
	SPI3_ReadWriteByte( (reg_val>>8) );
	SPI3_ReadWriteByte( reg_val);       
	
	DA_CS1_H();
}

void AD5542_WriteB( float val)
{
	unsigned short reg_val;
	
	if(val>1250)val=1250;
	
	reg_val=(uint16_t)(val*65535.0/2500.0);
	
	DA_CS2_L();
	
	//log_info("val:%f,reg_val:%d\r\n",val,reg_val);
	
	SPI3_ReadWriteByte( (reg_val>>8) );
	SPI3_ReadWriteByte( reg_val);       
	
	DA_CS2_H();
}












