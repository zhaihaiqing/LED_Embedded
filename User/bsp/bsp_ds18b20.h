
#ifndef __BSP_DS18B20_H
#define __BSP_DS18B20_H


#define DQ1_GPIO_Pin	GPIO_Pin_11		//板级指示灯，LED1
#define DQ1_GPIO_Port	GPIOA

#define DQ2_GPIO_Pin	GPIO_Pin_12		//板级指示灯，LED2
#define DQ2_GPIO_Port	GPIOA


//DS18B20数据线   PA11
#define DQ1_OUT_H					DQ1_GPIO_Port->BSRRL = DQ1_GPIO_Pin
#define DQ1_OUT_L					DQ1_GPIO_Port->BSRRH = DQ1_GPIO_Pin
#define DQ1_Read()					((DQ1_GPIO_Port->IDR & DQ1_GPIO_Pin) != 0)

//DS18B20数据线   PA12
#define DQ2_OUT_H					DQ2_GPIO_Port->BSRRL = DQ2_GPIO_Pin
#define DQ2_OUT_L					DQ2_GPIO_Port->BSRRH = DQ2_GPIO_Pin
#define DQ2_Read()					((DQ2_GPIO_Port->IDR & DQ2_GPIO_Pin) != 0)



////设置DS18B20数据线方向
#define SET_DQ1_IN     DQ1_GPIO_Port->MODER   &= (~(0x00000003<<(2*11)))
//															//设置输入功能
//															//设置上拉
//															
#define SET_DQ1_OUT    DQ1_GPIO_Port->MODER   &= (~(0x00000003<<(2*11)));DQ1_GPIO_Port->MODER   |= (0x00000001<<(2*11))


////设置DS18B20数据线方向
#define SET_DQ2_IN     DQ1_GPIO_Port->MODER   &= (~(0x00000003<<(2*12)))
//															//设置输入功能
//															//设置上拉
//															
#define SET_DQ2_OUT    DQ1_GPIO_Port->MODER   &= (~(0x00000003<<(2*12)));DQ1_GPIO_Port->MODER   |= (0x00000001<<(2*12))
							  //设置为输出功能
							  //设置输出类型为开漏
							  //设置输出速度为100MHz
							  //设置上拉
//							  

void DQ_Temp_Init(void);
void DQ_delayus(unsigned int us);

void DQ1_ReadROM(void);
float DS18B20_A_SendConV_Command(void);
float get_DS18B20_A_TEMP(void);

void DQ2_ReadROM(void);
float DS18B20_B_SendConV_Command(void);
float get_DS18B20_B_TEMP(void);


#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
