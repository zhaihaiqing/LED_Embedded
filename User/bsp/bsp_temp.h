
#ifndef __BSP_TEMP_H
#define __BSP_TEMP_H


							 
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
