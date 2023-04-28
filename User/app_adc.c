#include "main.h"

//static float ADC_gain[6]={0};



void adc_thread_entry(void *par)
{
	ADC_GPIO_Configuration();
	rt_thread_mdelay(50);
	ADC_Config();	//����ADC����ģʽ
	//����ʱ������ʱ����ADC
	//�����ⲿ�ж�
	//�����ź���
	
	rt_thread_mdelay(50);
//	ADC_gain[0] = ADC_CH1_GAIN;
//	ADC_gain[1] = ADC_CH2_GAIN;
//	ADC_gain[2] = ADC_CH3_GAIN;
//	ADC_gain[3] = ADC_CH4_GAIN;
//	ADC_gain[4] = ADC_CH5_GAIN;
//	ADC_gain[5] = ADC_CH6_GAIN;
	
	get_adc();		//��ǰת�����Σ����⾭δת�������ݽ������
	rt_thread_mdelay(10);
	get_adc();
	rt_thread_mdelay(10);
	
	while(1)
	{
//		if(led_sw_flag != LEDSW_ON)
//		{
//			get_10times_adc();
//			cal_results();
////			log_info("Exin_Analog_signal:%fV,LED2_Current:%fA,LED2_Temp:%f,LED1_Light_Intensity:%f,LED1_Current:%fA,LED1_Temp:%f\r\n",\
////			sADCCONVData.Exin_Analog_signal,sADCCONVData.LED2_Current,sADCCONVData.LED2_Temp,sADCCONVData.LED1_Light_Intensity,sADCCONVData.LED1_Current,sADCCONVData.LED1_Temp);
//		}
		
		rt_thread_mdelay(2000);
	}
}

/*
ADC ch1---->LED2_Current
ADC ch2---->LED1_Current
ADC ch3---->LED2_Temp
ADC ch4---->LED1_Temp
ADC ch5---->Light_intensity
ADC ch6---->Analog_Input
*/

static float value[6]={0};
void get_adc(void)
{
	uint8_t i=0;
	uint16_t adc_value[6]={0};
	
	ADC_RD_H();
	Start_ADC_ConV(ADC_CONV_ALL);	//ת������ͨ��
	//Delay_us(5);					//�ȴ��ź���
	while(IS_ADC_BUSY())
	{
		;
	}
		
	for(i=0;i<6;i++)
	{
		ADC_RD_H();
		__nop();__nop();
		ADC_RD_L();
		__nop();__nop();
		adc_value[i] = GPIOD->IDR & 0XFFFF;	//��ȡADC����
		if((adc_value[i] & 0x8000) != 0x8000)
		{
			value[i] = ((float)adc_value[i]) * ADC_RANGE / 0x7fff;
		}
		else
		{
			value[i] = 0-((float)(0xffff - adc_value[i])) * ADC_RANGE / 0x7fff;
		}
		//value[i] = value[i]/ADC_gain[i];
	}
	
	//rt_memcpy((void *)&sADCCONVData,value,sizeof(value));
	
	//log_info("adc_ch1:0x%x, ch2:0x%x, ch3:0x%x, ch4:0x%x, ch5:0x%x, ch6:0x%x, \r\n",adc_value[0],adc_value[1],adc_value[2],adc_value[3],adc_value[4],adc_value[5]);
	//log_info("adc_ch1:%fv ch2:%fv ch3:%fv ch4:%fv ch5:%fv ch6:%fv \r\n",value[0],value[1],value[2],value[3],value[4],value[5]);	
	//log_info("Exin_Analog_signal:%f,LED2_Current:%f,LED2_Temp:%f,LED1_Light_Intensity:%f,LED1_Current:%f,LED1_Temp:%f\r\n",sADCCONVData.Exin_Analog_signal,sADCCONVData.LED2_Current,sADCCONVData.LED2_Temp,sADCCONVData.LED1_Light_Intensity,sADCCONVData.LED1_Current,sADCCONVData.LED1_Temp);	
}


void get_10times_adc(void)	//��ȡ10�����ݣ���ȥ�����Сֵ��ȡ�м�6��������ƽ��
{
	float value_A[6][10]={0};
	float value_b[6]={0};
	uint8_t i=0;
	
	for(i=0;i<10;i++)
	{
		get_adc();
		value_A[0][i] = value[0];
		value_A[1][i] = value[1];
		value_A[2][i] = value[2];
		value_A[3][i] = value[3];
		value_A[4][i] = value[4];
		value_A[5][i] = value[5];
	}
	
	for(i=0;i<6;i++)
	{
		Bubble_Sort(value_A[i],10,0);
	}
	
	for(i=0;i<6;i++)
	{
		value_b[0]  +=  value_A[0][i+2];
		value_b[1]  +=  value_A[1][i+2];
		value_b[2]  +=  value_A[2][i+2];
		value_b[3]  +=  value_A[3][i+2];
		value_b[4]  +=  value_A[4][i+2];
		value_b[5]  +=  value_A[5][i+2];
	}
	
/*
	ADC ch0---->LED2_Current
	ADC ch1---->LED1_Current
	ADC ch2---->LED2_Temp
	ADC ch3---->LED1_Temp
	ADC ch4---->Light_intensity
	ADC ch5---->Analog_Input
*/
	sADCCONVData.Exin_Analog_signal 	= 	value_b[5]/6;
	sADCCONVData.LED2_Current 			= 	value_b[0]/6;
	sADCCONVData.LED2_Temp 				= 	value_b[2]/6;
	sADCCONVData.LED1_Light_Intensity 	= 	value_b[4]/6;
	sADCCONVData.LED1_Current 			= 	value_b[1]/6;
	sADCCONVData.LED1_Temp 				= 	value_b[3]/6;
	
	sADCCONVData.Exin_Analog_signal 	= 	sADCCONVData.Exin_Analog_signal		/	Analog_Input_Gain;
	sADCCONVData.LED2_Current 			= 	sADCCONVData.LED2_Current			/	LED2_Current_Gain;
	sADCCONVData.LED2_Temp 				= 	sADCCONVData.LED2_Temp				/	LED2_Temp_Gain;
	sADCCONVData.LED1_Light_Intensity 	= 	sADCCONVData.LED1_Light_Intensity	/	Light_intensity_Gain;
	sADCCONVData.LED1_Current 			= 	sADCCONVData.LED1_Current			/	LED1_Current_Gain;
	sADCCONVData.LED1_Temp 				= 	sADCCONVData.LED1_Temp				/	LED1_Temp_Gain;
}


void get_5times_adc(void)	//��ȡ5�����ݣ���ȥ�����Сֵ��ȡ�м�3��������ƽ��
{
	float value_A[6][5]={0};
	float value_b[6]={0};
	
	float value_max[6]={0};
	float value_min[6]={0};
	uint8_t i=0;
	
	for(i=0;i<5;i++)
	{
		get_adc();
		value_A[0][i] = value[0];
		value_A[1][i] = value[1];
		value_A[2][i] = value[2];
		value_A[3][i] = value[3];
		value_A[4][i] = value[4];
		value_A[5][i] = value[5];
	}
	
	for(i=0;i<5;i++)
	{
		value_b[0]  +=  value_A[0][i];
		value_b[1]  +=  value_A[1][i];
		value_b[2]  +=  value_A[2][i];
		value_b[3]  +=  value_A[3][i];
		value_b[4]  +=  value_A[4][i];
		value_b[5]  +=  value_A[5][i];
	}
	
	for(i=0;i<6;i++)
	{
		f_max_min(value_A[i],5,&value_max[i],&value_min[i]);
	}
	
	for(i=0;i<6;i++)
	{
		value_b[i] -= value_max[i];
		value_b[i] -= value_min[i];
	}
	
	
/*
	ADC ch0---->LED2_Current
	ADC ch1---->LED1_Current
	ADC ch2---->LED2_Temp
	ADC ch3---->LED1_Temp
	ADC ch4---->Light_intensity
	ADC ch5---->Analog_Input
*/
	sADCCONVData.Exin_Analog_signal 	= 	value_b[5]/3;
	sADCCONVData.LED2_Current 			= 	value_b[0]/3;
	sADCCONVData.LED2_Temp 				= 	value_b[2]/3;
	sADCCONVData.LED1_Light_Intensity 	= 	value_b[4]/3;
	sADCCONVData.LED1_Current 			= 	value_b[1]/3;
	sADCCONVData.LED1_Temp 				= 	value_b[3]/3;
	
	sADCCONVData.Exin_Analog_signal 	= 	sADCCONVData.Exin_Analog_signal		/	Analog_Input_Gain;
	sADCCONVData.LED2_Current 			= 	sADCCONVData.LED2_Current			/	LED2_Current_Gain;
	sADCCONVData.LED2_Temp 				= 	sADCCONVData.LED2_Temp				/	LED2_Temp_Gain;
	sADCCONVData.LED1_Light_Intensity 	= 	sADCCONVData.LED1_Light_Intensity	/	Light_intensity_Gain;
	sADCCONVData.LED1_Current 			= 	sADCCONVData.LED1_Current			/	LED1_Current_Gain;
	sADCCONVData.LED1_Temp 				= 	sADCCONVData.LED1_Temp				/	LED1_Temp_Gain;
}




static float r3=20000;
static float r4=0.0,A=0.0;
static float V=2.5;

void get_1times_adc(void)	//��ȡ5�����ݣ���ȥ�����Сֵ��ȡ�м�3��������ƽ��
{	
	
	get_adc();
	sADCCONVData.LED2_Current = value[0];
	sADCCONVData.LED1_Current = value[1];
	sADCCONVData.LED2_Temp = value[2];
	sADCCONVData.LED1_Temp = value[3];
	sADCCONVData.LED1_Light_Intensity = value[4];
	sADCCONVData.Exin_Analog_signal = value[5];
	
/*
	ADC ch0---->LED2_Current
	ADC ch1---->LED1_Current
	ADC ch2---->LED2_Temp
	ADC ch3---->LED1_Temp
	ADC ch4---->Light_intensity
	ADC ch5---->Analog_Input
*/

	
	sADCCONVData.Exin_Analog_signal 	= 	sADCCONVData.Exin_Analog_signal		/	Analog_Input_Gain;
	sADCCONVData.LED2_Current 			= 	sADCCONVData.LED2_Current			/	LED2_Current_Gain;
	sADCCONVData.LED2_Temp 				= 	sADCCONVData.LED2_Temp				/	LED2_Temp_Gain;
	sADCCONVData.LED1_Light_Intensity 	= 	sADCCONVData.LED1_Light_Intensity	/	Light_intensity_Gain;
	sADCCONVData.LED1_Current 			= 	sADCCONVData.LED1_Current			/	LED1_Current_Gain;
	sADCCONVData.LED1_Temp 				= 	sADCCONVData.LED1_Temp				/	LED1_Temp_Gain;
	
	sADCCONVData.LED2_Current = sADCCONVData.LED2_Current/LED2_CURRENT_RES;
	sADCCONVData.LED1_Current = sADCCONVData.LED1_Current/LED1_CURRENT_RES;
	
	
	A=0.5-sADCCONVData.LED2_Temp/V;
	r4=A*r3/(1-A);
	sADCCONVData.LED2_Temp = r4;				//���������ֵ
		
	A=0.5-sADCCONVData.LED1_Temp/V;
	r4=A*r3/(1-A);
	sADCCONVData.LED1_Temp = r4;				//���������ֵ	
}


void cal_results(void)
{
	//��������������Ϊʵ�ʵ�������
	
	//ch1Ϊ�ⲿ�����ģ���ѹֵ������Ҫ����ת��
	
	//ch2ΪLED2��ͷ�ϵĵ���ֵ����Ҫ����
	//ch3ΪLED2��ͷ�ϵ��¶�ֵ����Ҫ����
	//ch4ΪLED1�Ĺ�ǿ�ȣ���Ҫ����
	//ch5ΪLED1��ͷ�ϵĵ���ֵ����Ҫ����
	//ch6ΪLED1��ͷ�ϵ��¶�ֵ����Ҫ����
	
	sADCCONVData.LED2_Current = sADCCONVData.LED2_Current/LED2_CURRENT_RES;
	sADCCONVData.LED1_Current = sADCCONVData.LED1_Current/LED1_CURRENT_RES;
	
	{
		//float r1=10000,r2=10000,r3=20000;
		
		
		//A=r2/(r1+r2)-sADCCONVData.LED2_Temp/V;
		A=0.5-sADCCONVData.LED2_Temp/V;
		r4=A*r3/(1-A);
		sADCCONVData.LED2_Temp = r4;				//���������ֵ
		
		//A=r2/(r1+r2)-sADCCONVData.LED1_Temp/V;
		A=0.5-sADCCONVData.LED1_Temp/V;
		r4=A*r3/(1-A);
		sADCCONVData.LED1_Temp = r4;				//���������ֵ
	}
	
	//log_info("Exin_Analog_signal:%fV,LED2_Current:%fA,LED2_Temp:%f,LED1_Light_Intensity:%f,LED1_Current:%fA,LED1_Temp:%f\r\n",sADCCONVData.Exin_Analog_signal,sADCCONVData.LED2_Current,sADCCONVData.LED2_Temp,sADCCONVData.LED1_Light_Intensity,sADCCONVData.LED1_Current,sADCCONVData.LED1_Temp);
	
}






















