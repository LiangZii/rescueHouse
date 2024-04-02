#ifndef _BH1750_H_
#define _BH1750_H_

#include "I2C.h"
#include <stdio.h>


#define	BH1750_SlaveAddress	    0xB8	                                //  I2Cд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ


void BH1750WriteData(unsigned char DeviceAddr, unsigned char DataAddr);
void BH1750_ReadData(unsigned char Dev_Address, unsigned char* ptr);
void BH1750_Init(void);

#endif
