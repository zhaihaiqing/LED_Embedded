
#include "main.h"


//#define TEMP_A_ADDR		0x90
//#define TEMP_B_ADDR		0x91

//#define LM75_REG_TEMP        0x00



short Read_TMP75_Byte(unsigned char SLAVE_Device_Addr,unsigned char Reg_Addr)
{
	uint8_t msb=0,lsb=0;
	uint16_t Reg_Value=0;
	rt_enter_critical();
	
    i2cb_port_start();
	
	i2cb_port_send_byte( SLAVE_Device_Addr);		//����������ַ,д���� 	 
	  
	i2cb_port_wait_ack(); 
    i2cb_port_send_byte(Reg_Addr);						//���͵͵�ַ
	i2cb_port_wait_ack();
	
	i2cb_port_start();
	i2cb_port_send_byte(SLAVE_Device_Addr | 0x01);				//�������ģʽ
	i2cb_port_wait_ack();
    msb=i2cb_port_read_byte();
    i2cb_port_wait_ack();											//����һ��ֹͣ����
	lsb=i2cb_port_read_byte();
	i2cb_port_wait_ack();
	i2cb_port_stop();
	
	rt_exit_critical();
	
	Reg_Value = (msb<<8) | lsb;
	
	//log_info("temp:msb:0x%x,lsb:0x%x\r\n",msb,lsb);
	
	return Reg_Value;
}



//��ȡ�¶� 
float GetTempValue(uint8_t addr) 
{  
	short Result=0; 
	float Temp=0.0;
	//Result=Read_TMP75_Byte(I2C2,LM75_ADDR,LM75_REG_ID);  //��ȡID 
	//log_info("LM75_ID:0x%x\r\n",(unsigned char)Result);	

	Result=Read_TMP75_Byte(TEMP_A_ADDR,LM75_REG_TEMP);  //��ȡ�¶�
	Temp=( Result >> 4 )*0.0625;
	
//	log_info("LM75_Result:0x%x\r\n",Result);
//	log_info("LM75_temp:%.1f��\r\n",Temp);
//	
	//log_info("addr:0x%x,temp:%f\r\n",addr,Temp);
	return Temp;
}

















































