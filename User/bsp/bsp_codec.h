
#ifndef __BSP_CODEC_H
#define __BSP_CODEC_H


#define PULA_GPIO_Pin	GPIO_Pin_2		//板级指示灯，LED1
#define PULA_GPIO_Port	GPIOE

#define PULB_GPIO_Pin	GPIO_Pin_3		//板级指示灯，LED1
#define PULB_GPIO_Port	GPIOE

#define STA_GPIO_Pin	GPIO_Pin_4		//板级指示灯，LED1
#define STA_GPIO_Port	GPIOE



#define PULSE_A()		GPIO_ReadInputDataBit(PULA_GPIO_Port,PULA_GPIO_Pin)	
#define PULSE_B()		GPIO_ReadInputDataBit(PULB_GPIO_Port,PULB_GPIO_Pin)	



void Codec_Init(void);


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
