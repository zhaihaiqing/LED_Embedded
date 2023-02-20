
#include "main.h"

void DQ_Temp_Init(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);				//初始化该端口
	
}


unsigned char DS18B20_Rom_Addr[8];

//void DS18B20_delayus(unsigned int us)
//{	
//	unsigned int  i=0;
////	OS_ERR err;
////	CPU_SR_ALLOC();
//	
//	//OS_CRITICAL_ENTER();
//	
//	for(i=0;i<us;i++)
//	{__nop();}
//	
//	//OS_CRITICAL_EXIT();
//}

//cpu_clk=7.3728MHz
//#define ds18b20_t_538us	480
//#define ds18b20_t_69us	60
//#define ds18b20_t_66us	58
//#define ds18b20_t_63us	55
//#define ds18b20_t_6us	2
//#define ds18b20_t_4us	1
//#define ds18b20_t_2us	0

//cpu_clk=29.4912MHz
//#define ds18b20_t_538us	2000
//#define ds18b20_t_69us	250
//#define ds18b20_t_66us	240
//#define ds18b20_t_63us	230
//#define ds18b20_t_6us		20
//#define ds18b20_t_4us		11
//#define ds18b20_t_2us		4

//cpu_clk=168MHz
#define ds18b20_t_538us		18000
#define ds18b20_t_260us		9000
#define ds18b20_t_69us		2320
#define ds18b20_t_66us		2250
#define ds18b20_t_63us		2100
#define ds18b20_t_6us		198
#define ds18b20_t_4us		130
#define ds18b20_t_2us		60


unsigned char DS18B20_Rst(void)			//复位DS18B20
{
	unsigned char dat;
	
	SET_DS18B20_DQ_OUT;
	DQ_OUT_L;//拉低总线
	Delay_us(510);//延时至少480us,此处为500us
	DQ_OUT_H;	//释放总线
	SET_DS18B20_DQ_IN;
	Delay_us(63);//等待至少10-60us,此处等待63us
	
	dat=DQ_Read();
	
	Delay_us(240);//延时，释放总线
	SET_DS18B20_DQ_OUT;
	DQ_OUT_H;
	
	return dat;	
}

unsigned char DS18B20_Read_Byte(void)
{
	unsigned char i,dat=0;
	
	for(i=0;i<8;i++)
	{
		
		dat>>=1;
		SET_DS18B20_DQ_OUT;//设置为输出
		
		DQ_OUT_L;					//拉低总线
		Delay_us(2);
		SET_DS18B20_DQ_IN;//设置成输入，由外部上拉电阻将总线拉高，释放总线
		Delay_us(2);
		if(DQ_Read())//获取端口值
		dat|=0x80;
		Delay_us(66);//66us
	}
	SET_DS18B20_DQ_OUT;
	return dat;
}

void DS18B20_Write_Byte(unsigned char dat)
{
	unsigned char i;
	SET_DS18B20_DQ_OUT;
	for(i=0;i<8;i++)
	{
		if(dat&0x01)
		{	
			DQ_OUT_L;//写时间空隙总是从总线的低电平开始
			Delay_us(2);//15us内拉高
			DQ_OUT_H;
			Delay_us(69);//69us//整个写1时隙不低于60us
		}
		else
		{
			DQ_OUT_L;
			Delay_us(69);//保持在60-120us之间
			DQ_OUT_H;
			Delay_us(2);//
		}
		dat>>=1;
	}
}


void DS18B20_ReadROM(void)
{
	unsigned char i;
	
	rt_enter_critical();	//进入临界区，关闭任务调度
	DS18B20_Rst();
	DS18B20_Write_Byte(0x33);//发送读rom命令
	
	for(i=0;i<8;i++)				//读rom
	{
		DS18B20_Rom_Addr[i]=DS18B20_Read_Byte();
	}
	rt_exit_critical();	//退出临界区，开启任务调度

	log_info("Rom_Addr: ");
	for(i=0;i<8;i++)
	{
		log_info("0x%x ",DS18B20_Rom_Addr[i]);
	}
	log_info("\r\n");

}


float DS18B20_SendConV_Command(void)
{
	rt_enter_critical();	//进入临界区，关闭任务调度
	if(DS18B20_Rst())
	{
		rt_exit_critical();	//退出临界区，开启任务调度
		return -85;
	}
	DS18B20_Write_Byte(0xcc);
	DS18B20_Write_Byte(0x44);
	rt_exit_critical();	//退出临界区，开启任务调度
	return 0;
}


float get_DS18B20_TEMP(void)
{
	float tt,result;
	unsigned short temp;
	unsigned char a,b;
		
	rt_enter_critical();	//进入临界区，关闭任务调度
	if(DS18B20_Rst())
	{
		rt_exit_critical();	//退出临界区，开启任务调度
		return -85;
	}
	DS18B20_Write_Byte(0xcc);	//跳过ROM匹配
	DS18B20_Write_Byte(0xbe);	//获取转换数据
	a=DS18B20_Read_Byte();
	b=DS18B20_Read_Byte();
	
	rt_exit_critical();	//退出临界区，开启任务调度

	temp=(b<<8) | a ;
	
	
	if(temp & 0xf800)
	{
		temp=~temp+1;
		tt=(temp&0x07ff)*0.0625;
		result= 0-tt;
	}
	else 
	{
		tt=(temp&0x07ff)*0.0625;
		result = tt;
	}

	//log_info("DS18B20:%.2f\r\n",result);
	return result;
}







//#define TEMP_A_ADDR		0x90
//#define TEMP_B_ADDR		0x91

//#define LM75_REG_TEMP        0x00



short Read_TMP75_Byte(unsigned char SLAVE_Device_Addr,unsigned char Reg_Addr)
{
	uint8_t msb=0,lsb=0;
	uint16_t Reg_Value=0;
	rt_enter_critical();
	
    i2cb_port_start();
	
	i2cb_port_send_byte( SLAVE_Device_Addr);		//发送器件地址,写数据 	 
	  
	i2cb_port_wait_ack(); 
    i2cb_port_send_byte(Reg_Addr);						//发送低地址
	i2cb_port_wait_ack();
	
	i2cb_port_start();
	i2cb_port_send_byte(SLAVE_Device_Addr | 0x01);				//进入接收模式
	i2cb_port_wait_ack();
    msb=i2cb_port_read_byte();
    i2cb_port_wait_ack();											//产生一个停止条件
	lsb=i2cb_port_read_byte();
	i2cb_port_wait_ack();
	i2cb_port_stop();
	
	rt_exit_critical();
	
	Reg_Value = (msb<<8) | lsb;
	
	//log_info("temp:msb:0x%x,lsb:0x%x\r\n",msb,lsb);
	
	return Reg_Value;
}








//获取温度 
float GetTempValue(uint8_t addr) 
{  
	short Result=0; 
	float Temp=0.0;
	//Result=Read_TMP75_Byte(I2C2,LM75_ADDR,LM75_REG_ID);  //读取ID 
	//log_info("LM75_ID:0x%x\r\n",(unsigned char)Result);	

	Result=Read_TMP75_Byte(TEMP_A_ADDR,LM75_REG_TEMP);  //读取温度
	Result=Result/128;
	Temp=Result*0.5;
	
//	log_info("LM75_Result:0x%x\r\n",Result);
//	log_info("LM75_temp:%.1f℃\r\n",Temp);
//	
	//log_info("addr:0x%x,temp:%f\r\n",addr,Temp);
	return Temp;
}

















































