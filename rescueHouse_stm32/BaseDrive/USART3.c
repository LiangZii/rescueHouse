#include "stm32f4xx.h"
#include "USART3.h"
#include "stdio.h"

unsigned char Uart6ReceiveBuf[300] = {0};
unsigned char Uart5ReceiveBuf[300] = {0};

//  结构体定义
//USARTDATA   Uart6;
//USARTDATA   Uart5;

unsigned int   Uart5RXlenth;
unsigned char  Uart5Time;
unsigned char  Uart5ReceiveFinish;

unsigned int   Uart6RXlenth;
unsigned char  Uart6Time;
unsigned char  Uart6ReceiveFinish;

/**********************************************************************************************************
函数名称：UART3配置
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/
// USART3_TX	 PB10	//  out
// USART3_RX	 PB11	//  in
void UART3_Configuration(void)
{
    GPIO_InitTypeDef    GPIO_InitStructure;
	USART_InitTypeDef   USART_InitStructure;

	//  开启GPIO_D的时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
	//  开启串口3的时钟 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);


	USART_InitStructure.USART_BaudRate   = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART3, &USART_InitStructure);

	/* 使能串口3 */
	USART_Cmd(USART3, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}

/**********************************************************************************************************
函数名称：putchar函数重定义
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/
int fputc(int ch, FILE *f)
{
    USART3->SR;                                                         // 防止复位后无法打印首字符
    
    USART_SendData(USART3, (u8) ch);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
    {
        ; 
    }
    
    return (ch);
}

/**********************************************************************************************************
函数名称：打印接收到的数据
输入参数：数据指针、数据长度
输出参数：无
**********************************************************************************************************/
void PrintfReceivePacket(unsigned char *ptr , unsigned char n)
{
    u8 i;
    u16 uartbuf;
    
    for(i = 0 ; i < n ; i++)
    {
        uartbuf = ptr[i];
        printf("%02X ", uartbuf);
    }
    
    printf("\r\n");
}

/**********************************************************************************************************
函数名称：USART3发送数据函数
输入参数：发送数据首地址和数据长度
输出参数：无
**********************************************************************************************************/
void USART3_Senddata(unsigned char *Data, unsigned int length)
{
    while(length--)
    {
        USART_SendData(USART3,*Data++);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);
    }
}

/**********************************************************************************************************
函数名称：UART6配置
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/
// USART6_TX	 PC6	//  out
// USART6_RX	 PC7	//  in
void UART6_Configuration(unsigned int baud)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	USART_InitTypeDef   USART_InitStructure;
	NVIC_InitTypeDef    NVIC_InitStructure;
    
//    Uart6.ReceiveFinish = 0;
//    Uart6.RXlenth = 0;
//    Uart6.Time = 0;
//    Uart6.Rxbuf = Uart6ReceiveBuf;
    
    Uart6ReceiveFinish = 0;
    Uart6RXlenth = 0;
    Uart6Time = 0;

	//  开启GPIOA的时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
	//  开启串口1的时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);


	USART_InitStructure.USART_BaudRate   = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART6, &USART_InitStructure);

	/* 使能串口1 */
	USART_Cmd(USART6, ENABLE);
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);

    /* NVIC configuration */
    /* Configure the Priority Group to 2 bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable USART */
    USART_Cmd(USART6, ENABLE);
}
/**********************************************************************************************************
函数名称：USART6发送数据函数
输入参数：发送数据首地址和数据长度
输出参数：无
**********************************************************************************************************/
void USART6_Senddata(unsigned char *Data, unsigned int length)
{
    while(length--)
    {
        USART_SendData(USART6,*Data++);
        while (USART_GetFlagStatus(USART6, USART_FLAG_TC)==RESET);
    }
}

/**********************************************************************************************************
函数名称：UART1配置
输入参数：无
输出参数：无
函数返回：无
**********************************************************************************************************/
// USART6_TX	 PC12	//  out
// USART6_RX	 PD2	//  in
void UART5_Configuration(unsigned int baud)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	USART_InitTypeDef   USART_InitStructure;
	NVIC_InitTypeDef    NVIC_InitStructure;
    
    Uart5ReceiveFinish = 0;
    Uart5RXlenth = 0;
    Uart5Time = 0;
    
	//  开启GPIOA的时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOC, ENABLE);
    
	//  开启串口1的时钟 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);


	USART_InitStructure.USART_BaudRate   = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(UART5, &USART_InitStructure);

	/* 使能串口1 */
	USART_Cmd(UART5, ENABLE);
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

    /* NVIC configuration */
    /* Configure the Priority Group to 2 bits */
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable USART */
    USART_Cmd(UART5, ENABLE);
}

/**********************************************************************************************************
函数名称：USART6发送数据函数
输入参数：发送数据首地址和数据长度
输出参数：无
**********************************************************************************************************/
void UART5_Senddata(unsigned char *Data, unsigned int length)
{
    while(length--)
    {
        USART_SendData(UART5,*Data++);
        while (USART_GetFlagStatus(UART5, USART_FLAG_TC)==RESET);
    }
}

