#ifndef __APP_ADC_H
#define __APP_ADC_H
#include "main.h"


void adc_thread_entry(void *par);
void get_adc(void);
void cal_results(void);

void get_1times_adc(void);
void get_5times_adc(void);
void get_10times_adc(void);

#endif


























