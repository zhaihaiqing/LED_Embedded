#include "main.h"


float board_temp=0;
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
//		DS18B20_SendConV_Command();	//����ת������
//		rt_thread_mdelay(780);		//�ȴ�����750ms���ȡ����
//		board_temp=get_DS18B20_TEMP();			//��ȡ����
//		
//		if(board_temp>45.0)			//�����¶ȴ���45�ȣ�����ɢ��
//		{
//			FAN_ON();
//		}
//		else						//�����¶ȴ���45�ȣ�����ɢ��
//		{
//			FAN_OFF();
//		}
//		rt_thread_mdelay(2000);
//	}	
//}

void board_temp_thread_entry(void *par)
{
	float board_tempA=0,board_tempB=0;
	rt_thread_mdelay(100);
	
	while(1)
	{		
		board_tempA = GetTempValue(TEMP_A_ADDR);
		board_tempB = GetTempValue(TEMP_B_ADDR);
		
		log_info("board_tempA:%f   board_tempB:%f\r\n",board_tempA,board_tempB);
		
		board_temp = board_tempA;
		
		if(board_temp>45.0)			//�����¶ȴ���45�ȣ�����ɢ��
		{
			FAN_ON();
		}
		else						//�����¶ȴ���45�ȣ�����ɢ��
		{
			FAN_OFF();
		}
		rt_thread_mdelay(750);
	}	
}
























