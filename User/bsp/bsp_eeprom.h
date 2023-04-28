
#ifndef __BSP_EEPROM_H
#define __BSP_EEPROM_H
#include "main.h"

//容量列表
#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767
#define AT24C512	65535
#define AT24CM01	131071

//页大小
#define AT24C01_PAGESIZE		8
#define AT24C02_PAGESIZE		8
#define AT24C04_PAGESIZE		16
#define AT24C08_PAGESIZE		16
#define AT24C16_PAGESIZE		16
#define AT24C32_PAGESIZE		32
#define AT24C64_PAGESIZE	    32
#define AT24C128_PAGESIZE		64
#define AT24C256_PAGESIZE		64
#define AT24C512_PAGESIZE		128
#define AT24CM01_PAGESIZE		256



void EE_Init(void);
uint8_t EE_ReadS(uint16_t addr,uint8_t *pBuffer,uint16_t Length);
void EE_WriteOneByte(uint16_t addr,uint8_t data);
void EE_WritePage(uint16_t addr,uint8_t *pBuffer,uint16_t Length);
void EE_WriteS(uint16_t addr,uint8_t *pBuffer,uint16_t Length);






#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
