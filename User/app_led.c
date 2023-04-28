#include "main.h"

//#define PID_KP	20
//#define PID_KI	2
//#define PID_KD	0

//#define PID_KP	30
//#define PID_KI	2
//#define PID_KD	1

//#define PID_KP	50
//#define PID_KI	5
//#define PID_KD	1

#define PID_KP	50
#define PID_KI	5
#define PID_KD	1


sPID_t PID1 				= 	{0};
sPID_t PID2 				= 	{0};


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
	PID1.err = 0;//误差值
	PID1.err_n1 = 0;			//n-1误差值
	PID1.err_n2 = 0;			//n-2误差值
	PID1.Kp = PID_KP;
	PID1.Ki = PID_KI;
	PID1.Kd = PID_KD;
	PID1.Incerr = 0;
	PID1.UK = 0;
	
	PID2.err = 0;//误差值
	PID2.err_n1 = 0;			//n-1误差值
	PID2.err_n2 = 0;			//n-2误差值
	PID2.Kp = PID_KP;
	PID2.Ki = PID_KI;
	PID2.Kd = PID_KD;
	PID2.Incerr = 0;
	PID2.UK = 0;
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

float PID1_Cal(float target_Current,float Actual_current)
{ 
	PID1.err = target_Current - Actual_current;
	PID1.Incerr = PID1.Kp*(PID1.err - PID1.err_n1) + PID1.Ki*PID1.err + PID1.Kd*(PID1.err - 2*PID1.err_n1 + PID1.err_n2);
	PID1.UK += PID1.Incerr;
	PID1.err_n2 = PID1.err_n1;
	PID1.err_n1 = PID1.err;
	
	if(PID1.UK < PIDDUTYMIN)PID1.UK = PIDDUTYMIN;			//设置最小值
	else if(PID1.UK > PIDDUTYMAX) PID1.UK = PIDDUTYMAX;	//设置最大值
	
	return PID1.UK;
}

float PID2_Cal(float target_Current,float Actual_current)
{ 
	PID2.err = target_Current - Actual_current;
	PID2.Incerr = PID2.Kp*(PID2.err - PID2.err_n1) + PID2.Ki*PID2.err + PID2.Kd*(PID2.err - 2*PID2.err_n1 + PID2.err_n2);
	PID2.UK += PID2.Incerr;
	PID2.err_n2 = PID2.err_n1;
	PID2.err_n1 = PID2.err;
	
	if(PID2.UK < PIDDUTYMIN)PID2.UK = PIDDUTYMIN;			//设置最小值
	else if(PID2.UK > PIDDUTYMAX) PID2.UK = PIDDUTYMAX;		//设置最大值
	
	return PID2.UK;
}
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



struct rt_semaphore cal_adcpid_sem;  //创建一个信号量，当收到一次中断时，发送信号量


uint8_t IS_LED1_Exist	= 0;
uint8_t IS_LED2_Exist	= 0;

uint16_t OV_Count = 0;
uint16_t OV_Count_Avg = 5;	//电流积分次数

float LED1_I_Current = 0;
float LED2_I_Current = 0;

void mled_thread_entry(void *par)
{
	//包含LED灯电流控制
	//亮度控制
	//温度控制
	
	float led1_current=0,led2_current=0;
	float adjust_val1=0,old_val1=0;
	float adjust_val2=0,old_val2=0;
	float duty=0;
	
	rt_thread_mdelay(300);
	
	Timer2_PWM_Init(duty);
	Timer5_PWM_Init(duty);
	
	Timer2_PWM_OC3_Enable();
	Timer5_PWM_OC2_Enable();
	
	PID_Parm_init();
	DIGIN_Iint();
	
	rt_sem_init(&cal_adcpid_sem, "cal_adcpid_sem", 0, RT_IPC_FLAG_FIFO); //创建信号量
	
	rt_thread_mdelay(400);
	
	Timer6_Init(999,20);	//clk=84M/(psc+1)	0.25us
	
	if( IS_DIGIN_Rising() )
	{
		ex_led_sw_flag = 1;
	}		
	
	get_1times_adc();
	
	while(1)
	{
		//rt_thread_mdelay(2);					//刷新率 
		rt_sem_control(&cal_adcpid_sem, RT_IPC_CMD_RESET, RT_NULL); //等待前清零信号量，防止误操作
		rt_sem_take(&cal_adcpid_sem, RT_WAITING_FOREVER);			//持续等待信号量
		
		//LED2_ON();	//执行一轮计算，约用时275uS
		
		get_1times_adc();
		//cal_results();
		
		
		//检测灯头是否存在，通过灯头上的NTC电阻阻值进行判定
		if(sADCCONVData.LED1_Temp < 400000)
		{
			IS_LED1_Exist = 1;				//灯头1存在
		}
		else
		{
			IS_LED1_Exist = 0;
		}
		
		if(sADCCONVData.LED2_Temp < 400000)
		{
			IS_LED2_Exist = 1;				//灯头2存在
		}
		else
		{
			IS_LED2_Exist = 0;
		}
		
		IS_LED1_Exist =1;
		IS_LED2_Exist =1;
		
		if(led_sw_flag == LEDSW_ON)	//如果打开LED开关
		{
			if(extcom_sw_flag == EXTCOM_OFF)	//使用前面板旋钮控制
			{
				if(feedback_sw_flag == FEEDBACK_OFF)				//使用电流进行反馈
				{
					led1_current = FrontPanel_Set_current/100.0;	//使用设置的电流，单位转换为A
					led2_current = FrontPanel_Set_current/100.0;				
				}
				else												//使用光学反馈
				{
					led1_current = FrontPanel_Set_current/100.0 + 0;	//将光强度转换为对应的电流值，然后再利用电流值进行反馈
					led2_current = FrontPanel_Set_current/100.0 + 0;
				}
				
				if((led1_current >= 0.01) || (led2_current >= 0.01))
				{
					if(IS_LED1_Exist)
					{
						adjust_val1 = PID1_Cal(led1_current,sADCCONVData.LED1_Current);	//经PID算法计算出调整值
						//log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val1);
						if( (adjust_val1 - old_val1 > 0.15)  || (old_val1 - adjust_val1 > 0.15))
						{		//将调整值转换为占空比的变化量（比例值），计算出新的占空比
							old_val1 = adjust_val1;
							Set_LED1_Duty(adjust_val1);//打开LED//控制电流
						}
					}
				
					if(IS_LED2_Exist)
					{
						adjust_val2 = PID2_Cal(led2_current,sADCCONVData.LED2_Current);	
						if( (adjust_val2 - old_val2 > 0.15)  || (old_val2 - adjust_val2 > 0.15))
						{		//将调整值转换为占空比的变化量（比例值），计算出新的占空比
							old_val2 = adjust_val2;
							Set_LED2_Duty(adjust_val2);//打开LED//控制电流
						}
					}
				}
			}
			else								//使用后面板+前面板组合控制逻辑
			{
				/*	1:判断BNC信号是否有效，大于0.01有效，小于等于0.01无效	*/
				if(sADCCONVData.Exin_Analog_signal > 0.01)
				{
					/*	2A:判断FB开关状态，关闭：使用BNC电压，将电压信号转换为电流值，打开：将BNC电压转换为对应的光强度	*/
					if(feedback_sw_flag == FEEDBACK_OFF)		//使用电流进行反馈
					{
						led1_current = sADCCONVData.Exin_Analog_signal;	//使用模拟测量到的数据，单位转换为A
						led2_current = sADCCONVData.Exin_Analog_signal;				
					}
					else										//使用光学反馈
					{
						led1_current = sADCCONVData.Exin_Analog_signal + 0;	//将光强度转换为对应的电流值，然后再利用电流值进行反馈
						led2_current = sADCCONVData.Exin_Analog_signal + 0;
					}
					
					if(led1_current > 5) led1_current =5;
					if(led2_current > 5) led2_current =5;
					
					FrontPanel_Set_current = led1_current*100;
					rt_sem_release(&oled_refresh_sem); //释放采样信号量，更新OLED
					
					/*	3A:	判断LED1或LED2是否存在,同时判断控制信号是否为高，存在则执行PID调节*/
					if(ex_led_sw_flag == LEDSW_ON)
					{
						if(IS_LED1_Exist)
						{
							adjust_val1 = PID1_Cal(led1_current,sADCCONVData.LED1_Current);	//经PID算法计算出调整值
							//log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val1);
				
							if( (adjust_val1 - old_val1 > 0.25)  || (old_val1 - adjust_val1 > 0.25))
							{		//将调整值转换为占空比的变化量（比例值），计算出新的占空比
								old_val1 = adjust_val1;
								Set_LED1_Duty(adjust_val1);//打开LED//控制电流
							}
						}
				
						if(IS_LED2_Exist)
						{
							adjust_val2 = PID2_Cal(led2_current,sADCCONVData.LED2_Current);	
							if( (adjust_val2 - old_val2 > 0.25)  || (old_val2 - adjust_val2 > 0.25))
							{		//将调整值转换为占空比的变化量（比例值），计算出新的占空比
								old_val2 = adjust_val2;
								Set_LED2_Duty(adjust_val2);//打开LED//控制电流
							}
						}
					}
					else
					{
						LED_OVLD_OFF();
						PID_Parm_init();//关闭LED
						old_val1 = 0;
						old_val1 = 0;
						Set_LED1_Duty(0);
						Set_LED2_Duty(0);
					}	
				}
				else	//BNC信号小于阈值，使用前面板控制
				{
//					while(FrontPanel_Set_current%5 != 0)
//					{
//						FrontPanel_Set_current--;
//					}
					
					if(feedback_sw_flag == FEEDBACK_OFF)				//使用电流进行反馈
					{
						led1_current = FrontPanel_Set_current/100.0;	//使用设置的电流，单位转换为A
						led2_current = FrontPanel_Set_current/100.0;				
					}
					else												//使用光学反馈
					{
						led1_current = FrontPanel_Set_current/100.0 + 0;	//将光强度转换为对应的电流值，然后再利用电流值进行反馈
						led2_current = FrontPanel_Set_current/100.0 + 0;
					}
				
					if((ex_led_sw_flag == LEDSW_ON)  && ((led1_current >= 0.01) || (led2_current >= 0.01)))
					{
						/*	3B:	判断LED1或LED2是否存在,同时判断控制信号是否为高，存在则执行PID调节*/
						if(IS_LED1_Exist)
						{
							adjust_val1 = PID1_Cal(led1_current,sADCCONVData.LED1_Current);	//经PID算法计算出调整值
							//log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val1);
							if( (adjust_val1 - old_val1 > 0.15)  || (old_val1 - adjust_val1 > 0.15))
							{		//将调整值转换为占空比的变化量（比例值），计算出新的占空比
								old_val1 = adjust_val1;
								Set_LED1_Duty(adjust_val1);//打开LED//控制电流
							}
						}
				
						if(IS_LED2_Exist)
						{
							adjust_val2 = PID2_Cal(led2_current,sADCCONVData.LED2_Current);	
							if( (adjust_val2 - old_val2 > 0.15)  || (old_val2 - adjust_val2 > 0.15))
							{		//将调整值转换为占空比的变化量（比例值），计算出新的占空比
								old_val2 = adjust_val2;
								Set_LED2_Duty(adjust_val2);//打开LED//控制电流
							}
						}
					}
					else
					{
						LED_OVLD_OFF();
						PID_Parm_init();//关闭LED
						old_val1 = 0;
						old_val1 = 0;
						Set_LED1_Duty(0);
						Set_LED2_Duty(0);
					}
				}				
			}	
		}
		else						//关闭LED
		{
			LED_OVLD_OFF();
			PID_Parm_init();//关闭LED
			old_val1 = 0;
			old_val1 = 0;
			Set_LED1_Duty(0);
			Set_LED2_Duty(0);
		}
		
		
		
/*******************************************************************************************************/
//		if(extcom_sw_flag == EXTCOM_OFF)				//使用前面板控制参数
//		{
//			//log_info("Use Front parm\r\n");	
//			if(feedback_sw_flag == FEEDBACK_OFF)		//使用电流进行反馈
//			{
//				led1_current = FrontPanel_Set_current/100.0;	//使用设置的电流，单位转换为A
//				led2_current = FrontPanel_Set_current/100.0;				
//			}
//			else										//使用光学反馈
//			{
//				led1_current = FrontPanel_Set_current/100.0 + 0;	//将光强度转换为对应的电流值，然后再利用电流值进行反馈
//				led2_current = FrontPanel_Set_current/100.0 + 0;
//			}
//			
//			if((led_sw_flag == LEDSW_ON)  && ((led1_current >= 0.01) || (led2_current >= 0.01)))	//如果打开开关，控制灯亮
//			{

//				if(IS_LED1_Exist)
//				{
//					adjust_val1 = PID1_Cal(led1_current,sADCCONVData.LED1_Current);	//经PID算法计算出调整值
//					//log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val1);
//				
//					//if( (adjust_val1 - old_val1 > 0.15)  || (old_val1 - adjust_val1 > 0.15))
//					{		//将调整值转换为占空比的变化量（比例值），计算出新的占空比
//						old_val1 = adjust_val1;
//						Set_LED1_Duty(adjust_val1);//打开LED//控制电流
//					}
//				}
//				
//				if(IS_LED2_Exist)
//				{
//					adjust_val2 = PID2_Cal(led2_current,sADCCONVData.LED2_Current);	
//					if( (adjust_val2 - old_val2 > 0.15)  || (old_val2 - adjust_val2 > 0.15))
//					{		//将调整值转换为占空比的变化量（比例值），计算出新的占空比
//						old_val2 = adjust_val2;
//						Set_LED2_Duty(adjust_val2);//打开LED//控制电流
//					}
//				}
//			}
//			else
//			{	
//				LED_OVLD_OFF();
//				PID_Parm_init();//关闭LED
//				old_val1 = 0;
//				old_val1 = 0;
//				Set_LED1_Duty(0);
//				Set_LED2_Duty(0);
//			}
//		}
///*******************************************************************************************************/
//		else											//使用后面板控制参数
//		{			
//			//log_info("Use Front parm\r\n");	
//			if(feedback_sw_flag == FEEDBACK_OFF)		//使用电流进行反馈
//			{
//				led1_current = sADCCONVData.Exin_Analog_signal;	//使用模拟测量到的数据，单位转换为A
//				led2_current = sADCCONVData.Exin_Analog_signal;				
//			}
//			else										//使用光学反馈
//			{
//				led1_current = sADCCONVData.Exin_Analog_signal + 0;	//将光强度转换为对应的电流值，然后再利用电流值进行反馈
//				led2_current = sADCCONVData.Exin_Analog_signal + 0;
//			}
//			
//			if(led1_current < 0.01) led1_current =0;
//			else if(led1_current > 5) led1_current =5;
//			
//			if(led2_current < 0.01) led2_current =0;
//			else if(led2_current > 5) led2_current =5;
//			
//			//log_info("Use Front parm,LED1_Current:%f,ex_led_sw_flag:%d\r\n",led1_current,ex_led_sw_flag);
//			//ex_led_sw_flag =1;
//			
//			if((ex_led_sw_flag == LEDSW_ON)  && ((led1_current >= 0.01) || (led2_current >= 0.01)))	//如果打开开关，控制灯亮
//			{				
//				if(IS_LED1_Exist)
//				{
//					adjust_val1 = PID1_Cal(led1_current,sADCCONVData.LED1_Current);	//经PID算法计算出调整值
//					//log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val1);
//				
//					//if( (adjust_val1 - old_val1 > 0.25)  || (old_val1 - adjust_val1 > 0.25))
//					{		//将调整值转换为占空比的变化量（比例值），计算出新的占空比
//						old_val1 = adjust_val1;
//						Set_LED1_Duty(adjust_val1);//打开LED//控制电流
//					}
//				}
//				
//				if(IS_LED2_Exist)
//				{
//					//adjust_val2 = PID2_Cal(led2_current,sADCCONVData.LED2_Current);	
//					//if( (adjust_val2 - old_val2 > 0.25)  || (old_val2 - adjust_val2 > 0.25))
//					{		//将调整值转换为占空比的变化量（比例值），计算出新的占空比
//						old_val2 = adjust_val2;
//						Set_LED2_Duty(adjust_val2);//打开LED//控制电流
//					}
//				}
//			}
//			else
//			{	
//				LED_OVLD_OFF();
//				PID_Parm_init();//关闭LED
//				old_val1 = 0;
//				old_val1 = 0;
//				Set_LED1_Duty(0);
//				Set_LED2_Duty(0);
//			}	
//		}
		
		//LED2_OFF();
		
	}
	
}


//void mled_thread_entry(void *par)
//{
//	//包含LED灯电流控制
//	//亮度控制
//	//温度控制
//	
//	float led1_current=0,led2_current=0;
//	float adjust_val1=0,old_val1=0;
//	float adjust_val2=0,old_val2=0;
//	float duty=0;
//	
//	rt_thread_mdelay(300);
//	
//	Timer2_PWM_Init(duty);
//	Timer5_PWM_Init(duty);
//	
//	Timer2_PWM_OC3_Enable();
//	Timer5_PWM_OC2_Enable();
//	
//	PID_Parm_init();
//	DIGIN_Iint();
//	
//	rt_sem_init(&cal_adcpid_sem, "cal_adcpid_sem", 0, RT_IPC_FLAG_FIFO); //创建信号量
//	
//	rt_thread_mdelay(400);
//	
//	Timer6_Init(999,20);	//clk=84M/(psc+1)	0.25us
//	
//	if( IS_DIGIN_Rising() )
//	{
//		ex_led_sw_flag = 1;
//	}		
//	
//	get_1times_adc();
//	
//	while(1)
//	{
//		//rt_thread_mdelay(2);					//刷新率 
//		rt_sem_control(&cal_adcpid_sem, RT_IPC_CMD_RESET, RT_NULL); //等待前清零信号量，防止误操作
//		rt_sem_take(&cal_adcpid_sem, RT_WAITING_FOREVER);			//持续等待信号量
//		
//		//LED2_ON();	//执行一轮计算，约用时275uS
//		
//		get_1times_adc();
//		//cal_results();
//		
//		
//		//检测灯头是否存在，通过灯头上的NTC电阻阻值进行判定
//		if(sADCCONVData.LED1_Temp < 400000)
//		{
//			IS_LED1_Exist = 1;				//灯头1存在
//		}
//		else
//		{
//			IS_LED1_Exist = 0;
//		}
//		
//		if(sADCCONVData.LED2_Temp < 400000)
//		{
//			IS_LED2_Exist = 1;				//灯头2存在
//		}
//		else
//		{
//			IS_LED2_Exist = 0;
//		}
//		
//		IS_LED1_Exist =1;
//		IS_LED2_Exist =1;
//		
///*******************************************************************************************************/
//		if(extcom_sw_flag == EXTCOM_OFF)				//使用前面板控制参数
//		{
//			//log_info("Use Front parm\r\n");	
//			if(feedback_sw_flag == FEEDBACK_OFF)		//使用电流进行反馈
//			{
//				led1_current = FrontPanel_Set_current/100.0;	//使用设置的电流，单位转换为A
//				led2_current = FrontPanel_Set_current/100.0;				
//			}
//			else										//使用光学反馈
//			{
//				led1_current = FrontPanel_Set_current/100.0 + 0;	//将光强度转换为对应的电流值，然后再利用电流值进行反馈
//				led2_current = FrontPanel_Set_current/100.0 + 0;
//			}
//			
//			if((led_sw_flag == LEDSW_ON)  && ((led1_current >= 0.01) || (led2_current >= 0.01)))	//如果打开开关，控制灯亮
//			{
//				//过流检测//////////////
////				OV_Count++;
////				LED1_I_Current += sADCCONVData.LED1_Current;
////				LED2_I_Current += sADCCONVData.LED2_Current;
////				if(OV_Count >= OV_Count_Avg)
////				{
////					if((LED1_I_Current > (FrontPanel_Set_current*1.2*OV_Count_Avg/100) && IS_LED1_Exist) || (LED2_I_Current > (FrontPanel_Set_current*1.2*OV_Count_Avg/100) && IS_LED2_Exist))		//检查电流是否超限，连续记N次平均电流
////					{
////						LED_OVLD_ON();
////						//log_info("LED1_I_Current:%f,LED2_I_Current:%f\r\n",LED1_I_Current/OV_Count_Avg,LED2_I_Current/OV_Count_Avg);
////					}
////					else
////					{
////						LED_OVLD_OFF();
////					}
////					OV_Count = 0;
////					LED1_I_Current = 0;
////					LED2_I_Current = 0;
////				}

//				//高温检测//////////////
//				
//				///////////////////////
//				if(IS_LED1_Exist)
//				{
//					adjust_val1 = PID1_Cal(led1_current,sADCCONVData.LED1_Current);	//经PID算法计算出调整值
//					//log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val1);
//				
//					//if( (adjust_val1 - old_val1 > 0.15)  || (old_val1 - adjust_val1 > 0.15))
//					{		//将调整值转换为占空比的变化量（比例值），计算出新的占空比
//						old_val1 = adjust_val1;
//						Set_LED1_Duty(adjust_val1);//打开LED//控制电流
//					}
//				}
//				
//				if(IS_LED2_Exist)
//				{
//					adjust_val2 = PID2_Cal(led2_current,sADCCONVData.LED2_Current);	
//					if( (adjust_val2 - old_val2 > 0.15)  || (old_val2 - adjust_val2 > 0.15))
//					{		//将调整值转换为占空比的变化量（比例值），计算出新的占空比
//						old_val2 = adjust_val2;
//						Set_LED2_Duty(adjust_val2);//打开LED//控制电流
//					}
//				}
//			}
//			else
//			{	
//				LED_OVLD_OFF();
//				PID_Parm_init();//关闭LED
//				old_val1 = 0;
//				old_val1 = 0;
//				Set_LED1_Duty(0);
//				Set_LED2_Duty(0);
//			}
//		}
///*******************************************************************************************************/
//		else											//使用后面板控制参数
//		{			
//			//log_info("Use Front parm\r\n");	
//			if(feedback_sw_flag == FEEDBACK_OFF)		//使用电流进行反馈
//			{
//				led1_current = sADCCONVData.Exin_Analog_signal;	//使用模拟测量到的数据，单位转换为A
//				led2_current = sADCCONVData.Exin_Analog_signal;				
//			}
//			else										//使用光学反馈
//			{
//				led1_current = sADCCONVData.Exin_Analog_signal + 0;	//将光强度转换为对应的电流值，然后再利用电流值进行反馈
//				led2_current = sADCCONVData.Exin_Analog_signal + 0;
//			}
//			
//			if(led1_current < 0.01) led1_current =0;
//			else if(led1_current > 5) led1_current =5;
//			
//			if(led2_current < 0.01) led2_current =0;
//			else if(led2_current > 5) led2_current =5;
//			
//			//log_info("Use Front parm,LED1_Current:%f,ex_led_sw_flag:%d\r\n",led1_current,ex_led_sw_flag);
//			
//			//ex_led_sw_flag =1;
//			
//			if((ex_led_sw_flag == LEDSW_ON)  && ((led1_current >= 0.01) || (led2_current >= 0.01)))	//如果打开开关，控制灯亮
//			{
//				//过流检测//////////////
////				OV_Count++;
////				LED1_I_Current += sADCCONVData.LED1_Current;
////				LED2_I_Current += sADCCONVData.LED2_Current;
////				if(OV_Count >= OV_Count_Avg)
////				{
////					if((LED1_I_Current > (sADCCONVData.Exin_Analog_signal*1.2*OV_Count_Avg) && IS_LED1_Exist) || (LED2_I_Current > (sADCCONVData.Exin_Analog_signal*1.2*OV_Count_Avg) && IS_LED2_Exist))		//检查电流是否超限，连续记N次平均电流,超过1.5倍
////					{
////						LED_OVLD_ON();
////						//log_info("LED1_I_Current:%f,LED2_I_Current:%f,sADCCONVData.Exin_Analog_signal:%f\r\n",LED1_I_Current/OV_Count_Avg,LED2_I_Current/OV_Count_Avg,sADCCONVData.Exin_Analog_signal);
////					}
////					else
////					{
////						LED_OVLD_OFF();
////					}
////					OV_Count = 0;
////					LED1_I_Current = 0;
////					LED2_I_Current = 0;
////				}

//				//高温检测//////////////
//				
//				///////////////////////
//				
//				if(IS_LED1_Exist)
//				{
//					adjust_val1 = PID1_Cal(led1_current,sADCCONVData.LED1_Current);	//经PID算法计算出调整值
//					//log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val1);
//				
//					//if( (adjust_val1 - old_val1 > 0.25)  || (old_val1 - adjust_val1 > 0.25))
//					{		//将调整值转换为占空比的变化量（比例值），计算出新的占空比
//						old_val1 = adjust_val1;
//						Set_LED1_Duty(adjust_val1);//打开LED//控制电流
//					}
//				}
//				
//				if(IS_LED2_Exist)
//				{
//					//adjust_val2 = PID2_Cal(led2_current,sADCCONVData.LED2_Current);	
//					//if( (adjust_val2 - old_val2 > 0.25)  || (old_val2 - adjust_val2 > 0.25))
//					{		//将调整值转换为占空比的变化量（比例值），计算出新的占空比
//						old_val2 = adjust_val2;
//						Set_LED2_Duty(adjust_val2);//打开LED//控制电流
//					}
//				}
//			}
//			else
//			{	
//				LED_OVLD_OFF();
//				PID_Parm_init();//关闭LED
//				old_val1 = 0;
//				old_val1 = 0;
//				Set_LED1_Duty(0);
//				Set_LED2_Duty(0);
//			}	
//		}
//		
//		//LED2_OFF();
//		
//	}
//	
//}


uint8_t IS_PLED1_OV_TEMP = 0;
uint8_t IS_PLED2_OV_TEMP = 0;

//10K NTC，曲线拟合参数y=a*x^b+c
float cf_a = 259.6;
float cf_b = -0.1349;
float cf_c = -165.2;

#define STA_Count  100		//状态检测电流积分次数

void pled_monitor_thread_entry(void *par)
{
	float pled_temp1=0,pled_temp2=0;
	
	float pled_light=0;
	
	float pled1_current=0,pled2_current=0;
	
	
	uint32_t ix=0;
	
	rt_thread_mdelay(1000);
	
	while(1)
	{	
		pled_temp1 = sADCCONVData.LED1_Temp;
		pled_temp2 = sADCCONVData.LED2_Temp;
			
		pled_light = sADCCONVData.LED1_Light_Intensity;
			
		pled1_current = sADCCONVData.LED1_Current;
		pled2_current = sADCCONVData.LED2_Current;
			

		pled_temp1 = (pled_temp1/1000);
		pled_temp2 = (pled_temp2/1000);
		pled_light = pled_light;

		pled_temp1 = cf_a * powf( pled_temp1, cf_b ) + cf_c;		//计算温度
		pled_temp2 = cf_a * powf( pled_temp2, cf_b ) + cf_c;		//计算温度
		
		//log_info("LED1_I_Current:%f,LED2_I_Current:%f,pled_temp1:%f,pled_temp2:%f\r\n",pled1_current,pled2_current,pled_temp1,pled_temp2);
		
		//LED过流检测
		if(extcom_sw_flag == EXTCOM_OFF)				//使用前面板控制参数
		{
			if((led_sw_flag == LEDSW_ON )  &&  (FrontPanel_Set_current >= 1) )
			{
				if((pled1_current > (FrontPanel_Set_current*1.2/100) && IS_LED1_Exist) || (pled2_current > (FrontPanel_Set_current*1.2/100) && IS_LED2_Exist))
				{
					rt_thread_mdelay(100);	//延时100ms，再次采集电流，判断是否过流
					pled1_current = sADCCONVData.LED1_Current;
					pled2_current = sADCCONVData.LED2_Current;
					if((pled1_current > (FrontPanel_Set_current*1.2/100) && IS_LED1_Exist) || (pled2_current > (FrontPanel_Set_current*1.2/100) && IS_LED2_Exist))
					{
						LED_OVLD_ON();
						log_info("LED1_I_Current:%f,LED2_I_Current:%f\r\n",pled1_current,pled2_current);
					}
					else
					{
						LED_OVLD_OFF();
					}
					
				}
				else
				{
					LED_OVLD_OFF();
				}
			}
			else
			{
				LED_OVLD_OFF();
			}
		}
		else											//使用模拟信号控制
		{
			if( (ex_led_sw_flag == LEDSW_ON) && (sADCCONVData.Exin_Analog_signal >= 0.01) )
			{
				if((pled1_current > (sADCCONVData.Exin_Analog_signal*1.2) && IS_LED1_Exist) || (pled2_current > (sADCCONVData.Exin_Analog_signal*1.2) && IS_LED2_Exist))
				{
					rt_thread_mdelay(100);
					pled1_current = sADCCONVData.LED1_Current;
					pled2_current = sADCCONVData.LED2_Current;
					if((pled1_current > (sADCCONVData.Exin_Analog_signal*1.2) && IS_LED1_Exist) || (pled2_current > (sADCCONVData.Exin_Analog_signal*1.2) && IS_LED2_Exist))
					{
						LED_OVLD_ON();
						log_info("LED1_I_Current:%f,LED2_I_Current:%f\r\n",pled1_current,pled2_current);
					}
					else
					{
						LED_OVLD_OFF();
					}
					
				}
				else
				{
					LED_OVLD_OFF();
				}
			}
			else
			{
				LED_OVLD_OFF();
			}
		}
		rt_thread_mdelay(50);
	}	
}




















