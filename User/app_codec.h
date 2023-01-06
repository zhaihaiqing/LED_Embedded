#ifndef __APP_CODEC_H
#define __APP_CODEC_H
#include "main.h"


extern int16_t FrontPanel_Set_current;
extern struct rt_semaphore codec_sem;

void codec_thread_entry(void *par);


#endif


























