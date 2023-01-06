
#include "main.h"


//void GPIO_Configuration(void)
//{
//	GPIO_InitTypeDef        GPIO_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
//	GPIO_Init(GPIOA, &GPIO_InitStructure);				//初始化该端口
//	
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
//	GPIO_Init(GPIOC, &GPIO_InitStructure);				//初始化该端口
//}


//void LED_ON(unsigned char bits)
//{
////	if(bits&LED1)   GPIO_ResetBits(GPIOE,GPIO_Pin_9);
////	if(bits&LED2)   GPIO_ResetBits(GPIOB,GPIO_Pin_2);
////	if(bits&LED3)   GPIO_ResetBits(GPIOB,GPIO_Pin_1);
////	if(bits&LED4)   GPIO_ResetBits(GPIOB,GPIO_Pin_0);
//	
//	if(bits&LED1)   GPIOE->BSRRH = GPIO_Pin_9;
//	if(bits&LED2)   GPIOB->BSRRH = GPIO_Pin_2;
//	if(bits&LED3)   GPIOB->BSRRH = GPIO_Pin_1;
//	if(bits&LED4)   GPIOB->BSRRH = GPIO_Pin_0;
//}

//void LED_OFF(unsigned char bits)
//{
////	if(bits&LED1)   GPIO_SetBits(GPIOE,GPIO_Pin_9);
////	if(bits&LED2)   GPIO_SetBits(GPIOB,GPIO_Pin_2);
////	if(bits&LED3)   GPIO_SetBits(GPIOB,GPIO_Pin_1);
////	if(bits&LED4)   GPIO_SetBits(GPIOB,GPIO_Pin_0);
//	
//	if(bits&LED1)   GPIOE->BSRRL = GPIO_Pin_9;
//	if(bits&LED2)   GPIOB->BSRRL = GPIO_Pin_2;
//	if(bits&LED3)   GPIOB->BSRRL = GPIO_Pin_1;
//	if(bits&LED4)   GPIOB->BSRRL = GPIO_Pin_0;
//}








