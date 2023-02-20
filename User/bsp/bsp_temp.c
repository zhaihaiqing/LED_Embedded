
#include "main.h"

void DQ_Temp_Init(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);				//��ʼ���ö˿�
	
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


unsigned char DS18B20_Rst(void)			//��λDS18B20
{
	unsigned char dat;
	
	SET_DS18B20_DQ_OUT;
	DQ_OUT_L;//��������
	Delay_us(510);//��ʱ����480us,�˴�Ϊ500us
	DQ_OUT_H;	//�ͷ�����
	SET_DS18B20_DQ_IN;
	Delay_us(63);//�ȴ�����10-60us,�˴��ȴ�63us
	
	dat=DQ_Read();
	
	Delay_us(240);//��ʱ���ͷ�����
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
		SET_DS18B20_DQ_OUT;//����Ϊ���
		
		DQ_OUT_L;					//��������
		Delay_us(2);
		SET_DS18B20_DQ_IN;//���ó����룬���ⲿ�������轫�������ߣ��ͷ�����
		Delay_us(2);
		if(DQ_Read())//��ȡ�˿�ֵ
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
			DQ_OUT_L;//дʱ���϶���Ǵ����ߵĵ͵�ƽ��ʼ
			Delay_us(2);//15us������
			DQ_OUT_H;
			Delay_us(69);//69us//����д1ʱ϶������60us
		}
		else
		{
			DQ_OUT_L;
			Delay_us(69);//������60-120us֮��
			DQ_OUT_H;
			Delay_us(2);//
		}
		dat>>=1;
	}
}


void DS18B20_ReadROM(void)
{
	unsigned char i;
	
	rt_enter_critical();	//�����ٽ������ر��������
	DS18B20_Rst();
	DS18B20_Write_Byte(0x33);//���Ͷ�rom����
	
	for(i=0;i<8;i++)				//��rom
	{
		DS18B20_Rom_Addr[i]=DS18B20_Read_Byte();
	}
	rt_exit_critical();	//�˳��ٽ����������������

	log_info("Rom_Addr: ");
	for(i=0;i<8;i++)
	{
		log_info("0x%x ",DS18B20_Rom_Addr[i]);
	}
	log_info("\r\n");

}


float DS18B20_SendConV_Command(void)
{
	rt_enter_critical();	//�����ٽ������ر��������
	if(DS18B20_Rst())
	{
		rt_exit_critical();	//�˳��ٽ����������������
		return -85;
	}
	DS18B20_Write_Byte(0xcc);
	DS18B20_Write_Byte(0x44);
	rt_exit_critical();	//�˳��ٽ����������������
	return 0;
}


float get_DS18B20_TEMP(void)
{
	float tt,result;
	unsigned short temp;
	unsigned char a,b;
		
	rt_enter_critical();	//�����ٽ������ر��������
	if(DS18B20_Rst())
	{
		rt_exit_critical();	//�˳��ٽ����������������
		return -85;
	}
	DS18B20_Write_Byte(0xcc);	//����ROMƥ��
	DS18B20_Write_Byte(0xbe);	//��ȡת������
	a=DS18B20_Read_Byte();
	b=DS18B20_Read_Byte();
	
	rt_exit_critical();	//�˳��ٽ����������������

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
	Result=Result/128;
	Temp=Result*0.5;
	
//	log_info("LM75_Result:0x%x\r\n",Result);
//	log_info("LM75_temp:%.1f��\r\n",Temp);
//	
	//log_info("addr:0x%x,temp:%f\r\n",addr,Temp);
	return Temp;
}

















































