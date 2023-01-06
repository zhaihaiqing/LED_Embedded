#include "main.h"  


//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}

//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}

//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}

//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(unsigned int addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}



volatile unsigned int SysTick_Count = 0;   //Systick计数
volatile unsigned int TimingDelay = 0;     //延时函数计数

/*******************************************************************************
* Function Name  : Delay_us
* Description    : 延时函数，空闲时进入sleep模式
* Input          : 延时时长（us）
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_us(volatile unsigned int us)
{
    unsigned int delta;
    /* 获得延时经过的 tick 数 */
    us = us * (SysTick->LOAD/(1000000/1000));
    /* 获得当前时间 */
    delta = SysTick->VAL;
    /* 循环获得当前时间，直到达到指定的时间后退出循环 */
    while (delta - SysTick->VAL< us);
}

/*******************************************************************************
* Function Name  : Delay function
* Description    : 延时函数，空闲时进入sleep模式
* Input          : 延时时长（ms）
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_ms(volatile unsigned int nTime)
{ 
	TimingDelay = nTime;
	while(TimingDelay != 0);//__WFI;//等待过程中进入sleep模式
}

/*******************************************************************************
* Function Name  : SysTick init function
* Description    : 设置定时长度及SysTick优先级
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Init_SysTick(void)
{
	SysTick_Config(SystemCoreClock / 1000); //设置定时长度，1ms
	NVIC_SetPriority(SysTick_IRQn, 0x0);    //SysTick中断优先级设置
}

/*******************************************************************************
* Function Name  : get_rccclock_info
* Description    : 获取RCC时钟信息
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void get_rccclock_info(void)
{
	//char str[32]={0};
	RCC_ClocksTypeDef  RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	
	log_info("SYSCLK_Frequency:%.3fMHz\r\n",RCC_Clocks.SYSCLK_Frequency/1000000.0);
	log_info("HCLK_Frequency:%.3fMHz\r\n",RCC_Clocks.HCLK_Frequency/1000000.0);
	log_info("PCLK1_Frequency:%.3fMHz\r\n",RCC_Clocks.PCLK1_Frequency/1000000.0);
	log_info("PCLK2_Frequency:%.3fMHz\r\n",RCC_Clocks.PCLK2_Frequency/1000000.0);
	
}



/*******************************************************************************

* Author:翟海青
* Function Name  : Bubble_Sort 冒泡排序

* 输入：
* *pa:数据指针，需要根据数据类型进行重定义

* len:排序的数据长度
* f:排序方向，0：从小到大排序，1：从大到小排序
* return：None

*******************************************************************************/
void Bubble_Sort(float *pa,unsigned int len, unsigned char f)
{
	unsigned int i=0,j=0;
	float buf=0;

	for(i=0;i<len;i++)
		for(j=i+1;j<len;j++)
			if(f==0)
			{
				if(*(pa+i)>*(pa+j))
				{
					buf=*(pa+i);
					*(pa+i)=*(pa+j);
					*(pa+j)=buf;
				}
			}
			else if(f==1)
			{
				if(*(pa+i)<*(pa+j))
				{
					buf=*(pa+i);
					*(pa+i)=*(pa+j);
					*(pa+j)=buf;
				}
			}
}












