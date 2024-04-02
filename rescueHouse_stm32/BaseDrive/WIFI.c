/*-------------------------------------------------*/
/*                                                 */
/*            ����602Wifi���ܵ�Դ�ļ�              */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f4xx.h"  //������Ҫ��ͷ�ļ�
#include "wifi.h"	    //������Ҫ��ͷ�ļ�
#include "delay.h"	    //������Ҫ��ͷ�ļ�
#include "LED.h"        //������Ҫ��ͷ�ļ�
#include "mqtt.h"       //������Ҫ��ͷ�ļ�
#include "lcd.h"
#include <string.h>
#include <stdio.h>

char wifi_mode = 0;     //����ģʽ 0��SSID������д�ڳ�����   1��Smartconfig��ʽ��APP����
	
/*-------------------------------------------------*/
/*����������ʼ��WiFi�ĸ�λIO                       */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void WiFi_ResetIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    //  LED
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);   //ʹ��GPIOBʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;               //LED��Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //ͨ�����ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //������������
    GPIO_Init(GPIOC, &GPIO_InitStructure);                  //��ʼ�� GPIO
    
	RESET_IO(1);                                              //��λIO���ߵ�ƽ
}

/*-------------------------------------------------*/
/*��������Air724��λ                                 */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_Reset(int timeout)
{
    Uart5RXlenth = 0;
    Uart5ReceiveFinish = 0;
    
    RESET_IO(0);                                    //��λIO���͵�ƽ
	delay_ms(500);                                  //��ʱ500ms
	RESET_IO(1);                                    //��λIO���ߵ�ƽ	

	while(timeout--){                               //�ȴ���ʱʱ�䵽0
		delay_ms(100);                              //��ʱ100ms
        
        if(Uart5ReceiveFinish == 1)
        {
            Uart5RXlenth = 0;
            Uart5ReceiveFinish = 0;
            if(strstr((void*)Uart5ReceiveBuf,"ready"))  //������յ�ready��ʾ��λ�ɹ�
                break;                              //��������whileѭ��
        }
		printf("%d ",timeout);                      //����������ڵĳ�ʱʱ��
	}
	printf("\r\n");                                 //���������Ϣ
    
	if(timeout<=0)return 1;                         //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�ready������1
	else return 0;                                  //��֮����ʾ��ȷ��˵���յ�ready��ͨ��break��������while
}


/*-------------------------------------------------*/
/*��������WiFi��������ָ��                         */
/*��  ����cmd��ָ��                                */
/*��  ����timeout����ʱʱ�䣨100ms�ı�����         */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_SendCmd(char *cmd,  int len, int timeout)
{
	Uart5RXlenth = 0;
    Uart5ReceiveFinish = 0;
	UART5_Senddata((void*)cmd, len);
	while(timeout--){                           //�ȴ���ʱʱ�䵽0
		delay_ms(100);                          //��ʱ100ms
		if(strstr((void*)Uart5ReceiveBuf,"OK"))     //������յ�OK��ʾָ��ɹ�
			break;       						//��������whileѭ��
		printf("%d ",timeout);                  //����������ڵĳ�ʱʱ��
	}
	printf("\r\n");                             //���������Ϣ
	if(timeout<=0)return 1;                     //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�OK������1
	else return 0;		         				//��֮����ʾ��ȷ��˵���յ�OK��ͨ��break��������while
}

/*-------------------------------------------------*/
/*��������WiFi����·����ָ��                       */
/*��  ����timeout����ʱʱ�䣨1s�ı�����            */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_JoinAP(int timeout)
{
    char temp[100] = "";
    sprintf(temp,"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PASS);
    
	Uart5RXlenth = 0;
    Uart5ReceiveFinish = 0;
    UART5_Senddata((void*)temp, strlen(temp));
    
	while(timeout--){                               //�ȴ���ʱʱ�䵽0
		delay_ms(1000);                             //��ʱ1s
//		if(strstr((void*)Uart5ReceiveBuf ,"WIFI GOT IPOK")) //������յ�WIFI GOT IP��ʾ�ɹ�
		if(strstr((void*)Uart5ReceiveBuf,"\n\r\nOK"))   //���ӳɹ�
			break;       						    //��������whileѭ��
		printf("%d ",timeout);                      //����������ڵĳ�ʱʱ��
	}
	printf("\r\n");                                 //���������Ϣ
	if(timeout<=0)return 1;                         //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�WIFI GOT IP������1
	return 0;                                       //��ȷ������0
}
/*-------------------------------------------------*/
/*������������TCP��������������͸��ģʽ            */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ��0����ȷ  ����������                      */
/*-------------------------------------------------*/
char WiFi_Connect_Server(int timeout)
{	
    char buf[100] = "";
    
    sprintf(buf, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ServerIP, ServerPort);
    
    Uart5RXlenth = 0;
    Uart5ReceiveFinish = 0;
    UART5_Senddata((void*)buf, strlen(buf));
    
	while(timeout--){                               //�ȴ���ʱ���
		delay_ms(100);                              //��ʱ100ms	
		if(strstr((void*)Uart5ReceiveBuf ,"CONNECT"))   //������ܵ�CONNECT��ʾ���ӳɹ�
			break;                                  //����whileѭ��
		if(strstr((void*)Uart5ReceiveBuf ,"CLOSED"))    //������ܵ�CLOSED��ʾ������δ����
			return 1;                               //������δ��������1
		if(strstr((void*)Uart5ReceiveBuf ,"ALREADY CONNECTED"))//������ܵ�ALREADY CONNECTED�Ѿ���������
			return 2;                               //�Ѿ��������ӷ���2
		printf("%d ",timeout);                   //����������ڵĳ�ʱʱ��  
	}
	printf("\r\n");                        //���������Ϣ
	if(timeout<=0)return 3;                   //��ʱ���󣬷���3
	else                                      //���ӳɹ���׼������͸��
	{
		printf("���ӷ������ɹ���׼������͸��\r\n");  //������ʾ��Ϣ
		Uart5RXlenth = 0;
        Uart5ReceiveFinish = 0;
        UART5_Senddata("AT+CIPSEND\r\n", strlen("AT+CIPSEND\r\n")); 
		while(timeout--){                               //�ȴ���ʱ���
			delay_ms(100);                              //��ʱ100ms	
			if(strstr((void*)Uart5ReceiveBuf,"\r\nOK\r\n\r\n>"))    //���������ʾ����͸���ɹ�
				break;                          //����whileѭ��
			printf("%d ",timeout);           //����������ڵĳ�ʱʱ��  
		}
		if(timeout<=0)return 4;                 //͸����ʱ���󣬷���4	
	}
	return 0;	                                //�ɹ�����0	
}

/*-------------------------------------------------*/
/*��������WiFi���ӷ�����                           */
/*��  ������                                       */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char WiFi_Connect_IoTServer(void)
{	
	printf("׼����λģ��\r\n");                     //������ʾ����
    LCD_ShowString(30, 50, "׼����λģ��...          ", BLACK, WHITE);
	if(WiFi_Reset(50)){                                //��λ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		printf("��λʧ�ܣ�׼������\r\n");           //���ط�0ֵ������if��������ʾ����
        LCD_ShowString(30, 50, "��λʧ�ܣ�׼������     ", BLACK, WHITE);
		return 1;                                      //����1
	}
    else {
        printf("��λ�ɹ�\r\n");                   //������ʾ����
        LCD_ShowString(30, 50, "��λ�ɹ�              ", BLACK, WHITE);
    }
	
	printf("׼������STAģʽ\r\n");                  //������ʾ����
    LCD_ShowString(30, 50, "׼������STAģʽ...       ", BLACK, WHITE);
	if(WiFi_SendCmd("AT+CWMODE=1\r\n",strlen("AT+CIPMODE=1\r\n"),50)){  //����STAģʽ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		printf("����STAģʽʧ�ܣ�׼������\r\n");    //���ط�0ֵ������if��������ʾ����
        LCD_ShowString(30, 50, "����STAģʽʧ�ܣ�׼������  ", BLACK, WHITE);
		return 2;                                      //����2
	}else {
        printf("����STAģʽ�ɹ�\r\n");            //������ʾ����
        LCD_ShowString(30, 50, "����STAģʽ�ɹ�         ", BLACK, WHITE);
    }
	
	if(wifi_mode==0){                                      //�������ģʽ=0��SSID������д�ڳ����� 
		printf("׼��ȡ���Զ�����\r\n");                 //������ʾ����
        LCD_ShowString(30, 50, "׼��ȡ���Զ�����...       ", BLACK, WHITE);
		if(WiFi_SendCmd("AT+CWAUTOCONN=0\r\n",strlen("AT+CWAUTOCONN=0\r\n"),50)){  //ȡ���Զ����ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
			printf("ȡ���Զ�����ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
            LCD_ShowString(30, 50, "ȡ���Զ�����ʧ�ܣ�׼������ ", BLACK, WHITE);
			return 3;                                      //����3
		}else {
            printf("ȡ���Զ����ӳɹ�\r\n");           //������ʾ����
            LCD_ShowString(30, 50, "ȡ���Զ����ӳɹ�       ", BLACK, WHITE);
        }

		printf("׼������·����\r\n");                   //������ʾ����
        LCD_ShowString(30, 50, "׼������·����...       ", BLACK, WHITE);        
		if(WiFi_JoinAP(30)){                               //����·����,1s��ʱ��λ���ܼ�30s��ʱʱ��
			printf("����·����ʧ�ܣ�׼������\r\n");     //���ط�0ֵ������if��������ʾ����
            LCD_ShowString(30, 50, "����·����ʧ�ܣ�׼������ ", BLACK, WHITE);
			return 4;                                      //����4	
		}else {
            printf("����·�����ɹ�\r\n");             //������ʾ����		
            LCD_ShowString(30, 50, "����·�����ɹ�      ", BLACK, WHITE);
        }            
	}
	
	printf("׼������͸��\r\n");                     //������ʾ����
    LCD_ShowString(30, 50, "׼������͸��...      ", BLACK, WHITE);
	if(WiFi_SendCmd("AT+CIPMODE=1\r\n",strlen("AT+CIPMODE=1\r\n"),50)){ //����͸����100ms��ʱ��λ���ܼ�5s��ʱʱ��
		printf("����͸��ʧ�ܣ�׼������\r\n");       //���ط�0ֵ������if��������ʾ����
        LCD_ShowString(30, 50, "����͸��ʧ�ܣ�׼������ ", BLACK, WHITE);
		return 8;                                      //����8
	}else {
        printf("����͸���ɹ�\r\n");               //������ʾ����
        LCD_ShowString(30, 50, "����͸���ɹ�      ", BLACK, WHITE);
    }
	
	printf("׼���رն�·����\r\n");                 //������ʾ����
    LCD_ShowString(30, 50, "׼���رն�·����...  ", BLACK, WHITE);
	if(WiFi_SendCmd("AT+CIPMUX=0\r\n",strlen("AT+CIPMUX=0\r\n"),50)){   //�رն�·���ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
		printf("�رն�·����ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
        LCD_ShowString(30, 50, "�رն�·����ʧ�ܣ�׼������", BLACK, WHITE);
		return 9;                                      //����9
	}else {
        printf("�رն�·���ӳɹ�\r\n");           //������ʾ����
        LCD_ShowString(30, 50, "�رն�·���ӳɹ�    ", BLACK, WHITE);
    }
	
	printf("׼�����ӷ�����\r\n");                   //������ʾ����
    LCD_ShowString(30, 50, "׼�����ӷ�����...    ", BLACK, WHITE);
	if(WiFi_Connect_Server(100)){                      //���ӷ�������100ms��ʱ��λ���ܼ�10s��ʱʱ��
		printf("���ӷ�����ʧ�ܣ�׼������\r\n");     //���ط�0ֵ������if��������ʾ����
        LCD_ShowString(30, 50, "���ӷ�����ʧ�ܣ�׼������", BLACK, WHITE);
		return 10;                                     //����10
	}else {
        printf("���ӷ������ɹ�\r\n");             //������ʾ����	
        LCD_ShowString(30, 50, "���ӷ������ɹ�    ", BLACK, WHITE);
    }
	
	return 0;                                          //��ȷ����0
	
}
