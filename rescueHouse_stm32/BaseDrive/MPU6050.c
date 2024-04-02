#include "stm32f4xx.h"
#include "delay.h"
#include "USART3.h"
#include "MPU6050.h"
#include "stdio.h"
unsigned char display_flag = 0;                                         //  ��ʾ��־λ
unsigned char databuf[20] = {0};                                        //  ���ݻ�����
unsigned char flag = 0;

/**********************************************************************************************************
�������ƣ�У�����ݰ�
������������ݰ����Ⱥ�ָ��
�����������
�������أ�0     У��ͨ��
          1     ָ��Ϊ��
          2     У��ͳ���  
**********************************************************************************************************/
unsigned char CheckPacket(u8 *ptr, u16 len)
{    
	unsigned char i = 0;
    unsigned char CheckSum = 0;     

    //  ָ����
    if (ptr == NULL)
    {
        return 1;
    }
    
    // ����У���
    for (i = 0; i < len-1; i++)
	{
		CheckSum  += ptr[i];
	}
    
    // У��Ͳ���
	if ( CheckSum !=  ptr[len - 1])
	{
		return 2; 
	}
	
	return 0;
}

/******************************************************************************************
�������ƣ�void UltrasonicRanging(void)
�����������
�����������
�������أ�����
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
                
                angle[0] = ((short)(databuf[3]<<8| databuf[2]))/32768.0*180;   	//  Roll    ��ת�ǣ�x �ᣩ
                angle[1] = ((short)(databuf[5]<<8| databuf[4]))/32768.0*180;   	//  Pitch   �����ǣ�y �ᣩ
                angle[2] = ((short)(databuf[7]<<8| databuf[6]))/32768.0*180;   	//  Yaw     ƫ���ǣ�z �ᣩ
                T        = ((short)(databuf[9]<<8| databuf[8]))/340.0+36.25;   	//  Temp    �¶�
                
                printf("X��Ƕ�:%.2f, Y��Ƕ�:%.2f, Z��Ƕ�:%.2f\r\n", angle[0], angle[1], angle[2]);
                printf("�¶�:%.2f\r\n", T);
            }
        }
    }       
}
