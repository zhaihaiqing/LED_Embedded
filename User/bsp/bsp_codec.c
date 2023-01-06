
#include "main.h"


void Codec_Init(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	NVIC_InitTypeDef   		NVIC_InitStructure;
	EXTI_InitTypeDef   		EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//��ͨ���ģʽ
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);				//��ʼ���ö˿�
	
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource9);//
	
	/* ����EXTI_Line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;//LINE
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش���һ�� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE
	EXTI_Init(&EXTI_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�ⲿ�ж�0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
	
	EXTI_ClearITPendingBit(EXTI_Line9);
}




/********************************************************************
*	����	��	��ת���������룬���趨ֵ

���յ�PB13�����ж�ʱ������PB14���ŵ�ƽ���ж�ת������

A�ź���B�ź���Ͽ����ж�ת������
	A�½���ʱ��B�ź�Ϊ�ߵ�ƽ��Ϊ˳ʱ�뷽����ת
	
	A�½���ʱ��B�ź�Ϊ�͵�ƽ��Ϊ��ʱ�뷽����ת

******************************************************************************/


void EXTI9_5_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		log_info("EXTI9_5_IRQHandler\r\n");
		
		rt_sem_release(&codec_sem); //һ�����ݲɼ���ɺ󣬷��Ͳ�����ɲ����ź���
			
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	
	
}









