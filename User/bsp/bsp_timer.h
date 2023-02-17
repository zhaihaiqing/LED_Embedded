#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H 			    


void Timer2_PWM_Init(float Duty);
void Timer2_PWM_OC3_Enable(void);
void Timer2_PWM_OC3_Disable(void);
void Timer2_PWM_OC3_SetDuty(float Duty);



void Timer5_PWM_Init(float Duty);
void Timer5_PWM_OC2_Enable(void);
void Timer5_PWM_OC2_Disable(void);
void Timer5_PWM_OC2_SetDuty(float Duty);


void Timer6_Init(unsigned short arr,unsigned short psc);






#endif





























