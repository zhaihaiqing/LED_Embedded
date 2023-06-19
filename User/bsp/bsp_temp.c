
#include "main.h"


//#define TEMP_A_ADDR		0x90
//#define TEMP_B_ADDR		0x91

//#define LM75_REG_TEMP        0x00



//short Read_TMP75_Byte(unsigned char SLAVE_Device_Addr,unsigned char Reg_Addr)
//{
//	uint8_t msb=0,lsb=0;
//	uint16_t Reg_Value=0;
//	rt_enter_critical();
//	
//    i2cb_port_start();
//	
//	i2cb_port_send_byte( SLAVE_Device_Addr);		//发送器件地址,写数据 	 
//	i2cb_port_wait_ack(); 
//	
//    i2cb_port_send_byte(Reg_Addr);						//发送低地址
//	i2cb_port_wait_ack();
//	
//	i2cb_port_start();
//	
//	i2cb_port_send_byte(SLAVE_Device_Addr | 0x01);				//进入接收模式
//	i2cb_port_wait_ack();
//	
//    msb=i2cb_port_read_byte();
//    i2cb_port_ack();
//	//产生一个停止条件
//	lsb=i2cb_port_read_byte();
//	i2cb_port_nack();
//	
//	i2cb_port_stop();
//	
//	rt_exit_critical();
//	
//	Reg_Value = (msb<<8) | lsb;
//	
//	//log_info("temp:msb:0x%x,lsb:0x%x\r\n",msb,lsb);
//	
//	return Reg_Value;
//}


//获取温度 
//float GetTempValue(uint8_t addr) 
//{  
//	short Result=0; 
//	float Temp=0.0;
//	//Result=Read_TMP75_Byte(I2C2,LM75_ADDR,LM75_REG_ID);  //读取ID 
//	//log_info("LM75_ID:0x%x\r\n",(unsigned char)Result);	

//	Result=Read_TMP75_Byte(addr,LM75_REG_TEMP);  //读取温度
//	Temp=( Result >> 4 )*0.0625;
//	
//	log_info("LM75_Result:0x%x  ",Result);
//	log_info("LM75_temp:%.2f℃\r\n",Temp);
////	
//	//log_info("addr:0x%x,temp:%f\r\n",addr,Temp);
//	return Temp;
//}



uint16_t Read_TMP_Byte(unsigned char SLAVE_Device_Addr,unsigned char Reg_Addr)
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
    i2cb_port_ack();
	//产生一个停止条件
	lsb=i2cb_port_read_byte();
	i2cb_port_nack();
	
	i2cb_port_stop();
	
	rt_exit_critical();
	
	Reg_Value = (msb<<8) | lsb;
	
	//log_info("temp:msb:0x%x,lsb:0x%x\r\n",msb,lsb);
	
	return Reg_Value;
}




//获取温度 
float GetTempValue(uint8_t addr) 
{  
	uint16_t Result=0; 
	float Temp=0.0;
	//Result=Read_TMP75_Byte(I2C2,LM75_ADDR,LM75_REG_ID);  //读取ID 
	//log_info("LM75_ID:0x%x\r\n",(unsigned char)Result);	

	Result=Read_TMP_Byte(addr,LM75_REG_TEMP);  //读取温度
	
	if(Result <= 0x7fff)
	{
		Temp = Result*0.0078125;
	}
	else
	{
		Temp = 0-(~Result +1)  *0.0078125;
	}
	
	//log_info("LM75_Result:0x%x  ",Result);
	//log_info("LM75_temp:%.2f℃\r\n",Temp);
//	
	//log_info("addr:0x%x,temp:%f\r\n",addr,Temp);
	return Temp;
}

















































