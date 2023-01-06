
#ifndef __BSP_I2C_PORT_H
#define __BSP_I2C_PORT_H

//oled_I2C ʱ����---PB0
#define OLED_I2C_PORT_SCL_H()		GPIOB->BSRRL = GPIO_Pin_0
#define OLED_I2C_PORT_SCL_L()		GPIOB->BSRRH = GPIO_Pin_0

//oled_I2C ������---PC5
#define OLED_I2C_PORT_SDA_H()		GPIOC->BSRRL = GPIO_Pin_5
#define OLED_I2C_PORT_SDA_L()		GPIOC->BSRRH = GPIO_Pin_5

#define OLED_I2C_PORT_READ_SDA() 	(GPIOC->IDR & GPIO_Pin_5)

#define	OLED_I2C_PORT_SDA_NUM		10

#define SET_OLED_I2C_PORT_SDA_IN()     GPIOB->MODER   &= 0xfffffcff;
										//GPIOB->MODER   &= ~(3<<(I2C_PORT_SDA_NUM*2));
										//GPIOB->PUPDR   &= 0xffcfffff;GPIOB->PUPDR   |= 0x00100000
										//�������빦��
										//��������
#define SET_OLED_I2C_PORT_SDA_OUT()   {GPIOB->MODER   &= 0xfffffcff;GPIOB->MODER   |= 0x00000400;}

									//GPIOB->OTYPER  &= 0xffffefff;GPIOB->OTYPER  |= 0x00001000;\
									//GPIOB->OSPEEDR &= 0xfcffffff;GPIOB->OSPEEDR |= 0x03000000;\
									//GPIOB->PUPDR   &= 0xfcffffff;GPIOB->PUPDR   |= 0x01000000
									//����Ϊ�������
									//�����������Ϊ��©
									//��������ٶ�Ϊ40MHz
									//��������



//I2C ʱ����---PB10
#define I2C_PORT_SCL_H()		GPIOB->BSRRL = GPIO_Pin_10
#define I2C_PORT_SCL_L()		GPIOB->BSRRH = GPIO_Pin_10

//I2C ������---PB11
#define I2C_PORT_SDA_H()		GPIOB->BSRRL = GPIO_Pin_11
#define I2C_PORT_SDA_L()		GPIOB->BSRRH = GPIO_Pin_11

#define I2C_PORT_READ_SDA() 	(GPIOB->IDR & GPIO_Pin_11)

#define	I2C_PORT_SDA_NUM		10

#define SET_I2C_PORT_SDA_IN()     GPIOB->MODER   &= 0xffcfffff;
										//GPIOB->MODER   &= ~(3<<(I2C_PORT_SDA_NUM*2));
										//GPIOB->PUPDR   &= 0xffcfffff;GPIOB->PUPDR   |= 0x00100000
										//�������빦��
										//��������
#define SET_I2C_PORT_SDA_OUT()   {GPIOB->MODER   &= 0xffcfffff;GPIOB->MODER   |= 0x00400000;}

									//GPIOB->OTYPER  &= 0xffffefff;GPIOB->OTYPER  |= 0x00001000;\
									//GPIOB->OSPEEDR &= 0xfcffffff;GPIOB->OSPEEDR |= 0x03000000;\
									//GPIOB->PUPDR   &= 0xfcffffff;GPIOB->PUPDR   |= 0x01000000
									//����Ϊ�������
									//�����������Ϊ��©
									//��������ٶ�Ϊ40MHz
									//��������


void oled_i2c_port_init(void);
void oled_i2c_port_start(void);
void oled_i2c_port_stop(void);
unsigned char oled_i2c_port_wait_ack(void);
void oled_i2c_port_ack(void);
void oled_i2c_port_nack(void);
void oled_i2c_port_send_byte(unsigned char txd);
unsigned char oled_i2c_port_read_byte(unsigned char ack);



void i2c_port_init(void);
void i2c_port_start(void);
void i2c_port_stop(void);
unsigned char i2c_port_wait_ack(void);
void i2c_port_ack(void);
void i2c_port_nack(void);
void i2c_port_send_byte(unsigned char txd);
unsigned char i2c_port_read_byte(unsigned char ack);



#endif


