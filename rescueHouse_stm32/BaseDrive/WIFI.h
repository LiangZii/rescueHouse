/*-------------------------------------------------*/
/*                                                 */
/*              操作Wifi功能的头文件               */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __WIFI_H
#define __WIFI_H

#include "USART3.h"	    //包含需要的头文件

#define RESET_IO(x)    GPIO_WriteBit(GPIOC, GPIO_Pin_0, (BitAction)x)  //PA4控制WiFi的复位

#define SSID   "nova666"                     //路由器SSID名称
#define PASS   "liliang123"                 //路由器密码

void WiFi_ResetIO_Init(void);
char WiFi_Reset(int timeout);
char WiFi_SendCmd(char *cmd,  int len, int timeout);
char WiFi_JoinAP(int timeout);
char WiFi_Connect_Server(int timeout);
char WiFi_Connect_IoTServer(void);


#endif


