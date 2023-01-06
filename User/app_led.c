#include "main.h"

sPID_t PID 				= 	{0};


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
	PID.err = 0;//���ֵ
	PID.err_n1 = 0;			//n-1���ֵ
	PID.err_n2 = 0;			//n-2���ֵ
	PID.Kp = 20;
	PID.Ki = 2;
	PID.Kd = 0;
	PID.Incerr = 0;
	PID.UK = 0;
	
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

float PID_Cal(float target_Current,float Actual_current)
{ 
	PID.err = target_Current - Actual_current;
	PID.Incerr = PID.Kp*(PID.err - PID.err_n1) + PID.Ki*PID.err + PID.Kd*(PID.err - 2*PID.err_n1 + PID.err_n2);
	PID.UK += PID.Incerr;
	PID.err_n2 = PID.err_n1;
	PID.err_n1 = PID.err;
	
	if(PID.UK < PIDDUTYMIN)PID.UK = PIDDUTYMIN;			//������Сֵ
	else if(PID.UK > PIDDUTYMAX) PID.UK = PIDDUTYMAX;	//�������ֵ
	
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
//	if(PID.UK < PIDDUTYMIN)PID.UK = PIDDUTYMIN;			//������Сֵ
//	else if(PID.UK > PIDDUTYMAX) PID.UK = PIDDUTYMAX;	//�������ֵ
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
//	if(PID.incrementCurrent < PIDDUTYMIN)PID.incrementCurrent = PIDDUTYMIN;			//������Сֵ
//	else if(PID.incrementCurrent > PIDDUTYMAX) PID.incrementCurrent = PIDDUTYMAX;	//�������ֵ
//	
//	return PID.incrementCurrent;
//}






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








void mled_thread_entry(void *par)
{
	float diff_current=0;
	//����LED�Ƶ�������
	//���ȿ���
	//�¶ȿ���
	
	float led1_current=0,led2_current=0;
	float adjust_val=0,old_val=0;
	
	PID_Parm_init();
	
	
	while(1)
	{
		rt_thread_mdelay(4);					//ˢ���� 200hz
		
		
		get_20times_adc();
		cal_results();
		
		if(extcom_sw_flag == EXTCOM_OFF)		//ʹ��ǰ�����Ʋ���
		{
			//log_info("Use Front parm\r\n");
			
			if(feedback_sw_flag == FEEDBACK_OFF)	//ʹ�õ������з���
			{
				led1_current = FrontPanel_Set_current/100.0;	//ʹ�����õĵ�������λת��ΪA
				led2_current = FrontPanel_Set_current/100.0;				
				
			}
			else
			{
				led1_current = FrontPanel_Set_current;	//����ǿ��ת��Ϊ��Ӧ�ĵ���ֵ��Ȼ�������õ���ֵ���з���
				led2_current = FrontPanel_Set_current;
			}
			
			
			
			if((led_sw_flag == LEDSW_ON)  && (led1_current >= 0.01))	//����򿪿��أ����Ƶ���
			{
				diff_current = sADCCONVData.LED1_Current - led1_current;
				
				
				adjust_val = PID_Cal(led1_current,sADCCONVData.LED1_Current);	//��PID�㷨���������ֵ
				
				log_info("T_C:%.3f,A_C:%.3f,P_C:%.3f\r\n",led1_current,sADCCONVData.LED1_Current,adjust_val);
				
				if(adjust_val - old_val > 1)
				{			//������ֵת��Ϊռ�ձȵı仯��������ֵ����������µ�ռ�ձ�
//					float Duty=0;
//					Duty = adjust_val*84000/10000;	//����ռ�ձ�
					
					old_val = adjust_val;
					Set_LED1_Duty(adjust_val);
				}
				
				//��LED
				//���Ƶ���
			}
			else
			{	
				PID_Parm_init();//�ر�LED
				old_val = 0;
				Set_LED1_Duty(0);
			}
			
		}
		else						//ʹ�ú������Ʋ���
		{
			//log_info("Use Behind parm\r\n");
			
			
			if(sADCCONVData.Exin_Analog_signal<0.01)	//����0.01mA����Ϊ����Ч����
			{
				led1_current = 0;		//0-5V��Ӧ0-5A
				led2_current = 0;
			}
			else
			{
				led1_current = sADCCONVData.Exin_Analog_signal;		//0-5V��Ӧ0-5A
				led2_current = sADCCONVData.Exin_Analog_signal;		//0-5V��Ӧ0-5A
			}
			
			
			//�����ش�LED
			//�½��عر�LED
			
			
			
		}
		
		
		
		
		
		
		
		
		
		
	}
	
}

























