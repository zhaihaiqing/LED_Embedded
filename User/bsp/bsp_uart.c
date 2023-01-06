/* Includes ------------------------------------------------------------------*/
#include "main.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#define UART_FLAG_TimeOut  0x50000   //超时常量

unsigned char Uart1Flag;
UART1_RBUF_ST	uart1_rbuf	=	{ 0, 0, };

unsigned char Uart2Flag;
UART2_RBUF_ST	uart2_rbuf	=	{ 0, 0, };

unsigned char Uart3Flag;
UART3_RBUF_ST	uart3_rbuf	=	{ 0, 0, };

#ifdef	USE_UART7
unsigned char Uart7Flag=0;
unsigned short uart7_rx_len=0;
UART7_RBUF_ST	uart7_rbuf	=	{ 0, 0, };
#endif


/**********************************************************************************
* Function Name  : UART7_Configuration
* 串口7初始化
* 入口参数
* baudrate:波特率
**********************************************************************************/
void UART7_Configuration(unsigned int baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//打开GPIO和USART2的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);
	
	//串口7对应引脚复用映射
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_UART7); //GPIOE7复用为UART7
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_UART7); //GPIOE8复用为UART7
	
	/* 将UART7_Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baudrate;//设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//设置数据位为8
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//设置停止位为1
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控位
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//发送和接收
	USART_Init(UART7, &USART_InitStructure);
	
	USART_ClearFlag(UART7, USART_FLAG_TC);     /* 清发送标志，Transmission Complete flag */
	//USART_ClearFlag(UART7, USART_FLAG_IDLE); //必须先清除IDLE中断，否则会一直进IDLE中断
	
	USART_ITConfig(UART7, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(UART7, USART_IT_IDLE, ENABLE);  //开启 UART7 总线空闲中断
	
	USART_ITConfig(UART7, USART_IT_ERR, ENABLE);
	USART_ITConfig(UART7, USART_IT_ORE, ENABLE);
	USART_ITConfig(UART7, USART_IT_PE, ENABLE);
	USART_ITConfig(UART7, USART_IT_NE, ENABLE);
	USART_ITConfig(UART7, USART_IT_FE, ENABLE);
	
	//使能
	USART_Cmd(UART7, ENABLE);
	//中断使能
	
	//return 0;
}

/**********************************************************************************
* Function Name  : USART_PutChar
* 串口发送一个字符
* 入口参数
* USARTx:串口号
* ch:数据
**********************************************************************************/
void USART_PutChar(USART_TypeDef* USARTx,uint8_t  ch)
{
  unsigned int timeout = 0;
  USART_SendData(USARTx, (uint8_t)  ch);
  while((USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET) && (timeout++ < UART_FLAG_TimeOut));
}
/**********************************************************************************
* Function Name  : USART_PutData
* 串口发送若干个字符
* 入口参数
* USARTx:串口号
* ch:数据
* len:数据长度
**********************************************************************************/
void USART_PutData(USART_TypeDef* USARTx,unsigned char *dat,unsigned short int len)
{
	unsigned short int i;
	for(i = 0;i < len;i++)USART_PutChar(USARTx,(uint8_t)* (dat++));
}
/**********************************************************************************
* Function Name  : USART_PutS
* 串口发送字符串
* 入口参数
* USARTx:串口号
* *s:数据指针
**********************************************************************************/
void USART_PutS(USART_TypeDef* USARTx,unsigned char *s)
{
	while(*s != '\0')USART_PutChar(USARTx,*(s++));
}


/**********************************************************************************
* 串口1接收字符函数，阻塞模式（接收缓冲区中提取）
**********************************************************************************/
uint16_t USART1_GetCharBlock(uint16_t timeout)
{
	UART1_RBUF_ST *p = &uart1_rbuf;
	uint16_t to = timeout;	
	while(p->out == p->in)if(!(--to))return TIMEOUT;
	return (p->buf [(p->out++) & (UART1_RBUF_SIZE - 1)]);
}

/**********************************************************************************
* 串口1接收字符函数，非阻塞模式（接收缓冲区中提取）
**********************************************************************************/
uint16_t USART1_GetChar(void)
{
	UART1_RBUF_ST *p = &uart1_rbuf;			
	if(p->out == p->in) //缓冲区空条件
		return EMPTY;
	return USART1_GetCharBlock(1000);
}

/**********************************************************************************
* 串口2接收字符函数，阻塞模式（接收缓冲区中提取）
**********************************************************************************/
uint16_t USART2_GetCharBlock(uint16_t timeout)
{
	UART2_RBUF_ST *p = &uart2_rbuf;
	uint16_t to = timeout;	
	while(p->out == p->in)if(!(--to))return TIMEOUT;
	return (p->buf [(p->out++) & (UART2_RBUF_SIZE - 1)]);
}

/**********************************************************************************
* 串口2接收字符函数，非阻塞模式（接收缓冲区中提取）
**********************************************************************************/
uint16_t USART2_GetChar(void)
{
	UART2_RBUF_ST *p = &uart2_rbuf;			
	if(p->out == p->in) //缓冲区空条件
		return EMPTY;
	return USART2_GetCharBlock(1000);
}

/**********************************************************************************
* 串口3接收字符函数，阻塞模式（接收缓冲区中提取）
**********************************************************************************/
uint16_t USART3_GetCharBlock(uint16_t timeout)
{
	UART3_RBUF_ST *p = &uart3_rbuf;
	uint16_t to = timeout;
	while(((p->out - p->in)& (UART3_RBUF_SIZE - 1)) == 0)if(!(--to))return TIMEOUT;
	return (p->buf [(p->out++) & (UART3_RBUF_SIZE - 1)]);
}

/**********************************************************************************
* 串口3接收字符函数，非阻塞模式（接收缓冲区中提取）
**********************************************************************************/
uint16_t USART3_GetChar(void)
{
	UART3_RBUF_ST *p = &uart3_rbuf;		
	if(((p->out - p->in) & (UART3_RBUF_SIZE - 1)) == 0) //缓冲区空条件
		return EMPTY;
	return USART3_GetCharBlock(1000);
}





/**********************************************************************************
* 串口7接收字符函数，阻塞模式（接收缓冲区中提取）
**********************************************************************************/
#ifdef	USE_UART7
uint16_t USART7_GetCharBlock(uint16_t timeout)
{
	UART7_RBUF_ST *p = &uart7_rbuf;
	uint16_t to = timeout;
	while(((p->out - p->in)& (UART7_RBUF_SIZE - 1)) == 0)if(!(--to))return TIMEOUT;
	return (p->buf [(p->out++) & (UART7_RBUF_SIZE - 1)]);
}
#endif

/**********************************************************************************
* 串口3接收字符函数，非阻塞模式（接收缓冲区中提取）
**********************************************************************************/
#ifdef	USE_UART7
uint16_t USART7_GetChar(void)
{
	UART7_RBUF_ST *p = &uart7_rbuf;		
	if(((p->out - p->in) & (UART7_RBUF_SIZE - 1)) == 0) //缓冲区空条件
		return EMPTY;
	return USART7_GetCharBlock(1000);
}
#endif


/**********************************************************************************
清除串口1接收标志位，按帧进行
**********************************************************************************/
void USART1_ClearBuf_Flag(void)
{
	UART1_RBUF_ST *p = &uart1_rbuf;
	p->out = 0;
	p->in = 0;
	
	Uart1Flag = 0;
	//USART1_RX_Len = 0;
}

/**********************************************************************************
清除串口2接收标志位，按帧进行
**********************************************************************************/
void USART2_ClearBuf_Flag(void)
{
	UART2_RBUF_ST *p = &uart2_rbuf;
	p->out = 0;
	p->in = 0;
	
	Uart2Flag = 0;
	//USART2_RX_Len = 0;
	//ModbusDataPackage.DataFlag=0;
	//ModbusDataPackage.DataLen=0;
}

/**********************************************************************************
清除串口3接收标志位，按帧进行
**********************************************************************************/
void USART3_ClearBuf_Flag(void)
{
	UART3_RBUF_ST *p = &uart3_rbuf;
	p->out = 0;
	p->in = 0;
	
	Uart3Flag = 0;
	//USART3_RX_Len = 0;
}

/**********************************************************************************
清除串口7接收标志位，按帧进行
**********************************************************************************/
#ifdef USE_UART7
void USART7_ClearBuf_Flag(void)
{
	UART7_RBUF_ST *p = &uart7_rbuf;
	p->out = 0;
	p->in = 0;
	
	Uart7Flag = 0;
	//USART3_RX_Len = 0;
}
#endif


/**********************************************************************************
printf功能定义，(括号中表示从哪个串口输出数据及数据类型)
**********************************************************************************/
PUTCHAR_PROTOTYPE
{
	USART_PutChar(UART7,(uint8_t) ch);
	return ch;
}






