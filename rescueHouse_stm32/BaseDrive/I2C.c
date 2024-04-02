#include "I2C.h"
#include "delay.h"

/**********************************************************************************************************
函数名称：I2C_GPIO_Configuration
输入参数：无
输出参数：无
函数返回：无
引脚定义：
//SCL	PA6
//SDA	PA3
**********************************************************************************************************/
void I2C_GPIO_Configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   //使能GPIOB时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_3;    //LED对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //通用输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //输出推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);                  //初始化 GPIO
   
    I2C_Stop();
}

/**********************************************************************************************************
函数名称：配置SDA为输入
输入参数：无
输出参数：无
函数返回：无
引脚定义：
//SDA	PA3
**********************************************************************************************************/
void I2C_SDA_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**********************************************************************************************************
函数名称：配置SDA为输出
输入参数：无
输出参数：无
函数返回：无
引脚定义：
//SDA	PA3
**********************************************************************************************************/
void I2C_SDA_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/**********************************************************************************************************
函数名称：I2C_delay
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/
void I2C_delay(void)
{
    delay_us(10);
}



/**********************************************************************************************************
函数名称：I2C_Start
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/
void I2C_Start(void)
{
    I2C_SDA_Output();
    
    SDA_H;
    SCL_H;
    delay_us(4);
    
    SDA_L;
    delay_us(4);
 
    SCL_L;                    											//	拉低时钟线
}


/**********************************************************************************************************
函数名称：I2C_Stop
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/
void I2C_Stop(void)
{
    I2C_SDA_Output();
    
    SCL_L;
    SDA_L;                    											//	拉低数据线
    delay_us(4);
    
    SCL_H;                    											//	拉高时钟线
    SDA_H;                   							 				//	产生上升沿
    delay_us(4);
}

/**********************************************************************************************************
函数名称：等待ACK
输入参数：无
输出参数：无
函数返回：0     接收应答成功
          1     接收应答失败
**********************************************************************************************************/
unsigned char I2C_WaitACK(void)
{
    u8 ucErrTime = 0;
    
	I2C_SDA_Input();
	
    SDA_H;
    delay_us(1);	   
    
	SCL_H;
    delay_us(1);	 
    
	while(SDA_READ)
	{
		ucErrTime++;
        
		if(ucErrTime>250)
		{
			I2C_Stop();
            
			return 1;
		}
	}
    
	SCL_L;
	
    return 0;  
}

/**********************************************************************************************************
函数名称：产生ACK应答
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/
void I2C_ACK(void)
{
	SCL_L;
    
	I2C_SDA_Output();
    
	SDA_L;
	delay_us(2);
    
	SCL_H;
	delay_us(2);
    
	SCL_L;
}


/**********************************************************************************************************
函数名称：不产生ACK应答
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/
void I2C_NACK(void)
{
	SCL_L;
    
	I2C_SDA_Output();
    
	SDA_H;
	delay_us(2);
    
	SCL_H;
	delay_us(2);
	
    SCL_L;
}

/**********************************************************************************************************
函数名称：I2C_SendByte
输入参数：发送的1字节数据
输出参数：无
函数返回：无
**********************************************************************************************************/
void I2C_SendByte(unsigned char txd)
{
    u8 t;   
    
    I2C_SDA_Output();
	
    SCL_L;
    
    for(t = 0; t < 8; t++)
    {                      
        if( (txd&0x80) == 0x80)
        {
            SDA_H;
        }
        else
        {
            SDA_L;
        }
                
        txd <<= 1; 	  
        
		delay_us(2);
        SCL_H;
        
		delay_us(2); 
		SCL_L;	
        
		delay_us(2);
    }
}


unsigned char I2C_ReceiveByte(void)
{
    unsigned char i;
    unsigned char dat = 0;
    
	I2C_SDA_Input();
	
    for (i = 0; i < 8; i++)     										//	8位计数器
    {
        dat <<= 1;
        
        SCL_H;                											//	拉高时钟线
        I2C_delay();             										//	延时
        
        
		if (SDA_READ)
		{
			dat |= 1;             										//	读数据
		}
        
        SCL_L;                											//	拉低时钟线
        I2C_delay();             										//	延时
    }
    I2C_SDA_Output();
	
    return dat;
}
/**********************************************************************************************************
函数名称：I2C_SendACK
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/
void I2C_SendACK(unsigned char ack)
{
    if (ack == 1)
	{
		SDA_H;
	}
    else
	{
        SDA_L;                  										//	写应答信号
	}	
    
    SCL_H;                    											//	拉高时钟线
    I2C_delay();                 										//	延时
    
    SCL_L;                    											//	拉低时钟线
    I2C_delay();                 										//	延时
}
