
#include "main.h"


void GPIO_Configuration(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
//	NVIC_InitTypeDef   NVIC_InitStructure;
//	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//初始化该端口
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);				//初始化该端口
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);				//初始化该端口
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//普通输出模式
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//初始化该端口
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//普通输出模式
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);				//初始化该端口
	
	
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6);//
//	
//	/* 配置EXTI_Line6 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line6;//LINE13
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //上升沿触发一次 
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE13
//	EXTI_Init(&EXTI_InitStructure);//配置
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断0
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级2
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//子优先级2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
//	NVIC_Init(&NVIC_InitStructure);//配置
}

//void EXTI9_5_IRQHandler(void)
//{
//	if(EXTI->PR & (1 << 6))	//检测到PA6中断
//	{
//		log_info("IO-LIGHT-SEL_CPU\r\n");

//		EXTI->PR |= 1 << 6;
//	}
//}









