/*-------------------------------------------------*/
/*                                                 */
/*              ����Wifi���ܵ�ͷ�ļ�               */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __WIFI_H
#define __WIFI_H

#include "USART3.h"	    //������Ҫ��ͷ�ļ�

#define RESET_IO(x)    GPIO_WriteBit(GPIOC, GPIO_Pin_0, (BitAction)x)  //PA4����WiFi�ĸ�λ

#define SSID   "nova666"                     //·����SSID����
#define PASS   "liliang123"                 //·��������

void WiFi_ResetIO_Init(void);
char WiFi_Reset(int timeout);
char WiFi_SendCmd(char *cmd,  int len, int timeout);
char WiFi_JoinAP(int timeout);
char WiFi_Connect_Server(int timeout);
char WiFi_Connect_IoTServer(void);


#endif


