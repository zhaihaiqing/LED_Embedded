
#ifndef __BSP_TEMP_H
#define __BSP_TEMP_H

//DS18B20数据线   PE6
#define DQ_OUT_H					GPIOE->BSRRL = GPIO_Pin_6
#define DQ_OUT_L					GPIOE->BSRRH = GPIO_Pin_6
#define DQ_Read()					(GPIOE->IDR & GPIO_Pin_6)


//设置DS18B20数据线方向
#define SET_DS18B20_DQ_IN     GPIOE->MODER   &= 0xffffcfff;\
							  GPIOE->PUPDR   &= 0xffffcfff;GPIOE->PUPDR   |= 0x00001000
															//设置输入功能
															//设置上拉
															
#define SET_DS18B20_DQ_OUT    GPIOE->MODER   &= 0xffffcfff;GPIOE->MODER   |= 0x00001000;\
							  GPIOE->OTYPER  &= 0xffffffbf;GPIOE->OTYPER  |= 0x00000040;\
							  GPIOE->OSPEEDR &= 0xffffcfff;GPIOE->OSPEEDR |= 0x00003000;\
							  GPIOE->PUPDR   &= 0xffffcfff;GPIOE->PUPDR   |= 0x00001000
							  //设置为输出功能
							  //设置输出类型为开漏
							  //设置输出速度为100MHz
							  //设置上拉
							  


#define TEMP_A_ADDR		0x90
#define TEMP_B_ADDR		0x92

#define LM75_REG_TEMP        0x00






void DS18B20_delayus(unsigned int us);
void DS18B20_ReadROM(void);
void DQ_Temp_Init(void);
float DS18B20_SendConV_Command(void);
float get_DS18B20_TEMP(void);


float GetTempValue(uint8_t addr);

#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
