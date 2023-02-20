
#include "main.h"


LOG_USART_RBUF_ST log_usart_buf	=	{0};

//���ڲ�������
            
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
 
void _sys_exit(int x) 
{ 
	x = x; 
} 
 

int fputc(int ch, FILE *f)
{      
	while((LOG_USART->SR&0X40)==0);
	LOG_USART->DR = (unsigned char) ch;      
	return ch;
}

/*******************************************************************************
* Function Name  : USART6_IRQHandler function
* Description    : debug�����ж�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART6_IRQHandler(void)
{
	unsigned char Uart_Get_Data=0;
	
	LOG_USART_RBUF_ST *p = &log_usart_buf;
	
	if(USART_GetITStatus(LOG_USART, USART_IT_RXNE) != RESET)	//���ڽ����ж�
	{
		Uart_Get_Data = USART_ReceiveData(LOG_USART);
		
		USART_ClearITPendingBit(LOG_USART,USART_IT_RXNE);
		
		if((p->in - p->out)<LOG_USART_RBUF_SIZE)
		{
			p->rx_buf [p->in & (LOG_USART_RBUF_SIZE-1)] = Uart_Get_Data;	
			p->in++;
		}
		p->rx_len = (p->in - p->out) & (LOG_USART_RBUF_SIZE - 1);//��ȡ���ݳ���
	}
	
	else if(USART_GetITStatus(LOG_USART, USART_IT_IDLE) != RESET)	//���ڿ����ж�
	{
		USART_ClearITPendingBit(LOG_USART,USART_FLAG_IDLE);
		p->rx_flag = 1;
	}			
}
									 


void debug_usart_Init(void)
{
   //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART1ʱ��
 
	//���ڶ�Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOG9����ΪUSART6 RX
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOG14����ΪUSART6 TX
	
	//USART�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOG9��GPIOG14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOG,&GPIO_InitStructure); //��ʼ��

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = LOG_USART_BPS;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(LOG_USART, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(LOG_USART, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearITPendingBit(LOG_USART,USART_FLAG_TC);
	
	
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

//	//Usart1 NVIC ����
//	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����1�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����


	
}



/********************************************************************
*  ����   �����ڷ��ͺ���
******************************************************************************/
void debug_usart_Tx(unsigned char *buf,unsigned char len)
{
	u8 i;
	
	for(i=0; i<len; i++)
	{
		LOG_USART->DR  = buf[i];
		while((LOG_USART->SR&0x40) == 0);
	}
}
