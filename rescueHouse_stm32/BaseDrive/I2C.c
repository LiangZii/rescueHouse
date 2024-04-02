#include "I2C.h"
#include "delay.h"

/**********************************************************************************************************
�������ƣ�I2C_GPIO_Configuration
�����������
�����������
�������أ���
���Ŷ��壺
//SCL	PA6
//SDA	PA3
**********************************************************************************************************/
void I2C_GPIO_Configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   //ʹ��GPIOBʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_3;    //LED��Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //ͨ�����ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);                  //��ʼ�� GPIO
   
    I2C_Stop();
}

/**********************************************************************************************************
�������ƣ�����SDAΪ����
�����������
�����������
�������أ���
���Ŷ��壺
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
�������ƣ�����SDAΪ���
�����������
�����������
�������أ���
���Ŷ��壺
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
�������ƣ�I2C_delay
�����������
�����������
�������أ���
**********************************************************************************************************/
void I2C_delay(void)
{
    delay_us(10);
}



/**********************************************************************************************************
�������ƣ�I2C_Start
�����������
�����������
�������أ���
**********************************************************************************************************/
void I2C_Start(void)
{
    I2C_SDA_Output();
    
    SDA_H;
    SCL_H;
    delay_us(4);
    
    SDA_L;
    delay_us(4);
 
    SCL_L;                    											//	����ʱ����
}


/**********************************************************************************************************
�������ƣ�I2C_Stop
�����������
�����������
�������أ���
**********************************************************************************************************/
void I2C_Stop(void)
{
    I2C_SDA_Output();
    
    SCL_L;
    SDA_L;                    											//	����������
    delay_us(4);
    
    SCL_H;                    											//	����ʱ����
    SDA_H;                   							 				//	����������
    delay_us(4);
}

/**********************************************************************************************************
�������ƣ��ȴ�ACK
�����������
�����������
�������أ�0     ����Ӧ��ɹ�
          1     ����Ӧ��ʧ��
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
�������ƣ�����ACKӦ��
�����������
�����������
�������أ���
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
�������ƣ�������ACKӦ��
�����������
�����������
�������أ���
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
�������ƣ�I2C_SendByte
������������͵�1�ֽ�����
�����������
�������أ���
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
	
    for (i = 0; i < 8; i++)     										//	8λ������
    {
        dat <<= 1;
        
        SCL_H;                											//	����ʱ����
        I2C_delay();             										//	��ʱ
        
        
		if (SDA_READ)
		{
			dat |= 1;             										//	������
		}
        
        SCL_L;                											//	����ʱ����
        I2C_delay();             										//	��ʱ
    }
    I2C_SDA_Output();
	
    return dat;
}
/**********************************************************************************************************
�������ƣ�I2C_SendACK
�����������
�����������
�������أ���
**********************************************************************************************************/
void I2C_SendACK(unsigned char ack)
{
    if (ack == 1)
	{
		SDA_H;
	}
    else
	{
        SDA_L;                  										//	дӦ���ź�
	}	
    
    SCL_H;                    											//	����ʱ����
    I2C_delay();                 										//	��ʱ
    
    SCL_L;                    											//	����ʱ����
    I2C_delay();                 										//	��ʱ
}
