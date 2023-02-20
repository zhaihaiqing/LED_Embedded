
#include "main.h"


LOG_USART_RBUF_ST log_usart_buf	=	{0};

//串口操作变量
            
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
* Description    : debug串口中断
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART6_IRQHandler(void)
{
	unsigned char Uart_Get_Data=0;
	
	LOG_USART_RBUF_ST *p = &log_usart_buf;
	
	if(USART_GetITStatus(LOG_USART, USART_IT_RXNE) != RESET)	//串口接收中断
	{
		Uart_Get_Data = USART_ReceiveData(LOG_USART);
		
		USART_ClearITPendingBit(LOG_USART,USART_IT_RXNE);
		
		if((p->in - p->out)<LOG_USART_RBUF_SIZE)
		{
			p->rx_buf [p->in & (LOG_USART_RBUF_SIZE-1)] = Uart_Get_Data;	
			p->in++;
		}
		p->rx_len = (p->in - p->out) & (LOG_USART_RBUF_SIZE - 1);//获取数据长度
	}
	
	else if(USART_GetITStatus(LOG_USART, USART_IT_IDLE) != RESET)	//串口空闲中断
	{
		USART_ClearITPendingBit(LOG_USART,USART_FLAG_IDLE);
		p->rx_flag = 1;
	}			
}
									 


void debug_usart_Init(void)
{
   //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART1时钟
 
	//串口对应引脚复用映射
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOG9复用为USART6 RX
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOG14复用为USART6 TX
	
	//USART端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOG9与GPIOG14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = LOG_USART_BPS;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(LOG_USART, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(LOG_USART, ENABLE);  //使能串口1 
	
	USART_ClearITPendingBit(LOG_USART,USART_FLAG_TC);
	
	
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

//	//Usart1 NVIC 配置
//	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口1中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、


	
}



/********************************************************************
*  功能   ：串口发送函数
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
