#include "main.h"


float board_temp=0;
void board_temp_thread_entry(void *par)
{
	DS18B20_ReadROM();
	DS18B20_SendConV_Command();
	rt_thread_mdelay(780);
	get_DS18B20_TEMP();
	rt_thread_mdelay(100);
	
	while(1)
	{
		DS18B20_SendConV_Command();	//发送转换命令
		rt_thread_mdelay(780);		//等待至少750ms后读取数据
		board_temp=get_DS18B20_TEMP();			//读取数据
		
		if(board_temp>45.0)			//主板温度大于45度，开启散热
		{
			FAN_ON();
		}
		else						//主板温度大于45度，开启散热
		{
			FAN_OFF();
		}
		rt_thread_mdelay(2000);
	}	
}
























