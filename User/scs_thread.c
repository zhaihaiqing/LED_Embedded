#include "main.h"

static struct rt_timer timer1;
static void timeout1(void* parameter);
int timer_static_sample(void);


//编码器检测任务
static struct rt_thread codec_thread;				//线程控制块
static char codec_thread_stack[2048];				//线程堆栈
#define CODEC_THREAD_PRIORITY         9				//线程优先级，
#define CODEC_THREAD_TIMESLICE        100			//线程的时间片大小


//LED灯头控制任务
static struct rt_thread mled_thread;				//线程控制块
static char mled_thread_stack[2048];				//线程堆栈
#define MLED_THREAD_PRIORITY         10				//线程优先级，
#define MLED_THREAD_TIMESLICE        100			//线程的时间片大小S


//PLED灯头监测（电流、温度、光强）
static struct rt_thread pled_monitor_thread;			//线程控制块
static char pled_monitor_thread_stack[2048];			//线程堆栈
#define PLED_MONITOR_THREAD_PRIORITY         11		//线程优先级，
#define PLED_MONITOR_THREAD_TIMESLICE        100		//线程的时间片大小


////测试线程
//static struct rt_thread test_temp_thread;			//线程控制块
//static char test_temp_thread_stack[2048];			//线程堆栈
//#define TEST_TEMP_THREAD_PRIORITY         11		//线程优先级，
//#define TEST_TEMP_THREAD_TIMESLICE        100		//线程的时间片大小


//开关信号检测任务
static struct rt_thread sw_thread;					//线程控制块
static char sw_thread_stack[2048];					//线程堆栈
#define SW_THREAD_PRIORITY         12				//线程优先级，
#define SW_THREAD_TIMESLICE        100				//线程的时间片大小



//OLED显示
static struct rt_thread oled_thread;				//线程控制块
static char oled_thread_stack[2048];				//线程堆栈
#define OLED_THREAD_PRIORITY         13				//线程优先级，
#define OLED_THREAD_TIMESLICE        100			//线程的时间片大小



//片外ADC数据获取
static struct rt_thread adc_thread;					//线程控制块
static char adc_thread_stack[2048];					//线程堆栈
#define ADC_THREAD_PRIORITY         14				//线程优先级，
#define ADC_THREAD_TIMESLICE        100				//线程的时间片大小



//主板温度获取线程（）
static struct rt_thread board_temp_thread;			//线程控制块
static char board_temp_thread_stack[2048];			//线程堆栈
#define BOARD_TEMP_THREAD_PRIORITY         16		//线程优先级，
#define BOARD_TEMP_THREAD_TIMESLICE        100		//线程的时间片大小



//指示灯线程
static struct rt_thread led_thread;					//线程控制块
static char led_thread_stack[2048];					//线程堆栈
#define LED_THREAD_PRIORITY         17				//线程优先级，
#define LED_THREAD_TIMESLICE        100				//线程的时间片大小


int scs_thread_init(void)					//创建开机检测线程
{
	rt_thread_init(&mled_thread,					//线程控制块
                   "mled_thread",				//线程控制块名字
                   mled_thread_entry,			//线程入口函数
                   RT_NULL,						//线程入口函数的参数
                   &mled_thread_stack[0],		//线程栈起始地址
                   sizeof(mled_thread_stack),	//线程栈大小
                   MLED_THREAD_PRIORITY, 		//线程优先级
				   MLED_THREAD_TIMESLICE);		//线程时间片大小
				   
	rt_thread_init(&codec_thread,					//线程控制块
                   "codec_thread",				//线程控制块名字
                   codec_thread_entry,			//线程入口函数
                   RT_NULL,						//线程入口函数的参数
                   &codec_thread_stack[0],		//线程栈起始地址
                   sizeof(codec_thread_stack),	//线程栈大小
                   CODEC_THREAD_PRIORITY, 		//线程优先级
				   CODEC_THREAD_TIMESLICE);		//线程时间片大小
				   
	rt_thread_init(&sw_thread,					//线程控制块
                   "sw_thread",				//线程控制块名字
                   sw_thread_entry,			//线程入口函数
                   RT_NULL,						//线程入口函数的参数
                   &sw_thread_stack[0],		//线程栈起始地址
                   sizeof(sw_thread_stack),	//线程栈大小
                   SW_THREAD_PRIORITY, 		//线程优先级
				   SW_THREAD_TIMESLICE);		//线程时间片大小
				   
	rt_thread_init(&oled_thread,					//线程控制块
                   "oled_thread",				//线程控制块名字
                   oled_thread_entry,			//线程入口函数
                   RT_NULL,						//线程入口函数的参数
                   &oled_thread_stack[0],		//线程栈起始地址
                   sizeof(oled_thread_stack),	//线程栈大小
                   OLED_THREAD_PRIORITY, 		//线程优先级
				   OLED_THREAD_TIMESLICE);		//线程时间片大小
				   
	rt_thread_init(&adc_thread,					//线程控制块
                   "adc_thread",				//线程控制块名字
                   adc_thread_entry,			//线程入口函数
                   RT_NULL,						//线程入口函数的参数
                   &adc_thread_stack[0],		//线程栈起始地址
                   sizeof(adc_thread_stack),	//线程栈大小
                   ADC_THREAD_PRIORITY, 		//线程优先级
				   ADC_THREAD_TIMESLICE);		//线程时间片大小
				   
	rt_thread_init(&board_temp_thread,					//线程控制块
                   "board_temp_thread",				//线程控制块名字
                   board_temp_thread_entry,			//线程入口函数
                   RT_NULL,						//线程入口函数的参数
                   &board_temp_thread_stack[0],		//线程栈起始地址
                   sizeof(board_temp_thread_stack),	//线程栈大小
                   BOARD_TEMP_THREAD_PRIORITY, 		//线程优先级
				   BOARD_TEMP_THREAD_TIMESLICE);		//线程时间片大小
				   
	rt_thread_init(&pled_monitor_thread,					//线程控制块
                   "pled_monitor_thread",				//线程控制块名字
                   pled_monitor_thread_entry,			//线程入口函数
                   RT_NULL,						//线程入口函数的参数
                   &pled_monitor_thread_stack[0],		//线程栈起始地址
                   sizeof(pled_monitor_thread_stack),	//线程栈大小
                   PLED_MONITOR_THREAD_PRIORITY, 		//线程优先级
				   PLED_MONITOR_THREAD_TIMESLICE);		//线程时间片大小
				   
	rt_thread_init(&led_thread,					//线程控制块
                   "led_thread",				//线程控制块名字
                   led_thread_entry,			//线程入口函数
                   RT_NULL,						//线程入口函数的参数
                   &led_thread_stack[0],		//线程栈起始地址
                   sizeof(led_thread_stack),	//线程栈大小
                   LED_THREAD_PRIORITY, 		//线程优先级
				   LED_THREAD_TIMESLICE);		//线程时间片大小
				   
//	rt_thread_init(&test_temp_thread,					//线程控制块
//                   "test_temp_thread",				//线程控制块名字
//                   test_thread_entry,			//线程入口函数
//                   RT_NULL,						//线程入口函数的参数
//                   &test_temp_thread_stack[0],		//线程栈起始地址
//                   sizeof(test_temp_thread_stack),	//线程栈大小
//                   TEST_TEMP_THREAD_PRIORITY, 		//线程优先级
//				   TEST_TEMP_THREAD_TIMESLICE);		//线程时间片大小
		
	
	rt_thread_startup(&codec_thread);
	rt_thread_startup(&sw_thread);
	rt_thread_startup(&oled_thread);					   
	rt_thread_startup(&adc_thread);	
	rt_thread_startup(&board_temp_thread);		   
	rt_thread_startup(&led_thread); 
	rt_thread_startup(&mled_thread);
	rt_thread_startup(&pled_monitor_thread);	
				   
	//rt_thread_startup(&test_temp_thread);
				
	
				   
	return 0;
}




struct rt_semaphore oled_refresh_sem;  //创建一个信号量,
void oled_thread_entry(void *par)
{
	uint16_t count = 0;
	
	OLED_Init();
	
	Oled_Poll();
	
	rt_sem_init(&oled_refresh_sem, "oled_refresh_sem", 0, RT_IPC_FLAG_FIFO); //创建信号量
	
	timer_static_sample();
	
	while(1)
	{
		//rt_thread_mdelay(75);		//刷新率 25hz
		//Oled_Poll();	
		
		rt_sem_control(&oled_refresh_sem, RT_IPC_CMD_RESET, RT_NULL); 	//等待前清零信号量，防止误操作
		rt_sem_take(&oled_refresh_sem, RT_WAITING_FOREVER);			//持续等待信号量

		for(count=0;count<6;count++)		//OLED执行一次用时较长，当编码器转动较快时，可能存在漏更新，所以，收到一次信号量执行n次更新
		{
			Oled_Poll();					//执行一次约用时110ms
			rt_thread_mdelay(75);
		}
	}
	
}



void led_thread_entry(void *par)
{
	uint16_t count = 0;
	
	while(1)
	{
		
		LED_RUN_OFF();
		rt_thread_mdelay(75);
		//LED_OVLD_ON();
		
		LED_RUN_ON();
		rt_thread_mdelay(1050);
		//LED_OVLD_OFF();
	}
	
}



static void timeout1(void* parameter)
{
	rt_sem_release(&oled_refresh_sem);
}


int timer_static_sample(void)
{
    /* 初始化定时器 */
    rt_timer_init(&timer1, "timer1",  /* 定时器名字是 timer1 */
                    timeout1, /* 超时时回调的处理函数 */
                    RT_NULL, /* 超时函数的入口参数 */
                    3000, /* 定时长度，以 OS Tick 为单位，即 10 个 OS Tick */
                    RT_TIMER_FLAG_PERIODIC); /* 周期性定时器 */

    /* 启动定时器 */
    rt_timer_start(&timer1);
    return 0;
}




















