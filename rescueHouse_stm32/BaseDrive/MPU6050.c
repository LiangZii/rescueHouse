#include "stm32f4xx.h"
#include "delay.h"
#include "USART3.h"
#include "MPU6050.h"
#include "stdio.h"
unsigned char display_flag = 0;                                         //  显示标志位
unsigned char databuf[20] = {0};                                        //  数据缓冲区
unsigned char flag = 0;

/**********************************************************************************************************
函数名称：校验数据包
输入参数：数据包长度和指针
输出参数：无
函数返回：0     校验通过
          1     指针为空
          2     校验和出错  
**********************************************************************************************************/
unsigned char CheckPacket(u8 *ptr, u16 len)
{    
	unsigned char i = 0;
    unsigned char CheckSum = 0;     

    //  指针检查
    if (ptr == NULL)
    {
        return 1;
    }
    
    // 计算校验和
    for (i = 0; i < len-1; i++)
	{
		CheckSum  += ptr[i];
	}
    
    // 校验和不等
	if ( CheckSum !=  ptr[len - 1])
	{
		return 2; 
	}
	
	return 0;
}

/******************************************************************************************
函数名称：void UltrasonicRanging(void)
输入参数：无
输出参数：无
函数返回：距离
*******************************************************************************************/
void MPU6050ServerLoop(void)
{  
//    char buf[200] = "";
    
    float angle[3],T;
    
    if (display_flag == 1)
    {
		display_flag = 0;
        
        if (CheckPacket((u8*)databuf, 11) == 0)
        {
            if (flag == 1)
            {
                flag = 0;
                
                angle[0] = ((short)(databuf[3]<<8| databuf[2]))/32768.0*180;   	//  Roll    滚转角（x 轴）
                angle[1] = ((short)(databuf[5]<<8| databuf[4]))/32768.0*180;   	//  Pitch   俯仰角（y 轴）
                angle[2] = ((short)(databuf[7]<<8| databuf[6]))/32768.0*180;   	//  Yaw     偏航角（z 轴）
                T        = ((short)(databuf[9]<<8| databuf[8]))/340.0+36.25;   	//  Temp    温度
                
                printf("X轴角度:%.2f, Y轴角度:%.2f, Z轴角度:%.2f\r\n", angle[0], angle[1], angle[2]);
                printf("温度:%.2f\r\n", T);
            }
        }
    }       
}
