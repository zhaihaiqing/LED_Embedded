
#ifndef __BSP_CODEC_H
#define __BSP_CODEC_H




#define PULSE_A()		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)	
#define PULSE_B()		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10)	



void Codec_Init(void);


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
