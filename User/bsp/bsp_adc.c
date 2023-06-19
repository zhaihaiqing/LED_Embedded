
#include "main.h"


void ADC_GPIO_Configuration(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	//NVIC_InitTypeDef   NVIC_InitStructure;
	//EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);				//初始化该端口
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);				//初始化该端口
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//普通输出模式
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);				//初始化该端口
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//普通输出模式
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);				//初始化该端口
	
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource13);//
//	
//	/* 配置EXTI_Line0 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line13;//LINE13
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //上升沿触发一次 
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE13
//	EXTI_Init(&EXTI_InitStructure);//配置
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断0
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级2
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//子优先级2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
//	NVIC_Init(&NVIC_InitStructure);//配置
	
}





//void EXTI15_10_IRQHandler(void)
//{
//	if(EXTI->PR & (1 << 13))	//检测到PB13中断
//	{
//		log_info("123\r\n");

//		EXTI->PR |= 1 << 13;
//	}
//}


//启动ADC转换
void Start_ADC_ConV(void)
{	
	GPIO_SetBits(GPIOE,GPIO_Pin_13);
	__nop();__nop();
	__nop();__nop();
	__nop();__nop();
	__nop();__nop();
//		__nop();
//		__nop();
//		__nop();
//		__nop();
	//Delay_us(2);
	GPIO_ResetBits(GPIOE,GPIO_Pin_13);

	//GPIO_SetBits(GPIOE,GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);	//上升沿启动转换
}

//设置ADC工作模式
void ADC_Config(void)
{
	//工作在硬件控制模式
	//复位ADC
	//设置工作范围
	
	//GPIO_SetBits(GPIOE,GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);//复位过程中，拉高转换引脚
	
	ADC_CONV_H();
	rt_thread_mdelay(10);
	
	GPIO_SetBits(GPIOE,GPIO_Pin_11);	//
	rt_thread_mdelay(100);
	GPIO_ResetBits(GPIOE,GPIO_Pin_11);	
	rt_thread_mdelay(100);
	
	SET_ADC_RANGE_HIGH();
	rt_thread_mdelay(10);
	
	ADC_CONV_L();
	rt_thread_mdelay(10);
	
	ADC_CONV_H();
	rt_thread_mdelay(10);
	
	ADC_CONV_L();
	rt_thread_mdelay(10);	
}







