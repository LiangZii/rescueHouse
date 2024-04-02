/**-------------------------------------------------------------------------------------------
** Created by:          qinyx
** Last modified Date:  2014-02-28
** Last Version:        V1.00
** Descriptions:        STM32F407Ƕ��ʽʵ����
**	  Gpio�����ļ�
**
**-------------------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "BEEP.h"
#include "delay.h"

/*************************************************************************
*�������ƣ�void BEEPGpio_Init(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����BEEP��ʼ��
//BEEP      PC13
*************************************************************************/
void BEEPGpio_Init(void)
{    
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    //  BEEP
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);   //ʹ��GPIOBʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				//BEEP��Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //ͨ�����ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //������������
    GPIO_Init(GPIOC, &GPIO_InitStructure);                  //��ʼ�� GPIO
	
		BEEP_OFF;
}
