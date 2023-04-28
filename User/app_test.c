#include "main.h"



//void test_thread_entry(void *par)
//{
//	uint16_t count = 0;
//	float DQ1_temp = 0;
//	float DQ2_temp = 0;
//	float board_tempA = 0;
//	float board_tempB = 0;
//	float pled1_current = 0;
//	float pled_temp1 = 0;
//	
//	
//	
//	rt_thread_mdelay(3000);
//	DQ_Temp_Init();
//	rt_thread_mdelay(200);
//	DQ1_ReadROM();
//	rt_thread_mdelay(200);
//	DQ2_ReadROM();
//	
//	rt_thread_mdelay(1000);
//	DQ1_ReadROM();
//	rt_thread_mdelay(200);
//	DQ2_ReadROM();
//	rt_thread_mdelay(200);
//	while(1)
//	{
//		DS18B20_A_SendConV_Command();	//发送转换命令
//		rt_thread_mdelay(10);
//		DS18B20_B_SendConV_Command();	//发送转换命令
//		rt_thread_mdelay(10);
//		
//		board_tempA = GetTempValue(TEMP_A_ADDR);
//		rt_thread_mdelay(100);
//		board_tempB = GetTempValue(TEMP_B_ADDR);
//		rt_thread_mdelay(100);
//		
//		rt_thread_mdelay(568);		//等待至少750ms后读取数据
//		DQ1_temp=get_DS18B20_A_TEMP();			//读取数据
//		rt_thread_mdelay(10);
//		DQ2_temp=get_DS18B20_B_TEMP();			//读取数据
//		
//		pled_temp1 = sADCCONVData.LED1_Temp;
//		pled_temp1 = (pled_temp1/1000);
//		
//		pled1_current = sADCCONVData.LED1_Current;
//		
//		pled_temp1 = cf_a * powf( pled_temp1, cf_b ) + cf_c;
//		
//		log_info("pled1_current:%f pled_temp1:%.2f DQ1_temp:%.2f DQ2_temp:%.2f board_tempA:%.2f board_tempB:%.2f\r\n",pled1_current,pled_temp1,DQ1_temp,DQ2_temp,board_tempA,board_tempB);
//		
//		rt_thread_mdelay(4200);
//	}
//	
//}





