#ifndef _MPU6050_H_
#define _MPU6050_H_

#include <string.h>

//  ��������
extern unsigned char display_flag;                                      //  ��ʾ��־λ
extern unsigned char databuf[20];                                       //  ���ݻ�����
extern unsigned char flag;

//	��������
void MPU6050ServerLoop(void);

#endif
