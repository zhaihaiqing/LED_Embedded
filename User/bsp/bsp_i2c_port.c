/**
  ******************************************************************************
  * @file    Project/BSP/bsp_i2c_port.c 
  * @author  zhaihaiqing
  * @version V1.0.0
  * @date    5-April-2020
  * @brief   使用IO模拟I2C
  ******************************************************************************
  */
#include "main.h"

void oled_i2c_port_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOB时钟

	//GPIOE11,E10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	//GPIOE11,E10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	
	OLED_I2C_PORT_SCL_H();
	OLED_I2C_PORT_SDA_H();
}

//产生IIC起始信号
void oled_i2c_port_start(void)
{
	SET_OLED_I2C_PORT_SDA_OUT();     //sda线输出
	OLED_I2C_PORT_SDA_H();	  	  
	OLED_I2C_PORT_SCL_H();
	Delay_us(4);
 	OLED_I2C_PORT_SDA_L();//START:when CLK is high,DATA change form high to low 
	Delay_us(4);
	OLED_I2C_PORT_SCL_L();//钳住I2C总线，准备发送或接收数据 
}
//产生IIC停止信号
void oled_i2c_port_stop(void)
{
	SET_OLED_I2C_PORT_SDA_OUT();//sda线输出
	OLED_I2C_PORT_SCL_L();
	OLED_I2C_PORT_SDA_L();//STOP:when CLK is high DATA change form low to high
 	Delay_us(4);
	OLED_I2C_PORT_SCL_H();
	OLED_I2C_PORT_SDA_H();//发送I2C总线结束信号
	Delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
unsigned char oled_i2c_port_wait_ack(void)
{
//	u8 ucErrTime=0;
//	SET_OLED_I2C_PORT_SDA_IN();      //SDA设置为输入  
//	OLED_I2C_PORT_SDA_H();Delay_us(1);	   
//	OLED_I2C_PORT_SCL_H();Delay_us(1);	 
//	while(OLED_I2C_PORT_READ_SDA())
//	{
//		ucErrTime++;
//		if(ucErrTime>250)
//		{
//			oled_i2c_port_stop();
//			return 1;
//		}
//	}
	OLED_I2C_PORT_SCL_H();
	Delay_us(1);
	OLED_I2C_PORT_SCL_L();//时钟输出0 	
	
	return 0;  
} 
//产生ACK应答
void oled_i2c_port_ack(void)
{
	OLED_I2C_PORT_SCL_L();
	SET_OLED_I2C_PORT_SDA_OUT();
	OLED_I2C_PORT_SDA_L();
	Delay_us(2);
	OLED_I2C_PORT_SCL_H();
	Delay_us(2);
	OLED_I2C_PORT_SCL_L();
}
//不产生ACK应答		    
void oled_i2c_port_nack(void)
{
	OLED_I2C_PORT_SCL_L();
	SET_OLED_I2C_PORT_SDA_OUT();
	OLED_I2C_PORT_SDA_H();
	Delay_us(2);
	OLED_I2C_PORT_SCL_H();
	Delay_us(2);
	OLED_I2C_PORT_SCL_L();
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void oled_i2c_port_send_byte(unsigned char txd)
{                        
    unsigned char t;   
	SET_OLED_I2C_PORT_SDA_OUT();	    
    OLED_I2C_PORT_SCL_L();//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>>7)OLED_I2C_PORT_SDA_H();
		else	OLED_I2C_PORT_SDA_L();
        txd<<=1; 	  
		Delay_us(2);   //对TEA5767这三个延时都是必须的
		OLED_I2C_PORT_SCL_H();
		Delay_us(2); 
		OLED_I2C_PORT_SCL_L();
		Delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
unsigned char oled_i2c_port_read_byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SET_OLED_I2C_PORT_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        OLED_I2C_PORT_SCL_L(); 
        Delay_us(2);
		OLED_I2C_PORT_SCL_H();
        receive<<=1;
        if(OLED_I2C_PORT_READ_SDA())receive++;   
		Delay_us(2); 
    }					 
    if (!ack)
        oled_i2c_port_nack();//发送nACK
    else
        oled_i2c_port_ack(); //发送ACK   
    return receive;
}







void i2c_port_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

	//GPIOE11,E10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	I2C_PORT_SCL_H();
	I2C_PORT_SDA_H();
}

//产生IIC起始信号
void i2c_port_start(void)
{
	SET_I2C_PORT_SDA_OUT();     //sda线输出
	I2C_PORT_SDA_H();	  	  
	I2C_PORT_SCL_H();
	Delay_us(4);
 	I2C_PORT_SDA_L();//START:when CLK is high,DATA change form high to low 
	Delay_us(4);
	I2C_PORT_SCL_L();//钳住I2C总线，准备发送或接收数据 
}
//产生IIC停止信号
void i2c_port_stop(void)
{
	SET_I2C_PORT_SDA_OUT();//sda线输出
	I2C_PORT_SCL_L();
	I2C_PORT_SDA_L();//STOP:when CLK is high DATA change form low to high
 	Delay_us(4);
	I2C_PORT_SCL_H();
	Delay_us(4);
	I2C_PORT_SDA_H();//发送I2C总线结束信号	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
unsigned char i2c_port_wait_ack(void)
{
//	u8 ucErrTime=0;
//	SET_I2C_PORT_SDA_IN();      //SDA设置为输入  
//	I2C_PORT_SDA_H();Delay_us(1);	   
//	I2C_PORT_SCL_H();Delay_us(1);	 
//	while(I2C_PORT_READ_SDA())
//	{
//		ucErrTime++;
//		if(ucErrTime>250)
//		{
//			i2c_port_stop();
//			return 1;
//		}
//	}
	I2C_PORT_SCL_H();
	Delay_us(1);
	I2C_PORT_SCL_L();//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void i2c_port_ack(void)
{
	I2C_PORT_SCL_L();
	SET_I2C_PORT_SDA_OUT();
	I2C_PORT_SDA_L();
	Delay_us(2);
	I2C_PORT_SCL_H();
	Delay_us(2);
	I2C_PORT_SCL_L();
}
//不产生ACK应答		    
void i2c_port_nack(void)
{
	I2C_PORT_SCL_L();
	SET_I2C_PORT_SDA_OUT();
	I2C_PORT_SDA_H();
	Delay_us(2);
	I2C_PORT_SCL_H();
	Delay_us(2);
	I2C_PORT_SCL_L();
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void i2c_port_send_byte(unsigned char txd)
{                        
    unsigned char t;   
	SET_I2C_PORT_SDA_OUT();	    
    I2C_PORT_SCL_L();//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>>7)I2C_PORT_SDA_H();
		else	I2C_PORT_SDA_L();
        txd<<=1; 	  
		Delay_us(2);   //
		I2C_PORT_SCL_H();
		Delay_us(2); 
		I2C_PORT_SCL_L();
		Delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
unsigned char i2c_port_read_byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SET_I2C_PORT_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        I2C_PORT_SCL_L(); 
        Delay_us(2);
		I2C_PORT_SCL_H();
        receive<<=1;
        if(I2C_PORT_READ_SDA())receive++;   
		Delay_us(2); 
    }					 
    if (!ack)
        i2c_port_nack();//发送nACK
    else
        i2c_port_ack(); //发送ACK   
    return receive;
}
















