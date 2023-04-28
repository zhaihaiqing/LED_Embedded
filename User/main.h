/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.h 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//#include "arm_math.h"

#include "rtthread.h"
#include "scs_thread.h"

#include "sys.h"
#include "debug_uart.h"
#include "bsp_gpio.h"

#include "bsp_temp.h"
#include "bsp_adc.h"
#include "bsp_iadc.h"
#include "bsp_i2c.h"
#include "bsp_i2c_port.h"

#include "bsp_i2cc_port.h"

#include "bsp_oled.h"
#include "bsp_codec.h"
#include "bsp_timer.h"
#include "bsp_eeprom.h"
#include "bsp_ds18b20.h"

#include "app_key.h"
#include "app_codec.h"
#include "app_temp.h"
#include "app_adc.h"
#include "app_led.h"

#include "app_test.h"


//#define __FPU_PRESENT 	1U
//#define __FPU_USED 		1U


//需要存储的参数
typedef struct  __attribute__ ((__packed__))		
{												
	int16_t  		Front_Panel_current;			//前面板设置的电流值
	
}sMEMData_t;



//ADC转换后的数据
typedef struct  __attribute__ ((__packed__))		//转换后的数据
{												
	float  		Exin_Analog_signal;					//外部输出的模拟信号
	float  		LED2_Current;					//波形类型
	float		LED2_Temp;						//外部触发模式的宽度，仅在波形2、3起作用
	float		LED1_Light_Intensity;			//外部触发模式的宽度，仅在波形2、3起作用
	float		LED1_Current;
	float		LED1_Temp;
}sADCCONVData_t;



extern sADCCONVData_t	sADCCONVData;







/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);



#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
