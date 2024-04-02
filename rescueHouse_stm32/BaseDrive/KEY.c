#include "stm32f4xx.h"
#include "KEY.h"
#include "delay.h"

/******************************************************************************************
*�������ƣ�void KEYGpio_Init(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����KEY��ʼ��
//KEY_S1      PB2
//KEY_S2      PA0
//KEY_S3      PC2
//KEY_S4      PC0
//KEY_S5      PE4
*******************************************************************************************/
void KEYGpio_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    //  KEY_S1  	PF8
	//  KEY_S2      PF9
	//  KEY_S3      PF10
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}


/**********************************************************************************************************
�������ƣ�����ɨ�躯��
�����������
�����������
�������أ�����ֵ
**********************************************************************************************************/
unsigned char KeyScan(void)
{		 
    unsigned char buf[4] = {0};
    unsigned char temp = 0;
    
    buf[0] = KEY_S1_READ();
    buf[1] = KEY_S2_READ();
	buf[2] = KEY_S3_READ();
    
    if(buf[0] == 0 || buf[1] == 0 || buf[2] == 0)
    {
        delay_ms(100);
    
        buf[0] = KEY_S1_READ();
		buf[1] = KEY_S2_READ();
		buf[2] = KEY_S3_READ();
        
        //  KEY_S1
        if ( (buf[0] == 0) && (buf[1] == 1) && (buf[2] == 1))
        {
            temp = 1;
        }
        
        //  KEY_S2
        if ( (buf[0] == 1) && (buf[1] == 0) && (buf[2] == 1))
        {
            temp = 2;
        }
		
		//  KEY_S3
        if ( (buf[0] == 1) && (buf[1] == 1) && (buf[2] == 0))
        {
            temp = 3;
        }
    }
    
    return temp;
}
//unsigned char KeyScan(void)
//{		 
//    unsigned char buf[4] = {0};
//    unsigned char temp = 0;
//    static u8 key_up=1;
//    
//    buf[0] = KEY_S1_READ();
//    buf[1] = KEY_S2_READ();
//	buf[2] = KEY_S3_READ();
//    
//    if(key_up && (buf[0] == 0 || buf[1] == 0 || buf[2] == 0))
//    {
//        key_up = 0;
//        
//        delay_ms(100);
//    
//        buf[0] = KEY_S1_READ();
//		buf[1] = KEY_S2_READ();
//		buf[2] = KEY_S3_READ();
//        
//        //  KEY_S1
//        if ( (buf[0] == 0) && (buf[1] == 1) && (buf[2] == 1))
//        {
//            temp = 1;
//        }
//        
//        //  KEY_S2
//        if ( (buf[0] == 1) && (buf[1] == 0) && (buf[2] == 1))
//        {
//            temp = 2;
//        }
//		
//		//  KEY_S3
//        if ( (buf[0] == 1) && (buf[1] == 1) && (buf[2] == 0))
//        {
//            temp = 3;
//        }
//    }
//    else if (buf[0] == 1 && buf[1] == 1 && buf[2] == 1) key_up = 1;
//    
//    return temp;
//}

