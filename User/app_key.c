#include "main.h"

uint8_t extcom_sw_flag = 0;		//����ģʽѡ��ON��ʹ�ú����Ŀ����źſ���LED��OFF��ʹ��ǰ�������źſ���LED
uint8_t led_sw_flag	=	0;		//ǰ���LED�����źţ�ON����LED��OFF���ر�LED
uint8_t feedback_sw_flag = 0;	//������ǿ�ȷ���ģʽѡ��ON��ʹ�÷�����OFF����ʹ�÷���
__IO uint8_t ex_led_sw_flag	= 0;	//BNC����������źţ�1����LED��0���ر�LED

void sw_thread_entry(void *par)
{
	uint16_t count = 0;
	
	//��ʼ����⿪��״̬������־λ����ֵ,led_sw=ON:0,
	rt_thread_mdelay(5);
	
	while(1)
	{				
		//���led����
		if( READ_LED_SW()  && (led_sw_flag != 0)  )	//��״̬�������仯ʱ�����ڼ��
		{
			rt_thread_mdelay(5);
			if(READ_LED_SW())
			{
				led_sw_flag = 0;	//��⵽���ش򿪣����ڴ�״̬
				log_info("led_sw_flag:%d\r\n",led_sw_flag);
			}
		}
		else if(!READ_LED_SW()  && (led_sw_flag == 0))	//��״̬�������仯ʱ�����ڼ��
		{
			rt_thread_mdelay(5);
			if(!READ_LED_SW())
			{
				led_sw_flag = 1;	//���ش��ڱպ�״̬
				log_info("led_sw_flag:%d\r\n",led_sw_flag);
			}
		}
		
		
		//���extcom����
		if( READ_EXCOM_SW()  && (extcom_sw_flag != 0)  )	//��״̬�������仯ʱ�����ڼ��
		{
			rt_thread_mdelay(5);
			if(READ_EXCOM_SW())
			{
				extcom_sw_flag = 0;	//��⵽���ش򿪣����ڴ�״̬
				log_info("extcom_sw_flag:%d\r\n",extcom_sw_flag);
			}
		}
		else if(!READ_EXCOM_SW()  && (extcom_sw_flag == 0))	//��״̬�������仯ʱ�����ڼ��
		{
			rt_thread_mdelay(5);
			if(!READ_EXCOM_SW())
			{
				extcom_sw_flag = 1;	//���ش��ڱպ�״̬
				log_info("extcom_sw_flag:%d\r\n",extcom_sw_flag);
			}
		}
		
		
		//���feedback����
		if( READ_FEEDBACK_SW()  && (feedback_sw_flag != 0)  )	//��״̬�������仯ʱ�����ڼ��
		{
			rt_thread_mdelay(5);
			if(READ_FEEDBACK_SW())
			{
				feedback_sw_flag = 0;	//��⵽���ش򿪣����ڴ�״̬
				log_info("feedback_sw_flag:%d\r\n",feedback_sw_flag);
			}
		}
		else if(!READ_FEEDBACK_SW()  && (feedback_sw_flag == 0))	//��״̬�������仯ʱ�����ڼ��
		{
			rt_thread_mdelay(5);
			if(!READ_FEEDBACK_SW())
			{
				feedback_sw_flag = 1;	//���ش��ڱպ�״̬
				log_info("feedback_sw_flag:%d\r\n",feedback_sw_flag);
			}
		}
		
		rt_thread_mdelay(10);		//ˢ���� 100hz
		
	}
}
























