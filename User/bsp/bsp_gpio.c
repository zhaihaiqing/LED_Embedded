
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(LED1_GPIO_Port, &GPIO_InitStructure);		//��ʼ���ö˿�
	
	//LED2
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(LED2_GPIO_Port, &GPIO_InitStructure);		//��ʼ���ö˿�
	
	//WDI
	GPIO_InitStructure.GPIO_Pin = WDI_GPIO_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(WDI_GPIO_Port, &GPIO_InitStructure);		//��ʼ���ö˿�
	
	//LED_RUN
	GPIO_InitStructure.GPIO_Pin = LED_RUN_GPIO_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(LED_RUN_GPIO_Port, &GPIO_InitStructure);		//��ʼ���ö˿�
	
	//LED_OVLD
	GPIO_InitStructure.GPIO_Pin = LED_OVLD_GPIO_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(LED_OVLD_GPIO_Port, &GPIO_InitStructure);		//��ʼ���ö˿�
	
	//BEEP
	GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(BEEP_GPIO_Port, &GPIO_InitStructure);		//��ʼ���ö˿�
	
	//FAN
	GPIO_InitStructure.GPIO_Pin = FAN_GPIO_Pin ;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(FAN_GPIO_Port, &GPIO_InitStructure);		//��ʼ���ö˿�

	GPIO_InitStructure.GPIO_Pin = EXCOM_SW_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//��ͨ���ģʽ
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(EXCOM_SW_GPIO_Port, &GPIO_InitStructure);				//��ʼ���ö˿�
	
	GPIO_InitStructure.GPIO_Pin = LED_SW_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//��ͨ���ģʽ
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(LED_SW_GPIO_Port, &GPIO_InitStructure);				//��ʼ���ö˿�
	
	GPIO_InitStructure.GPIO_Pin = FB_SW_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//��ͨ���ģʽ
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(FB_SW_GPIO_Port, &GPIO_InitStructure);				//��ʼ���ö˿�
	
	
}



void DIGIN_Iint(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	NVIC_InitTypeDef   		NVIC_InitStructure;
	EXTI_InitTypeDef   		EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = DIGIN_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//��ͨ���ģʽ
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(DIGIN_GPIO_Port, &GPIO_InitStructure);				//��ʼ���ö˿�
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);//
	
	/* ����EXTI_Line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;//LINE
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش���һ�� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE
	EXTI_Init(&EXTI_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//�ⲿ�ж�0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
	
	EXTI_ClearITPendingBit(EXTI_Line1);
}


/********************************************************************
*	����	��	��ת���������룬���趨ֵ

���յ�PB13�����ж�ʱ������PB14���ŵ�ƽ���ж�ת������

A�ź���B�ź���Ͽ����ж�ת������
	A�½���ʱ��B�ź�Ϊ�ߵ�ƽ��Ϊ˳ʱ�뷽����ת
	
	A�½���ʱ��B�ź�Ϊ�͵�ƽ��Ϊ��ʱ�뷽����ת

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







