/**
  ******************************************************************************
  * @file    EXTI/EXTI_Example/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    13-April-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "USART3.h"
#include "delay.h"
#include "mqtt.h"
#include <string.h>
#include "LED.h"

extern unsigned char timer_flag;
extern unsigned char ping_cnt;
extern unsigned int radarTime;
extern unsigned char energyMode;

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup EXTI_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    static unsigned int cnt = 0;
    
    if(Connect_flag == 1)
    {
        cnt++;
        
        if (cnt%1000 == 0)                                  //  1s定时
        {
            if (ping_cnt > 0)
            {
                ping_cnt--;
                if (ping_cnt == 0)
                {
                    switch(Ping_flag)                       //  判断Ping_flag的状态
                    {
                    case 0:                                 //  如果Ping_flag等于0，表示正常状态，发送Ping报文
                        MQTT_PingREQ();                     //  添加Ping报文到发送缓冲区
                        break;
                    case 1:                                 //  如果Ping_flag等于1，说明上一次发送到的ping报文，没有收到服务器回复，所以1没有被清除为0，可能是连接异常，我们要启动快速ping模式
                        MQTT_PingREQ();                     //  添加Ping报文到发送缓冲区
                        break;
                    case 3:                                 //  如果Ping_flag等于2，说明还没有收到服务器回复
                        MQTT_PingREQ();                     //  添加Ping报文到发送缓冲区
                        break;
                    case 4:                                 //  如果Ping_flag等于5，说明我们发送了多次ping，均无回复，应该是连接有问题，我们重启连接
                        Connect_flag = 0;                   //  连接状态置0，表示断开，没连上服务器
                        break;
                    }
                    ping_cnt=2;
                    Ping_flag++; 
                }
            }
        }
        
        if(cnt >= PUBLISH_TIME)                                     //定时发送温湿度值
        {
            cnt = 0;
            timer_flag = 1;
        }
    }
    
    if(Uart5Time > 0)
    {
        Uart5Time--;

        if(Uart5Time == 0)
        {
            if (Connect_flag == 0)
            {
                Uart5ReceiveFinish = 1;
            }
            else
            {
                memcpy(&MQTT_RxDataInPtr[2], Uart5ReceiveBuf, Uart5RXlenth);   //拷贝数据到接收缓冲区
                MQTT_RxDataInPtr[0] = Uart5RXlenth / 256;                  //记录数据长度高字节
                MQTT_RxDataInPtr[1] = Uart5RXlenth % 256;                  //记录数据长度低字节
                MQTT_RxDataInPtr += RBUFF_UNIT;                             //指针下移
                if(MQTT_RxDataInPtr == MQTT_RxDataEndPtr)                   //如果指针到缓冲区尾部了
                {
                    MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                   //指针归位到缓冲区开头
                }
                Uart5RXlenth = 0;                                          //串口2接收数据量变量清零
            }
        }
    }
    
    
    //  UART6接收
    if (Uart6Time > 0)
    {
        Uart6Time--;

        if (Uart6Time == 0)
        {
            Uart6ReceiveFinish = 1;
        }
    }
    
    TimingDelay_Decrement();
}

//  UART5串口中断程序
void UART5_IRQHandler(void)
{   
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)                //  若接收数据寄存器满
	{
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
        
        Uart5ReceiveBuf[Uart5RXlenth++] = USART_ReceiveData(UART5);
        
        Uart5Time = 5;
    }	
}

void USART6_IRQHandler(void)
{
	if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)                //  若接收数据寄存器满
	{
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
        Uart6ReceiveBuf[Uart6RXlenth] = USART_ReceiveData(USART6);
        Uart6RXlenth++;
        Uart6Time = 5;
    }
}

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        //Int_flag0 = 1;
        LED1_REVERSE;
        
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        //Int_flag1 = 1;
        LED2_REVERSE;
        
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

//Usart3串口中断程序USART3_IRQHandler **************************************************************************

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
