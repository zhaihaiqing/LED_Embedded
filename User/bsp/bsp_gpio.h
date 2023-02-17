
#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#define LED1   0x01
#define LED2   0x02
#define LED3   0x04
#define LED4   0x08


#define LED_RUN_ON()		GPIOC->BSRRH = GPIO_Pin_4//GPIO_ResetBits(GPIOC,GPIO_Pin_4)
#define LED_RUN_OFF()		GPIOC->BSRRL = GPIO_Pin_4//GPIO_SetBits(GPIOC,GPIO_Pin_4)
#define LED_RUN_TOGGLE() 	GPIOC->ODR ^= GPIO_Pin_4

#define LED_OVLD_ON()		GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define LED_OVLD_OFF()		GPIO_SetBits(GPIOA,GPIO_Pin_7)


#define WDI_FD()				GPIO_ToggleBits(GPIOC,GPIO_Pin_0)

#define READ_EXCOM_SW()			GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)	
#define READ_LED_SW()			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)	
#define READ_FEEDBACK_SW()		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)	


#define FAN_ON()			GPIO_SetBits(GPIOE,GPIO_Pin_5)
#define FAN_OFF()			GPIO_ResetBits(GPIOE,GPIO_Pin_5)

void GPIO_Configuration(void);


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
