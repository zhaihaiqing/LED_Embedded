
#ifndef __BSP_UART_H
#define __BSP_UART_H

#define EMPTY 	0xFFFF
#define TIMEOUT 0xFFFF

#define USE_UART7			//是否使用串口7


//定义log_info
#define debug //是否打开打印功能 
#define USE_XSHELL //是否使用xshell输出调试信息

#ifdef debug
//#define log_info(fmt,...)     MyLogInfo(__FILE__" "fmt"\r\n",##__VA_ARGS__)
#define log_info(fmt,...)    printf(fmt,##__VA_ARGS__)

#ifdef USE_XSHELL
 #define LOG_D(fmt,...)      printf(fmt"\r\n",##__VA_ARGS__)
 #define LOG_E(fmt,...)      printf("\33[31m"fmt"\33[0m\r\n",##__VA_ARGS__) //红色
 #define LOG_I(fmt,...)      printf("\33[32m"fmt"\33[0m\r\n",##__VA_ARGS__) //绿色
 #define LOG_W(fmt,...)      printf("\33[33m"fmt"\33[0m\r\n",##__VA_ARGS__) //黄色
#else
 #define LOG_D(fmt,...)      MyLogInfo("[data]"fmt"\r\n",##__VA_ARGS__)
 #define LOG_E(fmt,...)      MyLogInfo("[err]"fmt"\r\n",##__VA_ARGS__)
 #define LOG_I(fmt,...)      MyLogInfo("[info]"fmt"\r\n",##__VA_ARGS__)
 #define LOG_W(fmt,...)      MyLogInfo("[warn]"fmt"\r\n",##__VA_ARGS__)

#endif //USE_XSHELL

//#define LOG_MSG_SIZE 50
//extern struct rt_messagequeue log_pipe; //log管道 

#else 
  #define log_info(fmt,...)   
  #define LOG_D(fmt,...) 
  #define LOG_E(fmt,...) 
  #define LOG_I(fmt,...) 
  #define LOG_W(fmt,...) 
  
#endif //debug







#define UART1_RBUF_SIZE   1024		//要求：2 的整次幂
#if UART1_RBUF_SIZE < 2
#error UART1_RBUF_SIZE is too small.  It must be larger than 1.
#elif ((UART1_RBUF_SIZE & (UART1_RBUF_SIZE-1)) != 0)
#error UART1_RBUF_SIZE must be a power of 2.
#endif

#define UART2_RBUF_SIZE   1024		//要求：2 的整次幂
#if UART2_RBUF_SIZE < 2
#error UART2_RBUF_SIZE is too small.  It must be larger than 1.
#elif ((UART2_RBUF_SIZE & (UART2_RBUF_SIZE-1)) != 0)
#error UART2_RBUF_SIZE must be a power of 2.
#endif

#define UART3_RBUF_SIZE   128		//要求：2 的整次幂
#if UART3_RBUF_SIZE < 2
#error UART3_RBUF_SIZE is too small.  It must be larger than 1.
#elif ((UART3_RBUF_SIZE & (UART3_RBUF_SIZE-1)) != 0)
#error UART3_RBUF_SIZE must be a power of 2.
#endif

#ifdef	USE_UART7
#define UART7_RBUF_SIZE   256		//要求：2 的整次幂
#if UART3_RBUF_SIZE < 2
#error UART3_RBUF_SIZE is too small.  It must be larger than 1.
#elif ((UART3_RBUF_SIZE & (UART3_RBUF_SIZE-1)) != 0)
#error UART3_RBUF_SIZE must be a power of 2.
#endif
#endif






typedef struct uart1_rbuf_st
{
	unsigned int in;							//输入
	unsigned int out;							//输出
	unsigned char  buf [UART1_RBUF_SIZE];		//缓冲区空间
}UART1_RBUF_ST;

typedef struct uart2_rbuf_st
{
	unsigned int in;							//输入
	unsigned int out;							//输出
	unsigned char  buf [UART2_RBUF_SIZE];		//缓冲区空间
}UART2_RBUF_ST;

typedef struct uart3_rbuf_st
{
	unsigned int in;							//输入
	unsigned int out;							//输出
	unsigned char  buf [UART3_RBUF_SIZE];		//缓冲区空间
}UART3_RBUF_ST;

#ifdef	USE_UART7
typedef struct uart7_rbuf_st
{
	unsigned int in;							//输入
	unsigned int out;							//输出
	unsigned char  buf [UART7_RBUF_SIZE];		//缓冲区空间
}UART7_RBUF_ST;
#endif

extern unsigned char    Uart1Flag;
extern UART1_RBUF_ST	uart1_rbuf;

extern unsigned char    Uart2Flag;
extern UART2_RBUF_ST	uart2_rbuf;

extern unsigned char    Uart3Flag;
extern UART3_RBUF_ST	uart3_rbuf;

#ifdef	USE_UART7
extern unsigned char    Uart7Flag;
extern unsigned short uart7_rx_len;
extern UART7_RBUF_ST	uart7_rbuf;
#endif

//void UART1TX_DMA_Configuration( void );
//void UART1_DMA_SendData(unsigned char *BufAddress,unsigned int Length);
void UART1_Configuration(unsigned int baudrate);

void USART_PutChar(USART_TypeDef* USARTx,uint8_t ch);
uint16_t USART7_GetChar(void);

#ifdef	USE_UART7
void UART7_Configuration(unsigned int baudrate);
#endif
//void UART2_Configuration(unsigned int baudrate);
//void UART3_Configuration(unsigned int baudrate);
//void NVIC_UART_Configuration(void);


#endif


