#include "main.h"


//float board_temp=0;
//void board_temp_thread_entry(void *par)
//{
//	DS18B20_ReadROM();
//	DS18B20_SendConV_Command();
//	rt_thread_mdelay(780);
//	get_DS18B20_TEMP();
//	rt_thread_mdelay(100);
//	
//	while(1)
//	{		
//		DS18B20_SendConV_Command();	//发送转换命令
//		rt_thread_mdelay(780);		//等待至少750ms后读取数据
//		board_temp=get_DS18B20_TEMP();			//读取数据
//		
//		if(board_temp>45.0)			//主板温度大于45度，开启散热
//		{
//			FAN_ON();
//		}
//		else						//主板温度大于45度，开启散热
//		{
//			FAN_OFF();
//		}
//		rt_thread_mdelay(2000);
//	}	
//}




//void pled_temp_thread_entry(void *par)
//{
//	float pled_temp1=0,pled_temp2=0;
//	
//	float pled_light=0;
//	
//	float pled1_current=0,pled2_current=0;
//	
//	float pled1_current_buf[STA_Count]={0},pled2_current_buf[STA_Count]={0};
//	
//	uint32_t ix=0;
//	
//	rt_thread_mdelay(1000);
//	
//	while(1)
//	{	
//		pled_temp1=0;
//		pled_temp2=0;
//		pled_light=0;
//		pled1_current=0;
//		pled2_current=0;
//		
//		for(ix=0;ix<STA_Count;ix++)
//		{
//			pled_temp1 += sADCCONVData.LED1_Temp;
//			pled_temp2 += sADCCONVData.LED2_Temp;
//			
//			pled_light += sADCCONVData.LED1_Light_Intensity;
//			
//			pled1_current_buf[ix] = sADCCONVData.LED1_Current;
//			pled2_current_buf[ix] = sADCCONVData.LED2_Current;
//			
//			rt_thread_mdelay(10);
//		}
//		
//		pled_temp1 = (pled_temp1/1000)/STA_Count;
//		pled_temp2 = (pled_temp2/1000)/STA_Count;
//		pled_light = pled_light/STA_Count;
//		
//		Bubble_Sort(pled1_current_buf,STA_Count,0);
//		Bubble_Sort(pled2_current_buf,STA_Count,0);
//		
////		for(ix=0;ix<STA_Count;ix++)
////		{
////			log_info("pled1_current_buf[%d]:%f\r\n",ix,pled1_current_buf[ix]);
////		}
//		
//		for(ix=0;ix<5;ix++)
//		{
//			pled1_current += pled1_current_buf[ix];
//			pled2_current += pled2_current_buf[ix];
//		}
//		
//		pled1_current = pled1_current/5;
//		pled2_current = pled2_current/5;
//		

//		pled_temp1 = cf_a * powf( pled_temp1, cf_b ) + cf_c;		//计算温度
//		pled_temp2 = cf_a * powf( pled_temp2, cf_b ) + cf_c;		//计算温度
//		
//		//log_info("pled1:%f,pled2:%f,pled_light:%f,pled1_current:%f,pled2_current:%f\r\n",pled_temp1,pled_temp2,pled_light,pled1_current,pled2_current);
//		
//		//LED过流检测
//		if(extcom_sw_flag == EXTCOM_OFF)				//使用前面板控制参数
//		{
//			if((led_sw_flag == LEDSW_ON ) &&  ((pled1_current >= 0.01) || (pled2_current >= 0.01)))
//			{
//				if((pled1_current > (FrontPanel_Set_current*1.2/100) && IS_LED1_Exist) || (pled2_current > (FrontPanel_Set_current*1.2/100) && IS_LED2_Exist))
//				{
//					LED_OVLD_ON();
//					log_info("LED1_I_Current:%f,LED2_I_Current:%f\r\n",pled1_current,pled2_current);
//				}
//				else
//				{
//					LED_OVLD_OFF();
//				}
//			}
//			else
//			{
//				LED_OVLD_OFF();
//			}
//		}
//		else											//使用模拟信号控制
//		{
//			if( (ex_led_sw_flag == LEDSW_ON) && ((pled1_current >= 0.01) || (pled2_current >= 0.01)) )
//			{
//				if((pled1_current > (sADCCONVData.Exin_Analog_signal*1.2) && IS_LED1_Exist) || (pled2_current > (sADCCONVData.Exin_Analog_signal*1.2) && IS_LED2_Exist))
//				{
//					LED_OVLD_ON();
//					log_info("LED1_I_Current:%f,LED2_I_Current:%f\r\n",pled1_current,pled2_current);
//				}
//				else
//				{
//					LED_OVLD_OFF();
//				}
//			}
//			else
//			{
//				LED_OVLD_OFF();
//			}
//		}
//		rt_thread_mdelay(10);
//	}	
//}







void board_temp_thread_entry(void *par)
{
	float board_tempA=0,board_tempB=0,board_temp=0;
	uint32_t ix=0;
	rt_thread_mdelay(400);
	
	board_tempA = GetTempValue(TEMP_A_ADDR);
	board_tempB = GetTempValue(TEMP_B_ADDR);
	
	rt_thread_mdelay(1000);
	
	board_tempA = GetTempValue(TEMP_A_ADDR);
	board_tempB = GetTempValue(TEMP_B_ADDR);
	
	rt_thread_mdelay(1000);
	
	while(1)
	{	
		board_tempA = GetTempValue(TEMP_A_ADDR);
		rt_thread_mdelay(100);
		board_tempB = GetTempValue(TEMP_B_ADDR);
		rt_thread_mdelay(100);
		
		ix++;
		if(ix >= 10)
		{
			ix=0;
			log_info("board_tempA:%f   board_tempB:%f\r\n",board_tempA,board_tempB);
		}
		
		if(board_tempA >= board_tempB)
		{
			board_temp = board_tempA;
		}
		else
		{
			board_temp = board_tempB;
		}
		
		
		
		if( board_temp>50.0)			//主板温度大于45度，开启散热
		{
			FAN_ON();
			rt_thread_mdelay(5000);
			rt_thread_mdelay(5000);
			rt_thread_mdelay(5000);
			rt_thread_mdelay(5000);
		}
		else						//主板温度大于45度，开启散热
		{
			FAN_OFF();
		}
		rt_thread_mdelay(800);
	}	
}
























