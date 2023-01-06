#include "main.h"  


//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}

//�ر������ж�(���ǲ�����fault��NMI�ж�)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}

//���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}

//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(unsigned int addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}



volatile unsigned int SysTick_Count = 0;   //Systick����
volatile unsigned int TimingDelay = 0;     //��ʱ��������

/*******************************************************************************
* Function Name  : Delay_us
* Description    : ��ʱ����������ʱ����sleepģʽ
* Input          : ��ʱʱ����us��
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_us(volatile unsigned int us)
{
    unsigned int delta;
    /* �����ʱ������ tick �� */
    us = us * (SysTick->LOAD/(1000000/1000));
    /* ��õ�ǰʱ�� */
    delta = SysTick->VAL;
    /* ѭ����õ�ǰʱ�䣬ֱ���ﵽָ����ʱ����˳�ѭ�� */
    while (delta - SysTick->VAL< us);
}

/*******************************************************************************
* Function Name  : Delay function
* Description    : ��ʱ����������ʱ����sleepģʽ
* Input          : ��ʱʱ����ms��
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_ms(volatile unsigned int nTime)
{ 
	TimingDelay = nTime;
	while(TimingDelay != 0);//__WFI;//�ȴ������н���sleepģʽ
}

/*******************************************************************************
* Function Name  : SysTick init function
* Description    : ���ö�ʱ���ȼ�SysTick���ȼ�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Init_SysTick(void)
{
	SysTick_Config(SystemCoreClock / 1000); //���ö�ʱ���ȣ�1ms
	NVIC_SetPriority(SysTick_IRQn, 0x0);    //SysTick�ж����ȼ�����
}

/*******************************************************************************
* Function Name  : get_rccclock_info
* Description    : ��ȡRCCʱ����Ϣ
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

* Author:�Ժ���
* Function Name  : Bubble_Sort ð������

* ���룺
* *pa:����ָ�룬��Ҫ�����������ͽ����ض���

* len:��������ݳ���
* f:������0����С��������1���Ӵ�С����
* return��None

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












