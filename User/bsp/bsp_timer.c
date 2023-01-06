
#include "main.h"

#define TIM2_PSC	3
#define TIM5_PSC	3

#define TIM2_PWM_FREQ	250
#define TIM5_PWM_FREQ	250

void Timer2_PWM_Init(float Duty)
{
	uint32_t TIM2_CLK=0;
	float Freq=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM5时钟
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
	/*--------------------------------- GPIO Configuration -------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//初始化该端口
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	
	/* -----------------------------------------------------------------------
	TIM2:32bit
    The TIM2 clk=84M/(psc+1)= 21 MHz: 
	TIM2 Freq = TIM2 counter clock/(arr + 1)
	----------------------------------------------------------------------- */
	Freq = TIM2_PWM_FREQ;
	if(Freq <= 0)Freq=1;
	if(Duty < 0)Duty=0;
	if(Duty > 100)Duty=0;
	TIM2_CLK = 84000000/(TIM2_PSC+1);
	TIM_TimeBaseInitStructure.TIM_Period = (TIM2_CLK / Freq)-1; 				//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=TIM2_PSC;  							//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 				//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM5
	
	/* ----------PWM Mode configuration: Channe3--------------------------- */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse =  (Duty/100.0) * ((TIM2_CLK / Freq)-1) ;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
}

/**********************************************************************************
* Function Name  : Timer2_PWM_OC3_Enable
* PWM启动
**********************************************************************************/
void Timer2_PWM_OC3_Enable(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* GPIOA Configuration: Pin_2 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
	
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	 /* TIM5 enable counter */
	TIM_Cmd(TIM2, ENABLE);
}

/**********************************************************************************
* Function Name  : Timer2_PWM_OC3_Disable
* PWM关闭
**********************************************************************************/
void Timer2_PWM_OC3_Disable(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*--------------------------------- GPIO Configuration -------------------------*/
	/* GPIOA Configuration: Pin 2 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_ARRPreloadConfig(TIM2,DISABLE);
	 /* TIM2 disable counter */
	TIM_Cmd(TIM2, DISABLE);
}

/**********************************************************************************
* Function Name  : Timer2_PWM_OC3_SetDuty
* 设置占空比，占空比为实际占空比
**********************************************************************************/
void Timer2_PWM_OC3_SetDuty(float Duty)
{
	uint32_t TIM2_CLK=0;
	float Freq=0;
	
	Freq = TIM2_PWM_FREQ;
	if(Freq <= 0)Freq=1;
	
	TIM2_CLK = 84000000/(TIM2_PSC+1);
	
	TIM_SetCompare3(TIM2,(Duty/100.0) * ((TIM2_CLK / Freq)-1));
}







void Timer5_PWM_Init(float Duty)
{
	uint32_t TIM5_CLK=0;
	float Freq=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///使能TIM5时钟
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
	/*--------------------------------- GPIO Configuration -------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		//复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//初始化该端口
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	
	/* -----------------------------------------------------------------------
	TIM5:32bit
    The TIM5 clk=84M/(psc+1)= 21 MHz: 
	TIM5 Freq = TIM5 counter clock/(arr + 1)
	----------------------------------------------------------------------- */
	Freq = TIM5_PWM_FREQ;
	if(Freq <= 0)Freq=1;
	if(Duty < 0)Duty=0;
	if(Duty > 100)Duty=0;
	TIM5_CLK = 84000000/(TIM5_PSC+1);
	TIM_TimeBaseInitStructure.TIM_Period = (TIM5_CLK / Freq)-1; 				//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=TIM5_PSC;  							//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 				//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//初始化TIM5
	
	/* ----------PWM Mode configuration: Channe2--------------------------- */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse =  (Duty/100.0) * ((TIM5_CLK / Freq)-1) ;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
}

/**********************************************************************************
* Function Name  : Timer5_PWM_OC2_Enable
* PWM启动
**********************************************************************************/
void Timer5_PWM_OC2_Enable(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* GPIOA Configuration: Pin_1 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
	
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM5,ENABLE);
	 /* TIM5 enable counter */
	TIM_Cmd(TIM5, ENABLE);
}

/**********************************************************************************
* Function Name  : Timer5_PWM_OC2_Disable
* PWM关闭
**********************************************************************************/
void Timer5_PWM_OC2_Disable(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*--------------------------------- GPIO Configuration -------------------------*/
	/* GPIOA Configuration: Pin 1 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Disable);
	TIM_ARRPreloadConfig(TIM5,DISABLE);
	 /* TIM5 disable counter */
	TIM_Cmd(TIM5, DISABLE);
}

/**********************************************************************************
* Function Name  : Timer5_PWM_OC2_SetDuty
* 设置占空比，占空比为实际占空比
**********************************************************************************/
void Timer5_PWM_OC2_SetDuty(float Duty)
{
	uint32_t TIM5_CLK=0;
	float Freq=0;
	
	Freq = TIM5_PWM_FREQ;
	if(Freq <= 0)Freq=1;
	
	TIM5_CLK = 84000000/(TIM5_PSC+1);	//计算TIM5的时钟
	
	TIM_SetCompare2(TIM5,(Duty/100.0) * ((TIM5_CLK / Freq)-1));
}






