
#ifndef __BSP_I2CC_PORT_H
#define __BSP_I2CC_PORT_H


//#define GPIO_Pin_0_NUM		0
//#define GPIO_Pin_1_NUM		1
//#define GPIO_Pin_2_NUM		2
//#define GPIO_Pin_3_NUM		3
//#define GPIO_Pin_4_NUM		4
//#define GPIO_Pin_5_NUM		5
//#define GPIO_Pin_6_NUM		6
//#define GPIO_Pin_7_NUM		7
//#define GPIO_Pin_8_NUM		8
//#define GPIO_Pin_9_NUM		9
//#define GPIO_Pin_10_NUM		10
//#define GPIO_Pin_11_NUM		11
//#define GPIO_Pin_12_NUM		12
//#define GPIO_Pin_13_NUM		13
//#define GPIO_Pin_14_NUM		14
//#define GPIO_Pin_15_NUM		15


/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define I2CC_SCL_RCC_PORT 		RCC_AHB1Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define I2CC_SCL_GPIO_PORT		GPIOB						/* GPIO�˿� */
#define I2CC_SCL_GPIO_Pin		GPIO_Pin_10					/* ���ӵ�SCLʱ���ߵ�GPIO */
//#define I2CC_SCL_GPIO_Pin_NUM	GPIO_Pin_10_NUM					/* ���ӵ�SCLʱ���ߵ�GPIO */

#define I2CC_SDA_RCC_PORT 		RCC_AHB1Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define I2CC_SDA_GPIO_PORT		GPIOB						/* GPIO�˿� */
#define I2CC_SDA_GPIO_Pin		GPIO_Pin_11					/* ���ӵ�SCLʱ���ߵ�GPIO */
//#define I2CC_SDA_GPIO_Pin_NUM	GPIO_Pin_11_NUM					/* ���ӵ�SCLʱ���ߵ�GPIO */


/* �����дSCL��SDA�ĺ� */
#define I2CC_SCL_1()  			I2CC_SCL_GPIO_PORT->BSRRL = I2CC_SCL_GPIO_Pin				/* SCL = 1 */
#define I2CC_SCL_0()  			I2CC_SCL_GPIO_PORT->BSRRH = I2CC_SCL_GPIO_Pin				/* SCL = 0 */

#define I2CC_SDA_1()  			I2CC_SDA_GPIO_PORT->BSRRL = I2CC_SDA_GPIO_Pin				/* SDA = 1 */
#define I2CC_SDA_0()  			I2CC_SDA_GPIO_PORT->BSRRH = I2CC_SDA_GPIO_Pin				/* SDA = 0 */

#define I2CC_SCL_READ()  		((I2CC_SCL_GPIO_PORT->IDR & I2CC_SCL_GPIO_Pin) != 0)	/* ��SCL����״̬ */
#define I2CC_SDA_READ()  		((I2CC_SDA_GPIO_PORT->IDR & I2CC_SDA_GPIO_Pin) != 0)	/* ��SDA����״̬ */










void i2cc_port_init(void);
void i2cc_port_start(void);
void i2cc_port_stop(void);
unsigned char i2cc_port_wait_ack(void);
void i2cc_port_ack(void);
void i2cc_port_nack(void);
void i2cc_port_send_byte(unsigned char txd);
unsigned char i2cc_port_read_byte(void);
unsigned char i2cc_CheckDevice(unsigned char Device_addr);



#endif


