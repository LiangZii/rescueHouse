#ifndef _MPU6050_H_
#define _MPU6050_H_

#include <string.h>

//  变量声明
extern unsigned char display_flag;                                      //  显示标志位
extern unsigned char databuf[20];                                       //  数据缓冲区
extern unsigned char flag;

//	函数声明
void MPU6050ServerLoop(void);

#endif
