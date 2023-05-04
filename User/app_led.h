#ifndef __APP_LED_H
#define __APP_LED_H
#include "main.h"

#define LED1_UPLIMIT_TEMP	60	//定义LED灯温度上限，超过上限后的处理待定
#define LED2_UPLIMIT_TEMP	60


#define EXTCOM_OFF 		0
#define EXTCOM_ON		1

#define FEEDBACK_OFF 	0
#define FEEDBACK_ON		1

#define LEDSW_OFF 		0
#define LEDSW_ON		1





#define PIDDUTYMAX	100	//占空比保留两位小数，最大占空比为90.00%
#define PIDDUTYMIN	0		//占空比最小值为0

#define MIN_LED_CURRENT	0.02	//定义最小的LED启动电流
#define MAX_LED_CURRENT	5		//定义最大的LED工作电流

#define MIN_BNC_THRESHOLD 0.02		//BNC信号有效性阈值


typedef struct
{
	float err;				//误差值
	float err_n1;			//n-1误差值
	float err_n2;			//n-2误差值
	float Kp;
	float Ki;
	float Kd;
	
	float Incerr;			//增量误差
	
	float UK;				//PID 误差输出
	
	float incrementCurrent;
	float AC;	
}sPID_t;

extern sPID_t PID1;
extern sPID_t PID2;


extern struct rt_semaphore cal_adcpid_sem;


extern uint8_t IS_LED1_Exist;
extern uint8_t IS_LED2_Exist;



extern float cf_a;
extern float cf_b;
extern float cf_c;



void LED1_Init(float Duty);
void Set_LED1_Duty(float Duty);
void LED1_ENable(void);
void LED1_Disable(void);

void LED2_Init(float Duty);
void Set_LED2_Duty(float Duty);
void LED2_ENable(void);
void LED2_Disable(void);

float PID1_Cal(float target_Current,float Actual_current);
float PID2_Cal(float target_Current,float Actual_current);

void mled_thread_entry(void *par);
void pled_monitor_thread_entry(void *par);

#endif


























