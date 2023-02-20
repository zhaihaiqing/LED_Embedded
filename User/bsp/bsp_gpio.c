
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
	
	//LED1
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
	
	GPIO_InitStructure.GPIO_Pin = DIGIN_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//普通输出模式
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(DIGIN_GPIO_Port, &GPIO_InitStructure);				//初始化该端口
}










