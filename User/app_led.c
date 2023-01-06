#include "main.h"

sPID_t PID 				= 	{0};


/*			PID设置过程

1：设置PID参数
2：初始化PID变量
3：配置PID采样时间
4：进行PID调节
5：将PID输出的数据返回给控制函数

*/

//初始化PID参数
void PID_Parm_init(void)
{
	PID.err = 0;//误差值
	PID.err_n1 = 0;			//n-1误差值
	PID.err_n2 = 0;			//n-2误差值
	PID.Kp = 20;
	PID.Ki = 2;
	PID.Kd = 0;
	PID.Incerr = 0;
	PID.UK = 0;
	
}

/*			PID
人口参数：目标值 实际测量值
根据离散PID公式：PWM += Kp[ e(k) - e(k-1)] + Ki*e(k) + Kd[e(k) -2e(k-1)+e(k-2)]
e(k)代表本次误差
e(k-1)代表上一次偏差
e(k-2)

输入：目标值，实际测量值
输出：PID.UK，增量值
*/

float PID_Cal(float target_Current,float Actual_current)
{ 
	PID.err = target_Current - Actual_current;
	PID.Incerr = PID.Kp*(PID.err - PID.err_n1) + PID.Ki*PID.err + PID.Kd*(PID.err - 2*PID.err_n1 + PID.err_n2);
	PID.UK += PID.Incerr;
	PID.err_n2 = PID.err_n1;
	PID.err_n1 = PID.err;
	
	if(PID.UK < PIDDUTYMIN)PID.UK = PIDDUTYMIN;			//设置最小值
	else if(PID.UK > PIDDUTYMAX) PID.UK = PIDDUTYMAX;	//设置最大值
	
	return PID.UK;
}


//float PID_Cal(float target_Current,float Actual_current)
//{ 
//	PID.err = target_Current - Actual_current;
//	PID.Incerr = PID.Kp*(PID.err - PID.err_n1) + PID.Ki*PID.err + PID.Kd*(PID.err - 2*PID.err_n1 + PID.err_n2);
//	PID.UK += PID.Incerr;
//	PID.err_n2 = PID.err_n1;
//	PID.err_n1 = PID.err;
//	
//	if(PID.UK < PIDDUTYMIN)PID.UK = PIDDUTYMIN;			//设置最小值
//	else if(PID.UK > PIDDUTYMAX) PID.UK = PIDDUTYMAX;	//设置最大值
//	
//	return PID.UK;
//}

//float PID_Cal(float target_Current,float Actual_current)
//{ 
//	PID.err = target_Current - Actual_current;
//	PID.incrementCurrent += PID.Kp*(PID.err - PID.err_n1) + PID.Ki*PID.err + PID.Kd*(PID.err - 2*PID.err_n1 + PID.err_n2);
//	PID.err_n2 = PID.err_n1;
//	PID.err_n1 = PID.err;
//	
//	if(PID.incrementCurrent < PIDDUTYMIN)PID.incrementCurrent = PIDDUTYMIN;			//设置最小值
//	else if(PID.incrementCurrent > PIDDUTYMAX) PID.incrementCurrent = PIDDUTYMAX;	//设置最大值
//	
//	return PID.incrementCurrent;
//}






//灯头控制任务

//TIM2、TIM5为32bit定时器，CLK=84M/(psc+1)
//LED1_PWM:PA2，TIM2_CH3	psc=20,clk=4MHz，PWM频率=250Hz，调光比=4M/250=16000
//LED2_PWM:PA1，TIM5_CH2	psc=20,clk=4MHz，PWM频率=250Hz，调光比=4M/250=16000


//TIM2、TIM5为32bit定时器，CLK=84M/(psc+1)
//LED1_PWM:PA2，TIM2_CH3，	psc=3,clk=21MHz，PWM频率=250Hz，调光比=21M/250=84000
//LED2_PWM:PA1，TIM5_CH2	psc=3,clk=21MHz，PWM频率=250Hz，调光比=21M/250=84000


	
		
		


void LED1_Init(float Duty)
{
	Timer2_PWM_Init(Duty);
}

void Set_LED1_Duty(float Duty)
{
	//log_info("Duty:%f\r\n\r\n",Duty);
	Timer2_PWM_OC3_SetDuty(Duty);
}

void LED1_ENable(void)
{
	Timer2_PWM_OC3_Enable();
}
void LED1_Disable(void)
{
	Timer2_PWM_OC3_Disable();
}


void LED2_Init(float Duty)
{
	Timer5_PWM_Init(Duty);
}


void Set_LED2_Duty(float Duty)
{
	Timer5_PWM_OC2_SetDuty(Duty);
}

void LED2_ENable(void)
{
	Timer5_PWM_OC2_Enable();
}
void LED2_Disable(void)
{
	Timer5_PWM_OC2_Disable();
}





static void set_led1( int argc, char **argv)
{
	char str[32]={0};
	unsigned short reg_val;
	int num=0;
	float val=0;
	uint8_t i=0;
	
	if(argc <2)
	{
		rt_kprintf("Missing parameters\r\n");
	}
	else if(argc >32)
	{
		rt_kprintf("Too many parameters\r\n");
	}
	else
	{
		strcpy(str,argv[1]);
		num=atoi(str);
		val=1.0*num/10.0;
		
		rt_kprintf("val:%d\r\n",(int)val);
		
		Set_LED1_Duty(val);
		
	}
	
}
MSH_CMD_EXPORT(set_led1,set_led1 Duty);








void mled_thread_entry(void *par)
{
	float diff_current=0;
	//包含LED灯电流控制
	//亮度控制
	//温度控制
	
	float led1_current=0,led2_current=0;
	float adjust_val=0,old_val=0;
	
	PID_Parm_init();
	
	
	while(1)
	{
		rt_thread_mdelay(4);					//刷新率 200hz
		
		
		get_20times_adc();
		cal_results();
		
		if(extcom_sw_flag == EXTCOM_OFF)		//使用前面板控制参数
		{
			//log_info("Use Front parm\r\n");
			
			if(feedback_sw_flag == FEEDBACK_OFF)	//使用电流进行反馈
			{
				led1_current = FrontPanel_Set_current/100.0;	//使用设置的电流，单位转换为A
				led2_current = FrontPanel_Set_current/100.0;				
				
			}
			else
			{
				led1_current = FrontPanel_Set_current;	//将光强度转换为对应的电流值，然后再利用电流值进行反馈
				led2_current = FrontPanel_Set_current;
			}
			
			
			
			if((led_sw_flag == LEDSW_ON)  && (led1_current >= 0.01))	//如果打开开关，控制灯亮
			{
				diff_current = sADCCONVData.LED1_Current - led1_current;
				
				
				adjust_val = PID_Cal(led1_current,sADCCONVData.LED1_Current);	//经PID算法计算出调整值
				
				log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val);
				
				if(adjust_val - old_val > 1)
				{			//将调整值转换为占空比的变化量（比例值），计算出新的占空比
//					float Duty=0;
//					Duty = adjust_val*84000/10000;	//计算占空比
					
					old_val = adjust_val;
					Set_LED1_Duty(adjust_val);
				}
				
				//打开LED
				//控制电流
			}
			else
			{	
				PID_Parm_init();//关闭LED
				old_val = 0;
				Set_LED1_Duty(0);
			}
			
		}
		else						//使用后面板控制参数
		{
			//log_info("Use Behind parm\r\n");
			
			
			if(sADCCONVData.Exin_Analog_signal<0.01)	//低于0.01mA，认为是无效数据
			{
				led1_current = 0;		//0-5V对应0-5A
				led2_current = 0;
			}
			else
			{
				led1_current = sADCCONVData.Exin_Analog_signal;		//0-5V对应0-5A
				led2_current = sADCCONVData.Exin_Analog_signal;		//0-5V对应0-5A
			}
			
			
			//上升沿打开LED
			//下降沿关闭LED
			
			
			
		}
		
		
		
		
		
		
		
		
		
		
	}
	
}

























