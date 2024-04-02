#include "BH1750.h"
#include "delay.h"


/**********************************************************************************************************
�������ƣ�BH1750WriteData
���������������ַ�����ݵ�ַ
�����������
�������أ���
**********************************************************************************************************/
void BH1750WriteData(unsigned char DeviceAddr, unsigned char DataAddr)
{
    I2C_Start();                                //	��ʼ�ź�
    I2C_SendByte(DeviceAddr);                   //	�����豸��ַ+д�ź�
	  I2C_WaitACK();
    I2C_SendByte(DataAddr);                     //	�ڲ��Ĵ�����ַ��
	  I2C_WaitACK();
    I2C_Stop();                                 //	����ֹͣ�ź�
}

/**********************************************************************************************************
�������ƣ�BH1750_ReadData
���������������ַ�����ݵ�ַ
�����������
�������أ�����������
**********************************************************************************************************/
void BH1750_ReadData(unsigned char Dev_Address, unsigned char* ptr)
{   
    unsigned char i = 0;
    
	 I2C_Start();                      
	 I2C_SendByte(Dev_Address + 1);             //  ����������ַ+���ź�
	 I2C_WaitACK();
    
	 for (i = 0; i < 3; i++) 
	 {   
        ptr[i] = I2C_ReceiveByte();                                                                                                                                               
         
        if (i == 3) 
        {
            I2C_SendACK(1);                      //  ���һ��������Ҫ��NOACK
        }
        else 
        {
            I2C_SendACK(0);                      //  ��ӦACK
        }
	 }
	 I2C_Stop();                         
	 //delay_ms(5);
}


/**********************************************************************************************************
�������ƣ�BH1750WriteData
���������������ַ�����ݵ�ַ
�����������
�������أ���
**********************************************************************************************************/
void BH1750_Init(void)
{
    BH1750WriteData(BH1750_SlaveAddress, 0x01); //  power on
    BH1750WriteData(BH1750_SlaveAddress, 0x10); //  H- resolution mode
}