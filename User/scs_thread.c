#include "main.h"

//LED��ͷ��������
static struct rt_thread mled_thread;				//�߳̿��ƿ�
static char mled_thread_stack[1024];				//�̶߳�ջ
#define MLED_THREAD_PRIORITY         10				//�߳����ȼ���
#define MLED_THREAD_TIMESLICE        100			//�̵߳�ʱ��Ƭ��СS

//�������������
static struct rt_thread codec_thread;				//�߳̿��ƿ�
static char codec_thread_stack[1024];				//�̶߳�ջ
#define CODEC_THREAD_PRIORITY         9				//�߳����ȼ���
#define CODEC_THREAD_TIMESLICE        100			//�̵߳�ʱ��Ƭ��С

//�����źż������
static struct rt_thread sw_thread;					//�߳̿��ƿ�
static char sw_thread_stack[1024];					//�̶߳�ջ
#define SW_THREAD_PRIORITY         12				//�߳����ȼ���
#define SW_THREAD_TIMESLICE        100				//�̵߳�ʱ��Ƭ��С

//OLED��ʾ
static struct rt_thread oled_thread;				//�߳̿��ƿ�
static char oled_thread_stack[1024];				//�̶߳�ջ
#define OLED_THREAD_PRIORITY         13				//�߳����ȼ���
#define OLED_THREAD_TIMESLICE        100			//�̵߳�ʱ��Ƭ��С

//Ƭ��ADC���ݻ�ȡ
static struct rt_thread adc_thread;					//�߳̿��ƿ�
static char adc_thread_stack[2048];					//�̶߳�ջ
#define ADC_THREAD_PRIORITY         14				//�߳����ȼ���
#define ADC_THREAD_TIMESLICE        100				//�̵߳�ʱ��Ƭ��С

//�����¶Ȼ�ȡ�̣߳�ds18b20��
static struct rt_thread board_temp_thread;			//�߳̿��ƿ�
static char board_temp_thread_stack[1024];			//�̶߳�ջ
#define BOARD_TEMP_THREAD_PRIORITY         15		//�߳����ȼ���
#define BOARD_TEMP_THREAD_TIMESLICE        100		//�̵߳�ʱ��Ƭ��С

//ָʾ���߳�
static struct rt_thread led_thread;					//�߳̿��ƿ�
static char led_thread_stack[1024];					//�̶߳�ջ
#define LED_THREAD_PRIORITY         16				//�߳����ȼ���
#define LED_THREAD_TIMESLICE        100				//�̵߳�ʱ��Ƭ��С


int scs_thread_init(void)					//������������߳�
{
	rt_thread_init(&mled_thread,					//�߳̿��ƿ�
                   "mled_thread",				//�߳̿��ƿ�����
                   mled_thread_entry,			//�߳���ں���
                   RT_NULL,						//�߳���ں����Ĳ���
                   &mled_thread_stack[0],		//�߳�ջ��ʼ��ַ
                   sizeof(mled_thread_stack),	//�߳�ջ��С
                   MLED_THREAD_PRIORITY, 		//�߳����ȼ�
				   MLED_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С
				   
	rt_thread_init(&codec_thread,					//�߳̿��ƿ�
                   "codec_thread",				//�߳̿��ƿ�����
                   codec_thread_entry,			//�߳���ں���
                   RT_NULL,						//�߳���ں����Ĳ���
                   &codec_thread_stack[0],		//�߳�ջ��ʼ��ַ
                   sizeof(codec_thread_stack),	//�߳�ջ��С
                   CODEC_THREAD_PRIORITY, 		//�߳����ȼ�
				   CODEC_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С
				   
	rt_thread_init(&sw_thread,					//�߳̿��ƿ�
                   "sw_thread",				//�߳̿��ƿ�����
                   sw_thread_entry,			//�߳���ں���
                   RT_NULL,						//�߳���ں����Ĳ���
                   &sw_thread_stack[0],		//�߳�ջ��ʼ��ַ
                   sizeof(sw_thread_stack),	//�߳�ջ��С
                   SW_THREAD_PRIORITY, 		//�߳����ȼ�
				   SW_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С
				   
	rt_thread_init(&oled_thread,					//�߳̿��ƿ�
                   "oled_thread",				//�߳̿��ƿ�����
                   oled_thread_entry,			//�߳���ں���
                   RT_NULL,						//�߳���ں����Ĳ���
                   &oled_thread_stack[0],		//�߳�ջ��ʼ��ַ
                   sizeof(oled_thread_stack),	//�߳�ջ��С
                   OLED_THREAD_PRIORITY, 		//�߳����ȼ�
				   OLED_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С
				   
	rt_thread_init(&adc_thread,					//�߳̿��ƿ�
                   "adc_thread",				//�߳̿��ƿ�����
                   adc_thread_entry,			//�߳���ں���
                   RT_NULL,						//�߳���ں����Ĳ���
                   &adc_thread_stack[0],		//�߳�ջ��ʼ��ַ
                   sizeof(adc_thread_stack),	//�߳�ջ��С
                   ADC_THREAD_PRIORITY, 		//�߳����ȼ�
				   ADC_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С
				   
	rt_thread_init(&board_temp_thread,					//�߳̿��ƿ�
                   "board_temp_thread",				//�߳̿��ƿ�����
                   board_temp_thread_entry,			//�߳���ں���
                   RT_NULL,						//�߳���ں����Ĳ���
                   &board_temp_thread_stack[0],		//�߳�ջ��ʼ��ַ
                   sizeof(board_temp_thread_stack),	//�߳�ջ��С
                   BOARD_TEMP_THREAD_PRIORITY, 		//�߳����ȼ�
				   BOARD_TEMP_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С
				   
	rt_thread_init(&led_thread,					//�߳̿��ƿ�
                   "led_thread",				//�߳̿��ƿ�����
                   led_thread_entry,			//�߳���ں���
                   RT_NULL,						//�߳���ں����Ĳ���
                   &led_thread_stack[0],		//�߳�ջ��ʼ��ַ
                   sizeof(led_thread_stack),	//�߳�ջ��С
                   LED_THREAD_PRIORITY, 		//�߳����ȼ�
				   LED_THREAD_TIMESLICE);		//�߳�ʱ��Ƭ��С
		
	
	rt_thread_startup(&codec_thread);
	rt_thread_startup(&sw_thread);
	rt_thread_startup(&oled_thread);					   
	rt_thread_startup(&adc_thread);	
	rt_thread_startup(&board_temp_thread);		   
	rt_thread_startup(&led_thread); 
	rt_thread_startup(&mled_thread);
				   
	return 0;
}




struct rt_semaphore oled_refresh_sem;  //����һ���ź���,
void oled_thread_entry(void *par)
{
	uint16_t count = 0;
	
	OLED_Init();
	
	Oled_Poll();
	
	rt_sem_init(&oled_refresh_sem, "oled_refresh_sem", 0, RT_IPC_FLAG_FIFO); //�����ź���
	
	while(1)
	{
		
		//rt_thread_mdelay(75);		//ˢ���� 25hz
		//Oled_Poll();	
		
		
		
		rt_sem_control(&oled_refresh_sem, RT_IPC_CMD_RESET, RT_NULL); 	//�ȴ�ǰ�����ź�������ֹ�����
		rt_sem_take(&oled_refresh_sem, RT_WAITING_FOREVER);			//�����ȴ��ź���

		for(count=0;count<10;count++)		//OLEDִ��һ����ʱ�ϳ�����������ת���Ͽ�ʱ�����ܴ���©���£����ԣ��յ�һ���ź���ִ��6�θ���
		{
			Oled_Poll();					//ִ��һ��Լ��ʱ110ms
			rt_thread_mdelay(50);
		}
	}
	
}








void led_thread_entry(void *par)
{
	uint16_t count = 0;
	
	while(1)
	{
		rt_thread_mdelay(200);
		LED_RUN_ON();
		//LED_OVLD_ON();
		rt_thread_mdelay(200);
		LED_RUN_OFF();
		//LED_OVLD_OFF();
	}
	
}



























