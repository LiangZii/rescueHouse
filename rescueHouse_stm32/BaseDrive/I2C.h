#ifndef I2C_H
#define I2C_H
#include "stm32f4xx.h"

// 	引脚定义
//SCL	PF1
//SDA	PF0

//SCL		PA6
#define  	SCL_L      		GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define  	SCL_H      		GPIO_SetBits(GPIOA, GPIO_Pin_6)

//SDA		PA3
#define  	SDA_L      		GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define  	SDA_H      		GPIO_SetBits(GPIOA, GPIO_Pin_3)

#define  	SDA_READ     	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)

//  函数声明
void I2C_SDA_Input(void);
void I2C_SDA_Output(void);
void I2C_GPIO_Configuration(void);
void I2C_Start(void);
void I2C_Stop(void);
unsigned char I2C_WaitACK(void);
void I2C_ACK(void);
void I2C_NACK(void);
void I2C_SendByte(unsigned char txd);
//unsigned char I2C_ReceiveByte(unsigned char ack);
void I2C_delay(void);
unsigned char I2C_ReceiveByte(void);
void I2C_SendACK(unsigned char ack);

#endif
