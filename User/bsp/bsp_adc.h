
#ifndef __BSP_ADC_H
#define __BSP_ADC_H
#include "main.h"

#define ADC_RANGE		10.0
#define ADC_CONV_A		1
#define ADC_CONV_B		2
#define ADC_CONV_C		3
#define ADC_CONV_ALL	4


/*
ADC ch1---->LED2_Current
ADC ch2---->LED1_Current
ADC ch3---->LED2_Temp
ADC ch4---->LED1_Temp
ADC ch5---->Light_intensity
ADC ch6---->Analog_Input
*/



//#define 	LED1_Current_Gain		(19.8/2.7+1.0)		
//#define 	LED1_Temp_Gain			(19.8/2.7+1.0)
//#define 	LED2_Current_Gain		(19.8/2.7+1.0)
//#define 	LED2_Temp_Gain			(19.8/2.7+1.0)
//#define 	Light_intensity_Gain	((10000.0/(100.0+10000.0))*(1.0+2.7/2.7))
//#define 	Analog_Input_Gain		(1.0)

#define 	LED1_Current_Gain		(8.333333)		
#define 	LED1_Temp_Gain			(8.333333)
#define 	LED2_Current_Gain		(8.333333)
#define 	LED2_Temp_Gain			(8.333333)
#define 	Light_intensity_Gain	(1.980198) 
#define 	Analog_Input_Gain		(1.0)


#define LED1_CURRENT_RES	0.045	//两个90mΩ采样电阻并联
#define LED2_CURRENT_RES	0.045	//两个90mΩ采样电阻并联






#define ADC_RD_H()		GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define ADC_RD_L()		GPIO_ResetBits(GPIOB,GPIO_Pin_12)

#define ADC_CONV_H()	GPIO_SetBits(GPIOE,GPIO_Pin_13)
#define ADC_CONV_L()	GPIO_ResetBits(GPIOE,GPIO_Pin_13)


#define IS_ADC_BUSY()			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)

#define SET_ADC_RANGE_HIGH()	GPIO_ResetBits(GPIOE,GPIO_Pin_12);	//拉低RANG，设置为4 X ±VREF
#define SET_ADC_RANGE_LOW()		GPIO_SetBits(GPIOE,GPIO_Pin_12);	//拉高RANG，设置为2 X ±VREF

void ADC_GPIO_Configuration(void);
void Start_ADC_ConV(void);
void ADC_Config(void);



#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
