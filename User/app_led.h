#ifndef __APP_LED_H
#define __APP_LED_H
#include "main.h"

#define LED1_UPLIMIT_TEMP	60	//����LED���¶����ޣ��������޺�Ĵ������
#define LED2_UPLIMIT_TEMP	60


#define EXTCOM_OFF 		0
#define EXTCOM_ON		1

#define FEEDBACK_OFF 	0
#define FEEDBACK_ON		1

#define LEDSW_OFF 		0
#define LEDSW_ON		1





#define PIDDUTYMAX	95	//ռ�ձȱ�����λС�������ռ�ձ�Ϊ90.00%
#define PIDDUTYMIN	0		//ռ�ձ���СֵΪ0

typedef struct
{
	float err;				//���ֵ
	float err_n1;			//n-1���ֵ
	float err_n2;			//n-2���ֵ
	float Kp;
	float Ki;
	float Kd;
	
	float Incerr;			//�������
	
	float UK;				//PID ������
	
	float incrementCurrent;
	float AC;	
}sPID_t;

extern sPID_t PID;





void LED1_Init(float Duty);
void Set_LED1_Duty(float Duty);
void LED1_ENable(void);
void LED1_Disable(void);

void LED2_Init(float Duty);
void Set_LED2_Duty(float Duty);
void LED2_ENable(void);
void LED2_Disable(void);


void mled_thread_entry(void *par);


#endif


























