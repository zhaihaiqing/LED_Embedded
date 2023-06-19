
#include "main.h"

#define SPI_Wait_TimeOut  		0xffff     //��ʱ���� 0x5000	

/*******************************************************************************
* Function Name  : SPI3_Config
* Description    : STM32 SPI1����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI3_Config(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);													//������ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF , ENABLE);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);                     						//����Ϊ���蹦�ܣ��˴����ܺϲ�
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
	
	DA_CS1_H();
	DA_CS2_H();
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;											//���ö˿�ģʽ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                						//���踴��ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;														//����ģʽ
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;														//Ĭ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;													//GPIO��ת�ٶ�Ϊ100MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);																//������д���Ӧ��GPIO�Ĵ�����
	
	

	//CS
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;													//����SPIƬѡ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;																//�˿����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;																//����ģʽ
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;																//Ĭ����������ֹADƬѡ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;															//GPIO��ת�ٶ�Ϊ100MHz
	GPIO_Init(GPIOF, &GPIO_InitStructure);																		//������д���Ӧ��GPIO�Ĵ����� 
	
	
	/* SPI3 configuration */
	SPI_Cmd(SPI3, DISABLE); 
	SPI_I2S_DeInit(SPI3);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//����ͬ��ʱ�ӵĵ�1�������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI3, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	
	SPI_Cmd(SPI3, ENABLE); 
}

/*******************************************************************************
* Function Name  : SPI1_ReadWriteByte
* Description    : ��SPI�����϶�ȡ��дһ���ֽ�����
* Input          : д�ֽ�
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












