
#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#define ADT_CS		1
#define FLASH_CS	2
#define SRAM_CS		4

#define SPI2_CS_ALL_H  8

#define ADT_CS_H      GPIO_SetBits(GPIOE,GPIO_Pin_6)
#define ADT_CS_L	  GPIO_ResetBits(GPIOE,GPIO_Pin_6)

#define FLASH_CS_H    GPIO_SetBits(GPIOE,GPIO_Pin_15)
#define FLASH_CS_L	  GPIO_ResetBits(GPIOE,GPIO_Pin_15)

#define SRAM_CS_H     GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define SRAM_CS_L	  GPIO_ResetBits(GPIOC,GPIO_Pin_13)



void SPI2_Configuration(void);
unsigned char SPI2_ReadWriteByte(unsigned char byte);
unsigned char SPI2_CS_Select(unsigned char cs);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
