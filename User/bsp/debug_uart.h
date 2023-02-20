
#ifndef __DEBUG_UART_H
#define __DEBUG_UART_H
#include  "main.h"

//定义log_info

#define EN_LOG_UART_RX


#define debug
#ifdef debug
		 #define log_info(...)     printf(__VA_ARGS__)
#else
		#define log_info(...)
#endif


#define LOG_USART			USART6
#define LOG_USART_BPS   	115200


#define LOG_USART_RBUF_SIZE   256		//要求：2 的整次幂
#if LOG_USART_RBUF_SIZE < 2
#error LOG_USART_RBUF_SIZE is too small.  It must be larger than 1.
#elif ((LOG_USART_RBUF_SIZE & (LOG_USART_RBUF_SIZE-1)) != 0)
#error LOG_USART_RBUF_SIZE must be a power of 2.
#endif

#define LOG_USART_TBUF_SIZE   256		//

typedef struct log_usart_rbuf_st
{
	unsigned int 	in;								//输入
	unsigned int 	out;							//输出
	unsigned char	rx_flag;						//接收一帧标志
	unsigned int	rx_len;							//帧长度
	unsigned char  	rx_buf [LOG_USART_RBUF_SIZE];		//接收缓冲区空间
	unsigned char  	tx_buf [LOG_USART_TBUF_SIZE];		//发送缓冲区空间	
}LOG_USART_RBUF_ST;



void debug_usart_Init(void);
void debug_usart_Tx(unsigned char *buf,unsigned char len);
#endif	 
