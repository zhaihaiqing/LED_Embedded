
#include "main.h"


void Codec_Init(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	NVIC_InitTypeDef   		NVIC_InitStructure;
	EXTI_InitTypeDef   		EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//普通输出模式
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);				//初始化该端口
	
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource9);//
	
	/* 配置EXTI_Line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;//LINE
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发一次 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE
	EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	
	EXTI_ClearITPendingBit(EXTI_Line9);
}




/********************************************************************
*	功能	：	旋转编码器解码，即设定值

在收到PB13边沿中断时，根据PB14引脚电平来判定转动方向。

A信号与B信号组合可以判定转动方向：
	A下降沿时，B信号为高电平则为顺时针方向旋转
	
	A下降沿时，B信号为低电平则为逆时针方向旋转

******************************************************************************/


void EXTI9_5_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		log_info("EXTI9_5_IRQHandler\r\n");
		
		rt_sem_release(&codec_sem); //一轮数据采集完成后，发送采样完成采样信号量
			
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	
	
}









