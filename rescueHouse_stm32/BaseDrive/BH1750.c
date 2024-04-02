#include "BH1750.h"
#include "delay.h"


/**********************************************************************************************************
函数名称：BH1750WriteData
输入参数：器件地址、数据地址
输出参数：无
函数返回：无
**********************************************************************************************************/
void BH1750WriteData(unsigned char DeviceAddr, unsigned char DataAddr)
{
    I2C_Start();                                //	起始信号
    I2C_SendByte(DeviceAddr);                   //	发送设备地址+写信号
	  I2C_WaitACK();
    I2C_SendByte(DataAddr);                     //	内部寄存器地址，
	  I2C_WaitACK();
    I2C_Stop();                                 //	发送停止信号
}

/**********************************************************************************************************
函数名称：BH1750_ReadData
输入参数：器件地址、数据地址
输出参数：无
函数返回：读到的数据
**********************************************************************************************************/
void BH1750_ReadData(unsigned char Dev_Address, unsigned char* ptr)
{   
    unsigned char i = 0;
    
	 I2C_Start();                      
	 I2C_SendByte(Dev_Address + 1);             //  发送器件地址+读信号
	 I2C_WaitACK();
    
	 for (i = 0; i < 3; i++) 
	 {   
        ptr[i] = I2C_ReceiveByte();                                                                                                                                               
         
        if (i == 3) 
        {
            I2C_SendACK(1);                      //  最后一个数据需要回NOACK
        }
        else 
        {
            I2C_SendACK(0);                      //  回应ACK
        }
	 }
	 I2C_Stop();                         
	 //delay_ms(5);
}


/**********************************************************************************************************
函数名称：BH1750WriteData
输入参数：器件地址、数据地址
输出参数：无
函数返回：无
**********************************************************************************************************/
void BH1750_Init(void)
{
    BH1750WriteData(BH1750_SlaveAddress, 0x01); //  power on
    BH1750WriteData(BH1750_SlaveAddress, 0x10); //  H- resolution mode
}
