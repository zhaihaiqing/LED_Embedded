#include "main.h"

int16_t FrontPanel_Set_current=0;	//ǰ������õĵ���ֵ

struct rt_semaphore codec_sem;  //����һ���ź���
//uint8_t Rotate_Increase_Cnt = 0, Rotate_Decrease_Cnt = 0;
void codec_thread_entry(void *par)
{	
	uint8_t pa=0,pb=0;
	
	rt_sem_init(&codec_sem, "codec_sem", 0, RT_IPC_FLAG_FIFO); //�����ź���
	
	Codec_Init();
	
	while(1)
	{
		rt_sem_control(&codec_sem, RT_IPC_CMD_RESET, RT_NULL); 	//�ȴ�ǰ�����ź�������ֹ�����
		rt_sem_take(&codec_sem, RT_WAITING_FOREVER);			//�����ȴ��ź���
		
		rt_thread_mdelay(1);		//
		
		pa=PULSE_A();
		pb=PULSE_B();
		
		//log_info("pa=%d,pb=%d\r\n",pa,pb);
		log_info("Rotate:");
		
		if(pa != pb)
		{
			//Rotate_Increase_Cnt++;
			
			//FrontPanel_Set_current++;
			FrontPanel_Set_current=FrontPanel_Set_current+5;
			if(FrontPanel_Set_current>500)
			{
				FrontPanel_Set_current=500;
			}
			rt_sem_release(&oled_refresh_sem); //�ͷŲ����ź���
			log_info("++\r\n");
//			if(Rotate_Decrease_Cnt)
//			{
//				Rotate_Decrease_Cnt--;
//			}
		}
		else
		{
			//Rotate_Decrease_Cnt++;
			//FrontPanel_Set_current--;
			FrontPanel_Set_current = FrontPanel_Set_current-5;
			if(FrontPanel_Set_current<0)
			{
				FrontPanel_Set_current=0;
			}
			rt_sem_release(&oled_refresh_sem); //�ͷŲ����ź���
			log_info("--\r\n");
//			if(Rotate_Increase_Cnt)
//			{
//				Rotate_Increase_Cnt--;
//			}
		}

		log_info("LED_current:%d\r\n",FrontPanel_Set_current);
	}
}



























