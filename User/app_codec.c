#include "main.h"

int16_t FrontPanel_Set_current=0;	//前面板设置的电流值

struct rt_semaphore codec_sem;  //创建一个信号量，当收到一次中断时，发送信号量
uint8_t Rotate_Increase_Cnt = 0, Rotate_Decrease_Cnt = 0;
void codec_thread_entry(void *par)
{	
	uint8_t pa=0,pb=0;
	
	rt_sem_init(&codec_sem, "codec_sem", 0, RT_IPC_FLAG_FIFO); //创建信号量
	
	Codec_Init();
	
	while(1)
	{
		rt_sem_control(&codec_sem, RT_IPC_CMD_RESET, RT_NULL); 	//等待前清零信号量，防止误操作
		rt_sem_take(&codec_sem, RT_WAITING_FOREVER);			//持续等待信号量
		
		rt_thread_mdelay(2);		//
		
		pa=PULSE_A();
		pb=PULSE_B();
		
		Oled_bRefresh = OLED_BREFRESH_BEGIN;	//设置Oled_bRefresh为0，刷新屏幕
		
		//log_info("pa=%d,pb=%d\r\n",pa,pb);
		log_info("Rotate:");
		
		if(pa != pb)
		{
			Rotate_Increase_Cnt++;
			
			//FrontPanel_Set_current++;
			FrontPanel_Set_current=FrontPanel_Set_current+5;
			if(FrontPanel_Set_current>500)
			{
				FrontPanel_Set_current=0;
			}
			log_info("++\r\n");
			if(Rotate_Decrease_Cnt)
			{
				Rotate_Decrease_Cnt--;
			}
		}
		else
		{
			Rotate_Decrease_Cnt++;
			//FrontPanel_Set_current--;
			FrontPanel_Set_current = FrontPanel_Set_current-5;
			if(FrontPanel_Set_current<0)
			{
				FrontPanel_Set_current=500;
			}
			log_info("--\r\n");
			if(Rotate_Increase_Cnt)
			{
				Rotate_Increase_Cnt--;
			}
		}

		log_info("LED_current:%d\r\n",FrontPanel_Set_current);
	}
}



























