/**-------------------------------------------------------------------------------------------
** Created by:          qinyx
** Last modified Date:  2014-02-28
** Last Version:        V1.00
** Descriptions:        STM32F407嵌入式实验箱
**	  Gpio配置文件
**
**-------------------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "BEEP.h"
#include "delay.h"

/*************************************************************************
*函数名称：void BEEPGpio_Init(void)
*
*入口参数：无
*
*出口参数：无
*
*功能说明：BEEP初始化
//BEEP      PC13
*************************************************************************/
void BEEPGpio_Init(void)
{    
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    //  BEEP
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);   //使能GPIOB时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				//BEEP对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //通用输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //输出推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //无上拉或下拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);                  //初始化 GPIO
	
		BEEP_OFF;
}
