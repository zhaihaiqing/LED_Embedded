#ifndef __APP_TEMP_H
#define __APP_TEMP_H
#include "main.h"


#define BOARD_TEMP_UP_LIMIT		45			//��·���¶����ޣ�������ֵ����������
#define PLED_TEMP_UP_LIMIT		65			//��ͷ�¶����ޣ�������ֵ����


void pled_temp_thread_entry(void *par);
	
void board_temp_thread_entry(void *par);






#endif


























