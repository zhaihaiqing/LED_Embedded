
#include "main.h"

void DQ_Temp_Init(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = DQ1_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(DQ1_GPIO_Port, &GPIO_InitStructure);//��ʼ��
	
	
	GPIO_InitStructure.GPIO_Pin = DQ2_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(DQ2_GPIO_Port, &GPIO_InitStructure);//��ʼ��
	
//	DQ1_OUT_L;
//	DQ2_OUT_L;
}


unsigned char DS18B20_Rom_Addr[8];

//void DQ_delayus(unsigned int us)
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


uint8_t DQ_crc8( uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;
	uint8_t mix=0;
	uint8_t i=0;
	uint8_t inbyte = 0;
	
	while (len--) 
	{
		inbyte = *addr++;
		for (i = 8; i; i--) 
		{
			mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}

unsigned char DQ1_Rst(void)			//��λDS18B20
{
	unsigned char dat;
	
	SET_DQ1_OUT;
	DQ1_OUT_L;//��������
	Delay_us(510);//��ʱ480-960us,�˴�Ϊ500us
	DQ1_OUT_H;	//�ͷ�����
	SET_DQ1_IN;
	Delay_us(55);//�ȴ�����10-60us,�˴��ȴ�63us
	
	dat=DQ1_Read();
	Delay_us(240);//��ʱ���ͷ�����
	SET_DQ1_OUT;
	DQ1_OUT_H;
	
	return dat;	
}

unsigned char DQ1_Read_Byte(void)
{
	unsigned char i,dat=0;
	
	for(i=0;i<8;i++)
	{
		
		dat>>=1;
		SET_DQ1_OUT;//����Ϊ���
		
		DQ1_OUT_L;					//��������
		Delay_us(10);
		DQ1_OUT_H;	
		SET_DQ1_IN;//���ó����룬���ⲿ�������轫�������ߣ��ͷ�����
		Delay_us(10);
		if(DQ1_Read())//��ȡ�˿�ֵ
		{
			dat|=0x80;
		}
		Delay_us(65);//66us
	}
	SET_DQ1_OUT;
	return dat;
}

void DQ1_Write_Byte(unsigned char dat)
{
	unsigned char i;
	SET_DQ1_OUT;
	for(i=0;i<8;i++)
	{
		if(dat&0x01)
		{	
			DQ1_OUT_L;//дʱ���϶���Ǵ����ߵĵ͵�ƽ��ʼ
			Delay_us(5);//15us������
			DQ1_OUT_H;
			Delay_us(60);//69us//����д1ʱ϶������60us
		}
		else
		{
			DQ1_OUT_L;
			Delay_us(60);//������60-120us֮��
			DQ1_OUT_H;
			Delay_us(5);//
		}
		dat>>=1;
	}
}


void DQ1_ReadROM(void)
{
	unsigned char i;
	uint8_t crc_8 = 0;
	
	rt_enter_critical();	//�����ٽ������ر��������
	DQ1_Rst();
	DQ1_Write_Byte(0x33);//���Ͷ�rom����
	
	for(i=0;i<8;i++)				//��rom
	{
		DS18B20_Rom_Addr[i]=DQ1_Read_Byte();
	}
	rt_exit_critical();	//�˳��ٽ����������������

	crc_8 = DQ_crc8(DS18B20_Rom_Addr,7);
	
	log_info("DQ1_Rom_Addr: ");
	for(i=0;i<8;i++)
	{
		log_info("0x%x ",DS18B20_Rom_Addr[i]);
	}
	log_info("  crc_8:0x%x ",crc_8);
	log_info("\r\n");

}


float DS18B20_A_SendConV_Command(void)
{
	rt_enter_critical();	//�����ٽ������ر��������
	if(DQ1_Rst())
	{
		rt_exit_critical();	//�˳��ٽ����������������
		return -85;
	}
	DQ1_Write_Byte(0xcc);
	DQ1_Write_Byte(0x44);
	rt_exit_critical();	//�˳��ٽ����������������
	return 0;
}


float get_DS18B20_A_TEMP(void)
{
	float tt,result;
	unsigned short temp;
	unsigned char a,b;
		
	rt_enter_critical();	//�����ٽ������ر��������
	if(DQ1_Rst())
	{
		rt_exit_critical();	//�˳��ٽ����������������
		return -85;
	}
	DQ1_Write_Byte(0xcc);	//����ROMƥ��
	DQ1_Write_Byte(0xbe);	//��ȡת������
	a=DQ1_Read_Byte();
	b=DQ1_Read_Byte();
	
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

	//log_info("DQ1:0x%x,%.2f\r\n",temp,result);
	return result;
}















unsigned char DQ2_Rst(void)			//��λDS18B20
{
	unsigned char dat;
	
	SET_DQ2_OUT;
	DQ2_OUT_L;//��������
	Delay_us(510);//��ʱ480-960us,�˴�Ϊ500us
	DQ2_OUT_H;	//�ͷ�����
	SET_DQ2_IN;
	Delay_us(55);//�ȴ�����10-60us,�˴��ȴ�63us
	
	dat=DQ2_Read();
	Delay_us(240);//��ʱ���ͷ�����
	SET_DQ2_OUT;
	DQ2_OUT_H;
	
	return dat;	
}

unsigned char DQ2_Read_Byte(void)
{
	unsigned char i,dat=0;
	
	for(i=0;i<8;i++)
	{
		
		dat>>=1;
		SET_DQ2_OUT;//����Ϊ���
		
		DQ2_OUT_L;					//��������
		Delay_us(10);
		DQ2_OUT_H;	
		SET_DQ2_IN;//���ó����룬���ⲿ�������轫�������ߣ��ͷ�����
		Delay_us(10);
		if(DQ2_Read())//��ȡ�˿�ֵ
		{
			dat|=0x80;
		}
		Delay_us(60);//66us
	}
	SET_DQ2_OUT;
	return dat;
}

void DQ2_Write_Byte(unsigned char dat)
{
	unsigned char i;
	SET_DQ2_OUT;
	for(i=0;i<8;i++)
	{
		if(dat&0x01)
		{	
			DQ2_OUT_L;//дʱ���϶���Ǵ����ߵĵ͵�ƽ��ʼ
			Delay_us(5);//15us������
			DQ2_OUT_H;
			Delay_us(60);//69us//����д1ʱ϶������60us
		}
		else
		{
			DQ2_OUT_L;
			Delay_us(60);//������60-120us֮��
			DQ2_OUT_H;
			Delay_us(5);//
		}
		dat>>=1;
	}
}


void DQ2_ReadROM(void)
{
	unsigned char i;
	uint8_t crc_8 = 0;
	
	rt_enter_critical();	//�����ٽ������ر��������
	DQ2_Rst();
	DQ2_Write_Byte(0x33);//���Ͷ�rom����
	
	for(i=0;i<8;i++)				//��rom
	{
		DS18B20_Rom_Addr[i]=DQ2_Read_Byte();
	}
	rt_exit_critical();	//�˳��ٽ����������������

	crc_8 = DQ_crc8(DS18B20_Rom_Addr,7);
	
	log_info("DQ2_Rom_Addr: ");
	for(i=0;i<8;i++)
	{
		log_info("0x%x ",DS18B20_Rom_Addr[i]);
	}
	log_info("  crc_8:0x%x ",crc_8);
	log_info("\r\n");

}


float DS18B20_B_SendConV_Command(void)
{
	rt_enter_critical();	//�����ٽ������ر��������
	if(DQ2_Rst())
	{
		rt_exit_critical();	//�˳��ٽ����������������
		return -85;
	}
	DQ2_Write_Byte(0xcc);
	Delay_us(10);
	DQ2_Write_Byte(0x44);
	rt_exit_critical();	//�˳��ٽ����������������
	return 0;
}


float get_DS18B20_B_TEMP(void)
{
	float tt,result;
	unsigned short temp;
	unsigned char a,b;
		
	rt_enter_critical();	//�����ٽ������ر��������
	if(DQ2_Rst())
	{
		rt_exit_critical();	//�˳��ٽ����������������
		return -85;
	}
	DQ2_Write_Byte(0xcc);	//����ROMƥ��
	DQ2_Write_Byte(0xbe);	//��ȡת������
	a=DQ2_Read_Byte();
	b=DQ2_Read_Byte();
	
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

	//log_info("DQ2:0x%x,%.2f\r\n",temp,result);
	return result;
}



































