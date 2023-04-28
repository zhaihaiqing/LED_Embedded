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


/*			PID���ù���

1������PID����
2����ʼ��PID����
3������PID����ʱ��
4������PID����
5����PID��������ݷ��ظ����ƺ���

*/


//��ʼ��PID����
void PID_Parm_init(void)
{
	PID1.err = 0;//���ֵ
	PID1.err_n1 = 0;			//n-1���ֵ
	PID1.err_n2 = 0;			//n-2���ֵ
	PID1.Kp = PID_KP;
	PID1.Ki = PID_KI;
	PID1.Kd = PID_KD;
	PID1.Incerr = 0;
	PID1.UK = 0;
	
	PID2.err = 0;//���ֵ
	PID2.err_n1 = 0;			//n-1���ֵ
	PID2.err_n2 = 0;			//n-2���ֵ
	PID2.Kp = PID_KP;
	PID2.Ki = PID_KI;
	PID2.Kd = PID_KD;
	PID2.Incerr = 0;
	PID2.UK = 0;
}

/*			PID
�˿ڲ�����Ŀ��ֵ ʵ�ʲ���ֵ
������ɢPID��ʽ��PWM += Kp[ e(k) - e(k-1)] + Ki*e(k) + Kd[e(k) -2e(k-1)+e(k-2)]
e(k)���������
e(k-1)������һ��ƫ��
e(k-2)

���룺Ŀ��ֵ��ʵ�ʲ���ֵ
�����PID.UK������ֵ
*/

float PID1_Cal(float target_Current,float Actual_current)
{ 
	PID1.err = target_Current - Actual_current;
	PID1.Incerr = PID1.Kp*(PID1.err - PID1.err_n1) + PID1.Ki*PID1.err + PID1.Kd*(PID1.err - 2*PID1.err_n1 + PID1.err_n2);
	PID1.UK += PID1.Incerr;
	PID1.err_n2 = PID1.err_n1;
	PID1.err_n1 = PID1.err;
	
	if(PID1.UK < PIDDUTYMIN)PID1.UK = PIDDUTYMIN;			//������Сֵ
	else if(PID1.UK > PIDDUTYMAX) PID1.UK = PIDDUTYMAX;	//�������ֵ
	
	return PID1.UK;
}

float PID2_Cal(float target_Current,float Actual_current)
{ 
	PID2.err = target_Current - Actual_current;
	PID2.Incerr = PID2.Kp*(PID2.err - PID2.err_n1) + PID2.Ki*PID2.err + PID2.Kd*(PID2.err - 2*PID2.err_n1 + PID2.err_n2);
	PID2.UK += PID2.Incerr;
	PID2.err_n2 = PID2.err_n1;
	PID2.err_n1 = PID2.err;
	
	if(PID2.UK < PIDDUTYMIN)PID2.UK = PIDDUTYMIN;			//������Сֵ
	else if(PID2.UK > PIDDUTYMAX) PID2.UK = PIDDUTYMAX;		//�������ֵ
	
	return PID2.UK;
}
//��ͷ��������

//TIM2��TIM5Ϊ32bit��ʱ����CLK=84M/(psc+1)
//LED1_PWM:PA2��TIM2_CH3	psc=20,clk=4MHz��PWMƵ��=250Hz�������=4M/250=16000
//LED2_PWM:PA1��TIM5_CH2	psc=20,clk=4MHz��PWMƵ��=250Hz�������=4M/250=16000


//TIM2��TIM5Ϊ32bit��ʱ����CLK=84M/(psc+1)
//LED1_PWM:PA2��TIM2_CH3��	psc=3,clk=21MHz��PWMƵ��=250Hz�������=21M/250=84000
//LED2_PWM:PA1��TIM5_CH2	psc=3,clk=21MHz��PWMƵ��=250Hz�������=21M/250=84000


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



struct rt_semaphore cal_adcpid_sem;  //����һ���ź��������յ�һ���ж�ʱ�������ź���


uint8_t IS_LED1_Exist	= 0;
uint8_t IS_LED2_Exist	= 0;

uint16_t OV_Count = 0;
uint16_t OV_Count_Avg = 5;	//�������ִ���

float LED1_I_Current = 0;
float LED2_I_Current = 0;

void mled_thread_entry(void *par)
{
	//����LED�Ƶ�������
	//���ȿ���
	//�¶ȿ���
	
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
	
	rt_sem_init(&cal_adcpid_sem, "cal_adcpid_sem", 0, RT_IPC_FLAG_FIFO); //�����ź���
	
	rt_thread_mdelay(400);
	
	Timer6_Init(999,20);	//clk=84M/(psc+1)	0.25us
	
	if( IS_DIGIN_Rising() )
	{
		ex_led_sw_flag = 1;
	}		
	
	get_1times_adc();
	
	while(1)
	{
		//rt_thread_mdelay(2);					//ˢ���� 
		rt_sem_control(&cal_adcpid_sem, RT_IPC_CMD_RESET, RT_NULL); //�ȴ�ǰ�����ź�������ֹ�����
		rt_sem_take(&cal_adcpid_sem, RT_WAITING_FOREVER);			//�����ȴ��ź���
		
		//LED2_ON();	//ִ��һ�ּ��㣬Լ��ʱ275uS
		
		get_1times_adc();
		//cal_results();
		
		
		//����ͷ�Ƿ���ڣ�ͨ����ͷ�ϵ�NTC������ֵ�����ж�
		if(sADCCONVData.LED1_Temp < 400000)
		{
			IS_LED1_Exist = 1;				//��ͷ1����
		}
		else
		{
			IS_LED1_Exist = 0;
		}
		
		if(sADCCONVData.LED2_Temp < 400000)
		{
			IS_LED2_Exist = 1;				//��ͷ2����
		}
		else
		{
			IS_LED2_Exist = 0;
		}
		
		IS_LED1_Exist =1;
		IS_LED2_Exist =1;
		
		if(led_sw_flag == LEDSW_ON)	//�����LED����
		{
			if(extcom_sw_flag == EXTCOM_OFF)	//ʹ��ǰ�����ť����
			{
				if(feedback_sw_flag == FEEDBACK_OFF)				//ʹ�õ������з���
				{
					led1_current = FrontPanel_Set_current/100.0;	//ʹ�����õĵ�������λת��ΪA
					led2_current = FrontPanel_Set_current/100.0;				
				}
				else												//ʹ�ù�ѧ����
				{
					led1_current = FrontPanel_Set_current/100.0 + 0;	//����ǿ��ת��Ϊ��Ӧ�ĵ���ֵ��Ȼ�������õ���ֵ���з���
					led2_current = FrontPanel_Set_current/100.0 + 0;
				}
				
				if((led1_current >= 0.01) || (led2_current >= 0.01))
				{
					if(IS_LED1_Exist)
					{
						adjust_val1 = PID1_Cal(led1_current,sADCCONVData.LED1_Current);	//��PID�㷨���������ֵ
						//log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val1);
						if( (adjust_val1 - old_val1 > 0.15)  || (old_val1 - adjust_val1 > 0.15))
						{		//������ֵת��Ϊռ�ձȵı仯��������ֵ����������µ�ռ�ձ�
							old_val1 = adjust_val1;
							Set_LED1_Duty(adjust_val1);//��LED//���Ƶ���
						}
					}
				
					if(IS_LED2_Exist)
					{
						adjust_val2 = PID2_Cal(led2_current,sADCCONVData.LED2_Current);	
						if( (adjust_val2 - old_val2 > 0.15)  || (old_val2 - adjust_val2 > 0.15))
						{		//������ֵת��Ϊռ�ձȵı仯��������ֵ����������µ�ռ�ձ�
							old_val2 = adjust_val2;
							Set_LED2_Duty(adjust_val2);//��LED//���Ƶ���
						}
					}
				}
			}
			else								//ʹ�ú����+ǰ�����Ͽ����߼�
			{
				/*	1:�ж�BNC�ź��Ƿ���Ч������0.01��Ч��С�ڵ���0.01��Ч	*/
				if(sADCCONVData.Exin_Analog_signal > 0.01)
				{
					/*	2A:�ж�FB����״̬���رգ�ʹ��BNC��ѹ������ѹ�ź�ת��Ϊ����ֵ���򿪣���BNC��ѹת��Ϊ��Ӧ�Ĺ�ǿ��	*/
					if(feedback_sw_flag == FEEDBACK_OFF)		//ʹ�õ������з���
					{
						led1_current = sADCCONVData.Exin_Analog_signal;	//ʹ��ģ������������ݣ���λת��ΪA
						led2_current = sADCCONVData.Exin_Analog_signal;				
					}
					else										//ʹ�ù�ѧ����
					{
						led1_current = sADCCONVData.Exin_Analog_signal + 0;	//����ǿ��ת��Ϊ��Ӧ�ĵ���ֵ��Ȼ�������õ���ֵ���з���
						led2_current = sADCCONVData.Exin_Analog_signal + 0;
					}
					
					if(led1_current > 5) led1_current =5;
					if(led2_current > 5) led2_current =5;
					
					FrontPanel_Set_current = led1_current*100;
					rt_sem_release(&oled_refresh_sem); //�ͷŲ����ź���������OLED
					
					/*	3A:	�ж�LED1��LED2�Ƿ����,ͬʱ�жϿ����ź��Ƿ�Ϊ�ߣ�������ִ��PID����*/
					if(ex_led_sw_flag == LEDSW_ON)
					{
						if(IS_LED1_Exist)
						{
							adjust_val1 = PID1_Cal(led1_current,sADCCONVData.LED1_Current);	//��PID�㷨���������ֵ
							//log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val1);
				
							if( (adjust_val1 - old_val1 > 0.25)  || (old_val1 - adjust_val1 > 0.25))
							{		//������ֵת��Ϊռ�ձȵı仯��������ֵ����������µ�ռ�ձ�
								old_val1 = adjust_val1;
								Set_LED1_Duty(adjust_val1);//��LED//���Ƶ���
							}
						}
				
						if(IS_LED2_Exist)
						{
							adjust_val2 = PID2_Cal(led2_current,sADCCONVData.LED2_Current);	
							if( (adjust_val2 - old_val2 > 0.25)  || (old_val2 - adjust_val2 > 0.25))
							{		//������ֵת��Ϊռ�ձȵı仯��������ֵ����������µ�ռ�ձ�
								old_val2 = adjust_val2;
								Set_LED2_Duty(adjust_val2);//��LED//���Ƶ���
							}
						}
					}
					else
					{
						LED_OVLD_OFF();
						PID_Parm_init();//�ر�LED
						old_val1 = 0;
						old_val1 = 0;
						Set_LED1_Duty(0);
						Set_LED2_Duty(0);
					}	
				}
				else	//BNC�ź�С����ֵ��ʹ��ǰ������
				{
//					while(FrontPanel_Set_current%5 != 0)
//					{
//						FrontPanel_Set_current--;
//					}
					
					if(feedback_sw_flag == FEEDBACK_OFF)				//ʹ�õ������з���
					{
						led1_current = FrontPanel_Set_current/100.0;	//ʹ�����õĵ�������λת��ΪA
						led2_current = FrontPanel_Set_current/100.0;				
					}
					else												//ʹ�ù�ѧ����
					{
						led1_current = FrontPanel_Set_current/100.0 + 0;	//����ǿ��ת��Ϊ��Ӧ�ĵ���ֵ��Ȼ�������õ���ֵ���з���
						led2_current = FrontPanel_Set_current/100.0 + 0;
					}
				
					if((ex_led_sw_flag == LEDSW_ON)  && ((led1_current >= 0.01) || (led2_current >= 0.01)))
					{
						/*	3B:	�ж�LED1��LED2�Ƿ����,ͬʱ�жϿ����ź��Ƿ�Ϊ�ߣ�������ִ��PID����*/
						if(IS_LED1_Exist)
						{
							adjust_val1 = PID1_Cal(led1_current,sADCCONVData.LED1_Current);	//��PID�㷨���������ֵ
							//log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val1);
							if( (adjust_val1 - old_val1 > 0.15)  || (old_val1 - adjust_val1 > 0.15))
							{		//������ֵת��Ϊռ�ձȵı仯��������ֵ����������µ�ռ�ձ�
								old_val1 = adjust_val1;
								Set_LED1_Duty(adjust_val1);//��LED//���Ƶ���
							}
						}
				
						if(IS_LED2_Exist)
						{
							adjust_val2 = PID2_Cal(led2_current,sADCCONVData.LED2_Current);	
							if( (adjust_val2 - old_val2 > 0.15)  || (old_val2 - adjust_val2 > 0.15))
							{		//������ֵת��Ϊռ�ձȵı仯��������ֵ����������µ�ռ�ձ�
								old_val2 = adjust_val2;
								Set_LED2_Duty(adjust_val2);//��LED//���Ƶ���
							}
						}
					}
					else
					{
						LED_OVLD_OFF();
						PID_Parm_init();//�ر�LED
						old_val1 = 0;
						old_val1 = 0;
						Set_LED1_Duty(0);
						Set_LED2_Duty(0);
					}
				}				
			}	
		}
		else						//�ر�LED
		{
			LED_OVLD_OFF();
			PID_Parm_init();//�ر�LED
			old_val1 = 0;
			old_val1 = 0;
			Set_LED1_Duty(0);
			Set_LED2_Duty(0);
		}
		
		
		
/*******************************************************************************************************/
//		if(extcom_sw_flag == EXTCOM_OFF)				//ʹ��ǰ�����Ʋ���
//		{
//			//log_info("Use Front parm\r\n");	
//			if(feedback_sw_flag == FEEDBACK_OFF)		//ʹ�õ������з���
//			{
//				led1_current = FrontPanel_Set_current/100.0;	//ʹ�����õĵ�������λת��ΪA
//				led2_current = FrontPanel_Set_current/100.0;				
//			}
//			else										//ʹ�ù�ѧ����
//			{
//				led1_current = FrontPanel_Set_current/100.0 + 0;	//����ǿ��ת��Ϊ��Ӧ�ĵ���ֵ��Ȼ�������õ���ֵ���з���
//				led2_current = FrontPanel_Set_current/100.0 + 0;
//			}
//			
//			if((led_sw_flag == LEDSW_ON)  && ((led1_current >= 0.01) || (led2_current >= 0.01)))	//����򿪿��أ����Ƶ���
//			{

//				if(IS_LED1_Exist)
//				{
//					adjust_val1 = PID1_Cal(led1_current,sADCCONVData.LED1_Current);	//��PID�㷨���������ֵ
//					//log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val1);
//				
//					//if( (adjust_val1 - old_val1 > 0.15)  || (old_val1 - adjust_val1 > 0.15))
//					{		//������ֵת��Ϊռ�ձȵı仯��������ֵ����������µ�ռ�ձ�
//						old_val1 = adjust_val1;
//						Set_LED1_Duty(adjust_val1);//��LED//���Ƶ���
//					}
//				}
//				
//				if(IS_LED2_Exist)
//				{
//					adjust_val2 = PID2_Cal(led2_current,sADCCONVData.LED2_Current);	
//					if( (adjust_val2 - old_val2 > 0.15)  || (old_val2 - adjust_val2 > 0.15))
//					{		//������ֵת��Ϊռ�ձȵı仯��������ֵ����������µ�ռ�ձ�
//						old_val2 = adjust_val2;
//						Set_LED2_Duty(adjust_val2);//��LED//���Ƶ���
//					}
//				}
//			}
//			else
//			{	
//				LED_OVLD_OFF();
//				PID_Parm_init();//�ر�LED
//				old_val1 = 0;
//				old_val1 = 0;
//				Set_LED1_Duty(0);
//				Set_LED2_Duty(0);
//			}
//		}
///*******************************************************************************************************/
//		else											//ʹ�ú������Ʋ���
//		{			
//			//log_info("Use Front parm\r\n");	
//			if(feedback_sw_flag == FEEDBACK_OFF)		//ʹ�õ������з���
//			{
//				led1_current = sADCCONVData.Exin_Analog_signal;	//ʹ��ģ������������ݣ���λת��ΪA
//				led2_current = sADCCONVData.Exin_Analog_signal;				
//			}
//			else										//ʹ�ù�ѧ����
//			{
//				led1_current = sADCCONVData.Exin_Analog_signal + 0;	//����ǿ��ת��Ϊ��Ӧ�ĵ���ֵ��Ȼ�������õ���ֵ���з���
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
//			if((ex_led_sw_flag == LEDSW_ON)  && ((led1_current >= 0.01) || (led2_current >= 0.01)))	//����򿪿��أ����Ƶ���
//			{				
//				if(IS_LED1_Exist)
//				{
//					adjust_val1 = PID1_Cal(led1_current,sADCCONVData.LED1_Current);	//��PID�㷨���������ֵ
//					//log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val1);
//				
//					//if( (adjust_val1 - old_val1 > 0.25)  || (old_val1 - adjust_val1 > 0.25))
//					{		//������ֵת��Ϊռ�ձȵı仯��������ֵ����������µ�ռ�ձ�
//						old_val1 = adjust_val1;
//						Set_LED1_Duty(adjust_val1);//��LED//���Ƶ���
//					}
//				}
//				
//				if(IS_LED2_Exist)
//				{
//					//adjust_val2 = PID2_Cal(led2_current,sADCCONVData.LED2_Current);	
//					//if( (adjust_val2 - old_val2 > 0.25)  || (old_val2 - adjust_val2 > 0.25))
//					{		//������ֵת��Ϊռ�ձȵı仯��������ֵ����������µ�ռ�ձ�
//						old_val2 = adjust_val2;
//						Set_LED2_Duty(adjust_val2);//��LED//���Ƶ���
//					}
//				}
//			}
//			else
//			{	
//				LED_OVLD_OFF();
//				PID_Parm_init();//�ر�LED
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
//	//����LED�Ƶ�������
//	//���ȿ���
//	//�¶ȿ���
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
//	rt_sem_init(&cal_adcpid_sem, "cal_adcpid_sem", 0, RT_IPC_FLAG_FIFO); //�����ź���
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
//		//rt_thread_mdelay(2);					//ˢ���� 
//		rt_sem_control(&cal_adcpid_sem, RT_IPC_CMD_RESET, RT_NULL); //�ȴ�ǰ�����ź�������ֹ�����
//		rt_sem_take(&cal_adcpid_sem, RT_WAITING_FOREVER);			//�����ȴ��ź���
//		
//		//LED2_ON();	//ִ��һ�ּ��㣬Լ��ʱ275uS
//		
//		get_1times_adc();
//		//cal_results();
//		
//		
//		//����ͷ�Ƿ���ڣ�ͨ����ͷ�ϵ�NTC������ֵ�����ж�
//		if(sADCCONVData.LED1_Temp < 400000)
//		{
//			IS_LED1_Exist = 1;				//��ͷ1����
//		}
//		else
//		{
//			IS_LED1_Exist = 0;
//		}
//		
//		if(sADCCONVData.LED2_Temp < 400000)
//		{
//			IS_LED2_Exist = 1;				//��ͷ2����
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
//		if(extcom_sw_flag == EXTCOM_OFF)				//ʹ��ǰ�����Ʋ���
//		{
//			//log_info("Use Front parm\r\n");	
//			if(feedback_sw_flag == FEEDBACK_OFF)		//ʹ�õ������з���
//			{
//				led1_current = FrontPanel_Set_current/100.0;	//ʹ�����õĵ�������λת��ΪA
//				led2_current = FrontPanel_Set_current/100.0;				
//			}
//			else										//ʹ�ù�ѧ����
//			{
//				led1_current = FrontPanel_Set_current/100.0 + 0;	//����ǿ��ת��Ϊ��Ӧ�ĵ���ֵ��Ȼ�������õ���ֵ���з���
//				led2_current = FrontPanel_Set_current/100.0 + 0;
//			}
//			
//			if((led_sw_flag == LEDSW_ON)  && ((led1_current >= 0.01) || (led2_current >= 0.01)))	//����򿪿��أ����Ƶ���
//			{
//				//�������//////////////
////				OV_Count++;
////				LED1_I_Current += sADCCONVData.LED1_Current;
////				LED2_I_Current += sADCCONVData.LED2_Current;
////				if(OV_Count >= OV_Count_Avg)
////				{
////					if((LED1_I_Current > (FrontPanel_Set_current*1.2*OV_Count_Avg/100) && IS_LED1_Exist) || (LED2_I_Current > (FrontPanel_Set_current*1.2*OV_Count_Avg/100) && IS_LED2_Exist))		//�������Ƿ��ޣ�������N��ƽ������
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

//				//���¼��//////////////
//				
//				///////////////////////
//				if(IS_LED1_Exist)
//				{
//					adjust_val1 = PID1_Cal(led1_current,sADCCONVData.LED1_Current);	//��PID�㷨���������ֵ
//					//log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val1);
//				
//					//if( (adjust_val1 - old_val1 > 0.15)  || (old_val1 - adjust_val1 > 0.15))
//					{		//������ֵת��Ϊռ�ձȵı仯��������ֵ����������µ�ռ�ձ�
//						old_val1 = adjust_val1;
//						Set_LED1_Duty(adjust_val1);//��LED//���Ƶ���
//					}
//				}
//				
//				if(IS_LED2_Exist)
//				{
//					adjust_val2 = PID2_Cal(led2_current,sADCCONVData.LED2_Current);	
//					if( (adjust_val2 - old_val2 > 0.15)  || (old_val2 - adjust_val2 > 0.15))
//					{		//������ֵת��Ϊռ�ձȵı仯��������ֵ����������µ�ռ�ձ�
//						old_val2 = adjust_val2;
//						Set_LED2_Duty(adjust_val2);//��LED//���Ƶ���
//					}
//				}
//			}
//			else
//			{	
//				LED_OVLD_OFF();
//				PID_Parm_init();//�ر�LED
//				old_val1 = 0;
//				old_val1 = 0;
//				Set_LED1_Duty(0);
//				Set_LED2_Duty(0);
//			}
//		}
///*******************************************************************************************************/
//		else											//ʹ�ú������Ʋ���
//		{			
//			//log_info("Use Front parm\r\n");	
//			if(feedback_sw_flag == FEEDBACK_OFF)		//ʹ�õ������з���
//			{
//				led1_current = sADCCONVData.Exin_Analog_signal;	//ʹ��ģ������������ݣ���λת��ΪA
//				led2_current = sADCCONVData.Exin_Analog_signal;				
//			}
//			else										//ʹ�ù�ѧ����
//			{
//				led1_current = sADCCONVData.Exin_Analog_signal + 0;	//����ǿ��ת��Ϊ��Ӧ�ĵ���ֵ��Ȼ�������õ���ֵ���з���
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
//			if((ex_led_sw_flag == LEDSW_ON)  && ((led1_current >= 0.01) || (led2_current >= 0.01)))	//����򿪿��أ����Ƶ���
//			{
//				//�������//////////////
////				OV_Count++;
////				LED1_I_Current += sADCCONVData.LED1_Current;
////				LED2_I_Current += sADCCONVData.LED2_Current;
////				if(OV_Count >= OV_Count_Avg)
////				{
////					if((LED1_I_Current > (sADCCONVData.Exin_Analog_signal*1.2*OV_Count_Avg) && IS_LED1_Exist) || (LED2_I_Current > (sADCCONVData.Exin_Analog_signal*1.2*OV_Count_Avg) && IS_LED2_Exist))		//�������Ƿ��ޣ�������N��ƽ������,����1.5��
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

//				//���¼��//////////////
//				
//				///////////////////////
//				
//				if(IS_LED1_Exist)
//				{
//					adjust_val1 = PID1_Cal(led1_current,sADCCONVData.LED1_Current);	//��PID�㷨���������ֵ
//					//log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val1);
//				
//					//if( (adjust_val1 - old_val1 > 0.25)  || (old_val1 - adjust_val1 > 0.25))
//					{		//������ֵת��Ϊռ�ձȵı仯��������ֵ����������µ�ռ�ձ�
//						old_val1 = adjust_val1;
//						Set_LED1_Duty(adjust_val1);//��LED//���Ƶ���
//					}
//				}
//				
//				if(IS_LED2_Exist)
//				{
//					//adjust_val2 = PID2_Cal(led2_current,sADCCONVData.LED2_Current);	
//					//if( (adjust_val2 - old_val2 > 0.25)  || (old_val2 - adjust_val2 > 0.25))
//					{		//������ֵת��Ϊռ�ձȵı仯��������ֵ����������µ�ռ�ձ�
//						old_val2 = adjust_val2;
//						Set_LED2_Duty(adjust_val2);//��LED//���Ƶ���
//					}
//				}
//			}
//			else
//			{	
//				LED_OVLD_OFF();
//				PID_Parm_init();//�ر�LED
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

//10K NTC��������ϲ���y=a*x^b+c
float cf_a = 259.6;
float cf_b = -0.1349;
float cf_c = -165.2;

#define STA_Count  100		//״̬���������ִ���

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

		pled_temp1 = cf_a * powf( pled_temp1, cf_b ) + cf_c;		//�����¶�
		pled_temp2 = cf_a * powf( pled_temp2, cf_b ) + cf_c;		//�����¶�
		
		//log_info("LED1_I_Current:%f,LED2_I_Current:%f,pled_temp1:%f,pled_temp2:%f\r\n",pled1_current,pled2_current,pled_temp1,pled_temp2);
		
		//LED�������
		if(extcom_sw_flag == EXTCOM_OFF)				//ʹ��ǰ�����Ʋ���
		{
			if((led_sw_flag == LEDSW_ON )  &&  (FrontPanel_Set_current >= 1) )
			{
				if((pled1_current > (FrontPanel_Set_current*1.2/100) && IS_LED1_Exist) || (pled2_current > (FrontPanel_Set_current*1.2/100) && IS_LED2_Exist))
				{
					rt_thread_mdelay(100);	//��ʱ100ms���ٴβɼ��������ж��Ƿ����
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
		else											//ʹ��ģ���źſ���
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




















