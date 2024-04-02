#ifndef USART3_H
#define USART3_H

#define DEBUG               1

#define     SHELL_OUT(fmt, argc...)         {   if (DEBUG)                          \
                                                {                                   \
                                                    printf(fmt, ## argc);           \
                                                }                                   \
                                            }

#define     SHELL_DATAOUT(ptr, len)         {   if (DEBUG)                          \
                                                {                                   \
                                                    PrintfReceivePacket(ptr, len);  \
                                                }                                   \
                                            } 

//  用于USART接收数据包
struct UsartData                                                        
{		
	unsigned char *Rxbuf;
    unsigned int   RXlenth;
    unsigned char  Time;
    unsigned char  ReceiveFinish;
};
typedef  struct UsartData USARTDATA;
typedef  USARTDATA       *PUSARTDATA;

//extern USARTDATA   Uart6;
extern unsigned int     Uart6RXlenth;
extern unsigned char    Uart6Time;
extern unsigned char    Uart6ReceiveFinish;
extern unsigned char    Uart6ReceiveBuf[300];

//extern USARTDATA   Uart5;
extern unsigned int     Uart5RXlenth;
extern unsigned char    Uart5Time;
extern unsigned char    Uart5ReceiveFinish;
extern unsigned char    Uart5ReceiveBuf[300];

// 函数声明
void UART3_Configuration(void);
void USART3_Senddata(unsigned char *Data, unsigned int length);
void UART6_Configuration(unsigned int baud);
void USART6_Senddata(unsigned char *Data, unsigned int length);
void UART5_Configuration(unsigned int baud);
void UART5_Senddata(unsigned char *Data, unsigned int length);
void PrintfReceivePacket(unsigned char *ptr , unsigned char n);

#endif
