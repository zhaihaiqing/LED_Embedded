#ifndef __APP_TEMP_H
#define __APP_TEMP_H
#include "main.h"


#define BOARD_TEMP_UP_LIMIT		45			//电路板温度上限，超过该值，则开启风扇
#define PLED_TEMP_UP_LIMIT		65			//灯头温度上限，超过该值，则


void pled_temp_thread_entry(void *par);
	
void board_temp_thread_entry(void *par);






#endif


























