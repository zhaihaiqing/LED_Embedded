
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
	
//	//LED1
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(LED1_GPIO_Port, &GPIO_InitStructure);		//初始化该端口
	
	//LED2
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(LED2_GPIO_Port, &GPIO_InitStructure);		//初始化该端口
	
	//WDI
	GPIO_InitStructure.GPIO_Pin = WDI_GPIO_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(WDI_GPIO_Port, &GPIO_InitStructure);		//初始化该端口
	
	//LED_RUN
	GPIO_InitStructure.GPIO_Pin = LED_RUN_GPIO_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(LED_RUN_GPIO_Port, &GPIO_InitStructure);		//初始化该端口
	
	//LED_OVLD
	GPIO_InitStructure.GPIO_Pin = LED_OVLD_GPIO_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(LED_OVLD_GPIO_Port, &GPIO_InitStructure);		//初始化该端口
	
	//BEEP
	GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(BEEP_GPIO_Port, &GPIO_InitStructure);		//初始化该端口
	
	//FAN
	GPIO_InitStructure.GPIO_Pin = FAN_GPIO_Pin ;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(FAN_GPIO_Port, &GPIO_InitStructure);		//初始化该端口

	GPIO_InitStructure.GPIO_Pin = EXCOM_SW_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//普通输出模式
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(EXCOM_SW_GPIO_Port, &GPIO_InitStructure);				//初始化该端口
	
	GPIO_InitStructure.GPIO_Pin = LED_SW_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//普通输出模式
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(LED_SW_GPIO_Port, &GPIO_InitStructure);				//初始化该端口
	
	GPIO_InitStructure.GPIO_Pin = FB_SW_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//普通输出模式
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(FB_SW_GPIO_Port, &GPIO_InitStructure);				//初始化该端口
	
	
}



void DIGIN_Iint(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	NVIC_InitTypeDef   		NVIC_InitStructure;
	EXTI_InitTypeDef   		EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = DIGIN_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//普通输出模式
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(DIGIN_GPIO_Port, &GPIO_InitStructure);				//初始化该端口
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);//
	
	/* 配置EXTI_Line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;//LINE
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发一次 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE
	EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	
	EXTI_ClearITPendingBit(EXTI_Line1);
}


/********************************************************************
*	功能	：	旋转编码器解码，即设定值

在收到PB13边沿中断时，根据PB14引脚电平来判定转动方向。

A信号与B信号组合可以判定转动方向：
	A下降沿时，B信号为高电平则为顺时针方向旋转
	
	A下降沿时，B信号为低电平则为逆时针方向旋转

******************************************************************************/



void EXTI1_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		if( IS_DIGIN_Rising() )
		{
			//log_info("DIGIN_Rising\r\n"); 
			ex_led_sw_flag = 1;
		}
		else
		{
			//log_info("DIGIN_trailing\r\n");
			ex_led_sw_flag = 0;
		}	
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
	
	
}







