
#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#define LED1   0x01
#define LED2   0x02
#define LED3   0x04
#define LED4   0x08


#define LED1_GPIO_Pin	GPIO_Pin_11		//板级指示灯，LED1
#define LED1_GPIO_Port	GPIOA

#define LED2_GPIO_Pin	GPIO_Pin_12		//板级指示灯，LED2
#define LED2_GPIO_Port	GPIOA

#define WDI_GPIO_Pin	GPIO_Pin_0		//喂狗信号，WDI
#define WDI_GPIO_Port	GPIOC

#define LED_RUN_GPIO_Pin	GPIO_Pin_8	//运行指示灯，LED_RUN
#define LED_RUN_GPIO_Port	GPIOB

#define LED_OVLD_GPIO_Pin	GPIO_Pin_9	//过载指示灯，LED_OVLD
#define LED_OVLD_GPIO_Port	GPIOB


#define EXCOM_SW_GPIO_Pin	GPIO_Pin_0	//外部信号使能开关，EXCOM
#define EXCOM_SW_GPIO_Port	GPIOB

#define LED_SW_GPIO_Pin		GPIO_Pin_9	//LED开关信号，LED_SW
#define LED_SW_GPIO_Port	GPIOC

#define FB_SW_GPIO_Pin		GPIO_Pin_7	//反馈开关信号，FB_SW
#define FB_SW_GPIO_Port		GPIOA

#define DIGIN_GPIO_Pin		GPIO_Pin_1	//数字开关信号，DIG_SW
#define DIGIN_GPIO_Port		GPIOB

#define BEEP_GPIO_Pin		GPIO_Pin_8	//蜂鸣器开关信号，BEEP
#define BEEP_GPIO_Port		GPIOC

#define FAN_GPIO_Pin		GPIO_Pin_7	//风扇开关信号，FAN_SW
#define FAN_GPIO_Port		GPIOC




#define LED1_ON()				GPIO_ResetBits(LED1_GPIO_Port,LED1_GPIO_Pin)
#define LED1_OFF()				GPIO_SetBits(LED1_GPIO_Port,LED1_GPIO_Pin)

#define LED2_ON()				GPIO_ResetBits(LED2_GPIO_Port,LED2_GPIO_Pin)
#define LED2_OFF()				GPIO_SetBits(LED2_GPIO_Port,LED2_GPIO_Pin)


#define WDI_FD()				GPIO_ToggleBits(WDI_GPIO_Port,WDI_GPIO_Pin)

#define LED_RUN_ON()			GPIO_ResetBits(LED_RUN_GPIO_Port,LED_RUN_GPIO_Pin)	//GPIOB->BSRRH = GPIO_Pin_8
#define LED_RUN_OFF()			GPIO_SetBits(LED_RUN_GPIO_Port,LED_RUN_GPIO_Pin)	//GPIOB->BSRRL = GPIO_Pin_8
//#define LED_RUN_TOGGLE() 		GPIOB->ODR ^= GPIO_Pin_8

#define LED_OVLD_OFF()			GPIO_ResetBits(LED_OVLD_GPIO_Port,LED_OVLD_GPIO_Pin)//;GPIO_ResetBits(BEEP_GPIO_Port,BEEP_GPIO_Pin)
#define LED_OVLD_ON()			GPIO_SetBits(LED_OVLD_GPIO_Port,LED_OVLD_GPIO_Pin)//;GPIO_SetBits(BEEP_GPIO_Port,BEEP_GPIO_Pin)


#define READ_EXCOM_SW()			GPIO_ReadInputDataBit(EXCOM_SW_GPIO_Port,EXCOM_SW_GPIO_Pin)	
#define READ_LED_SW()			GPIO_ReadInputDataBit(LED_SW_GPIO_Port,LED_SW_GPIO_Pin)	
#define READ_FEEDBACK_SW()		GPIO_ReadInputDataBit(FB_SW_GPIO_Port,FB_SW_GPIO_Pin)	
#define READ_DIGIN_SW()			GPIO_ReadInputDataBit(DIGIN_GPIO_Port,DIGIN_GPIO_Pin)

#define BEEP_ON()				GPIO_SetBits(BEEP_GPIO_Port,BEEP_GPIO_Pin)
#define BEEP_OFF()				GPIO_ResetBits(BEEP_GPIO_Port,BEEP_GPIO_Pin)

#define FAN_ON()				GPIO_SetBits(FAN_GPIO_Port,FAN_GPIO_Pin)
#define FAN_OFF()				GPIO_ResetBits(FAN_GPIO_Port,FAN_GPIO_Pin)


#define IS_DIGIN_Rising()		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)



void GPIO_Configuration(void);
void DIGIN_Iint(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
