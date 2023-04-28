#include "main.h"

uint8_t extcom_sw_flag = 0;		//控制模式选择，ON：使用后面板的控制信号控制LED，OFF：使用前面板控制信号控制LED
uint8_t led_sw_flag	=	0;		//前面板LED控制信号，ON：打开LED，OFF，关闭LED
uint8_t feedback_sw_flag = 0;	//后面板光强度反馈模式选择，ON：使用反馈，OFF：不使用反馈
__IO uint8_t ex_led_sw_flag	= 0;	//BNC输入的数字信号，1：打开LED，0：关闭LED

void sw_thread_entry(void *par)
{
	uint16_t count = 0;
	
	//初始化检测开关状态，给标志位赋初值,led_sw=ON:0,
	rt_thread_mdelay(5);
	
	while(1)
	{				
		//检测led开关
		if( READ_LED_SW()  && (led_sw_flag != 0)  )	//当状态不发生变化时，不在检测
		{
			rt_thread_mdelay(5);
			if(READ_LED_SW())
			{
				led_sw_flag = 0;	//检测到开关打开，处于打开状态
				log_info("led_sw_flag:%d\r\n",led_sw_flag);
			}
		}
		else if(!READ_LED_SW()  && (led_sw_flag == 0))	//当状态不发生变化时，不在检测
		{
			rt_thread_mdelay(5);
			if(!READ_LED_SW())
			{
				led_sw_flag = 1;	//开关处于闭合状态
				log_info("led_sw_flag:%d\r\n",led_sw_flag);
			}
		}
		
		
		//检测extcom开关
		if( READ_EXCOM_SW()  && (extcom_sw_flag != 0)  )	//当状态不发生变化时，不在检测
		{
			rt_thread_mdelay(5);
			if(READ_EXCOM_SW())
			{
				extcom_sw_flag = 0;	//检测到开关打开，处于打开状态
				log_info("extcom_sw_flag:%d\r\n",extcom_sw_flag);
			}
		}
		else if(!READ_EXCOM_SW()  && (extcom_sw_flag == 0))	//当状态不发生变化时，不在检测
		{
			rt_thread_mdelay(5);
			if(!READ_EXCOM_SW())
			{
				extcom_sw_flag = 1;	//开关处于闭合状态
				log_info("extcom_sw_flag:%d\r\n",extcom_sw_flag);
			}
		}
		
		
		//检测feedback开关
		if( READ_FEEDBACK_SW()  && (feedback_sw_flag != 0)  )	//当状态不发生变化时，不在检测
		{
			rt_thread_mdelay(5);
			if(READ_FEEDBACK_SW())
			{
				feedback_sw_flag = 0;	//检测到开关打开，处于打开状态
				log_info("feedback_sw_flag:%d\r\n",feedback_sw_flag);
			}
		}
		else if(!READ_FEEDBACK_SW()  && (feedback_sw_flag == 0))	//当状态不发生变化时，不在检测
		{
			rt_thread_mdelay(5);
			if(!READ_FEEDBACK_SW())
			{
				feedback_sw_flag = 1;	//开关处于闭合状态
				log_info("feedback_sw_flag:%d\r\n",feedback_sw_flag);
			}
		}
		
		rt_thread_mdelay(10);		//刷新率 100hz
		
	}
}
























