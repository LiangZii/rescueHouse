/****************************************Copyright (c)***************************************
**-------------------------------------------------------------------------------------------
** Created by:          qinyx
** Last modified Date:  2014-02-28
** Last Version:        V1.00
** Descriptions:        STM32F407Ƕ��ʽʵ����
**-------------------------------------------------------------------------------------------*/
//********************************************************************************************/
#include "stm32f4xx.h"
#include "USART3.h"
#include "delay.h"
#include "stdio.h"
#include "KEY.h"
#include "LED.h"
#include "WIFI.h"
#include "mqtt.h"
#include "lcd.h"
#include "BH1750.h"
#include "I2C.h"
#include "string.h"
#include "BEEP.h"



#define ILLU_MIN 200
#define ILLU_MAX 500

#define MANU    "MANU"
#define AUTO    "AUTO"
#define TIME    "TIME"

unsigned char timer_flag = 0;
unsigned char ping_cnt = 0;
void warn_State(unsigned char sta);
void sys_State(void);       //�����������ж�ϵͳ״̬����������������
unsigned char lamp_mode[4] = MANU;
unsigned int illuMin = ILLU_MIN;
unsigned int illuMax = ILLU_MAX;

int main(void)
{
		char *stringTemp;
		unsigned char keyValue;
		char disbuf[50] = "";
		
		SysTick_Init();                 //  ϵͳ�δ�ʱ����ʼ��
    LEDGpio_Init();									//��
    BEEPGpio_Init();								//������
    KEYGpio_Init(); 								//��������
    I2C_GPIO_Configuration();       //  ����I2C��GPIO   ��
	  BH1750_Init();                  //  ��ʼ��BH1750   ���մ�����
    UART3_Configuration();          //  USART3����
    //UART6_Configuration(9600);      //  PM2.5
    UART5_Configuration(115200);    //  WIFI����


		LCD_Init();                     //  Һ����ʼ��
    LCD_Clear(WHITE);               //  ����Һ������
	  LCD_ShowString(250, 10, "���ܾ�Ԯ�ֹ���ϵͳ-��Ԯ��1", BLACK, WHITE);
    sprintf(disbuf,"SSID:%s   PASS:%s",SSID,PASS);
    LCD_ShowString(30, 30, (u8 *)disbuf, BLACK, WHITE);
    

		WiFi_ResetIO_Init();            //  ��ʼ��WiFi�ĸ�λIO
    AliIoT_Parameter_Init();	    //  ��ʼ������EMQX������MQTT�������Ĳ���


		while (1)
    {
       
        if(Connect_flag == 1)
        {
            if (SubcribePack_flag == 1)
            {
                keyValue = KeyScan();
                if (keyValue == 1)
                {
                    BEEP_ON;
										warn_State(1);    //�ֶ�����
                }
                else
                {
                    BEEP_OFF;
                }
                if (timer_flag == 1)
                {
                    timer_flag = 0;
                    sys_State();
                }
            }
            /*-------------------------------------------------------------*/
            /*                     �����ͻ���������                      */
            /*-------------------------------------------------------------*/
            if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr)                 //if�����Ļ���˵�����ͻ�������������
            {
                //3������ɽ���if
                //��1�֣�0x10 ���ӱ���
                //��2�֣�0x82 ���ı��ģ���ConnectPack_flag��λ����ʾ���ӱ��ĳɹ�
                //��3�֣�SubcribePack_flag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
                if((MQTT_TxDataOutPtr[2] == 0x10) || ((MQTT_TxDataOutPtr[2] == 0x82) && (ConnectPack_flag == 1)) || (SubcribePack_flag == 1))
                {
                    printf("��������:0x%x\r\n", MQTT_TxDataOutPtr[2]);  //������ʾ��Ϣ
                    UART5_Senddata(&MQTT_TxDataOutPtr[2], (MQTT_TxDataOutPtr[0]*256 + MQTT_TxDataOutPtr[1]));
                    MQTT_TxDataOutPtr += TBUFF_UNIT;                    //ָ������
                    if(MQTT_TxDataOutPtr == MQTT_TxDataEndPtr)          //���ָ�뵽������β����
                        MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];          //ָ���λ����������ͷ
                }
            }//�����ͻ��������ݵ�else if��֧��β
            
            /*-------------------------------------------------------------*/
            /*                     ������ջ���������                      */
            /*-------------------------------------------------------------*/
            if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr)   //if�����Ļ���˵�����ջ�������������
            {
                printf("���յ�����:");
                /*-----------------------------------------------------*/
                /*                    ����CONNACK����                  */
                /*-----------------------------------------------------*/
                //if�жϣ������һ���ֽ���0x20����ʾ�յ�����CONNACK����
                //��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
                if(MQTT_RxDataOutPtr[2] == 0x20)
                {
                    switch(MQTT_RxDataOutPtr[5])
                    {
                    case 0x00 :
                        printf("CONNECT���ĳɹ�\r\n");                      //���������Ϣ
                        LCD_ShowString(30, 50, "CONNECT�ɹ�          ", BLACK, WHITE);
                        ConnectPack_flag = 1;                               //CONNECT���ĳɹ������ı��Ŀɷ�
                        break;                                              //������֧case 0x00
                    case 0x01 :
                        printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");  //���������Ϣ
                        Connect_flag = 0;                                   //Connect_flag���㣬��������
                        break;                                              //������֧case 0x01
                    case 0x02 :
                        printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n"); //���������Ϣ
                        Connect_flag = 0;                                   //Connect_flag���㣬��������
                        break;                                              //������֧case 0x02
                    case 0x03 :
                        printf("�����Ѿܾ�������˲����ã�׼������\r\n");    //���������Ϣ
                        Connect_flag = 0;                                   //Connect_flag���㣬��������
                        break;                                              //������֧case 0x03
                    case 0x04 :
                        printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n"); //���������Ϣ
                        Connect_flag = 0;                                   //Connect_flag���㣬��������
                        break;                                              //������֧case 0x04
                    case 0x05 :
                        printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");           //���������Ϣ
                        Connect_flag = 0;                                   //Connect_flag���㣬��������
                        break;                                              //������֧case 0x05
                    default   :
                        printf("�����Ѿܾ���δ֪״̬��׼������\r\n");          //���������Ϣ
                        Connect_flag = 0;                                   //Connect_flag���㣬��������
                        break;                                              //������֧case default
                    }
                }
                //if�жϣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
                //��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
                else if(MQTT_RxDataOutPtr[2] == 0x90)
                {
                    switch(MQTT_RxDataOutPtr[6])
                    {
                    case 0x00 :
                    case 0x01 :
                        printf("���ĳɹ�\r\n");                             //���������Ϣ
                        LCD_ShowString(30, 50, "���ĳɹ�              ", BLACK, WHITE);
                        SubcribePack_flag = 1;                              //SubcribePack_flag��1����ʾ���ı��ĳɹ����������Ŀɷ���
                        Ping_flag = 0;                                      //Ping_flag����
                        ping_cnt = 30;
                        break;                                              //������֧
                    default   :
                        printf("����ʧ�ܣ�׼������\r\n");                    //���������Ϣ
                        
                        Connect_flag = 0;                                   //Connect_flag���㣬��������
                        break;                                              //������֧
                    }
                }
                //if�жϣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
                else if(MQTT_RxDataOutPtr[2] == 0xD0)
                {
                    printf("PING���Ļظ�\r\n");                             //���������Ϣ
                    Ping_flag = 0;                                      //Ҫ���Ping_flag��־
                    ping_cnt = 30;
                }
                //if�жϣ������һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
                //����Ҫ��ȡ��������
                else if((MQTT_RxDataOutPtr[2] == 0x30))
                {
                    printf("�������ȼ�0����\r\n");                             //���������Ϣ
                    MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);               //����ȼ�0��������
                }

                MQTT_RxDataOutPtr += RBUFF_UNIT;                            //ָ������
                if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)                  //���ָ�뵽������β����
                    MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];                  //ָ���λ����������ͷ
            }//������ջ��������ݵ�else if��֧��β
            
            /*-------------------------------------------------------------*/
            /*                     ���������������                      */
            /*-------------------------------------------------------------*/
            if(MQTT_CMDOutPtr != MQTT_CMDInPtr)                             //if�����Ļ���˵�����������������
            {
                printf("����:%s\r\n", &MQTT_CMDOutPtr[2]);                                    //���������Ϣ
                
								if(strstr((char *)MQTT_CMDOutPtr+2,"\"led2switch\": 1")){ //���������"params":{"PowerSwitch_2":1}˵���������·��򿪿���2	
										LED2_ON;
								}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"led2switch\": 0")){ //���������"params":{"PowerSwitch_2":0}˵���������·��رտ���2
										LED2_OFF;
								}
								
								
                MQTT_CMDOutPtr += CBUFF_UNIT;                               //ָ������
                if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)                        //���ָ�뵽������β����
                    MQTT_CMDOutPtr = MQTT_CMDBuf[0];                        //ָ���λ����������ͷ
            }//��������������ݵ�else if��֧��β
            
        }//Connect_flag=1��if��֧�Ľ�β
        
        else
        {
            printf("��Ҫ���ӷ�����\r\n");          //���������Ϣ
            if(WiFi_Connect_IoTServer() == 0)       //���Air724�����Ʒ�������������0����ʾ��ȷ������if
            {
                printf("����TCP���ӳɹ�\r\n");        //���������Ϣ
                Connect_flag = 1;                   //Connect_flag��1����ʾ���ӳɹ�
                Uart5RXlenth = 0;                  //Air724������������������
                Uart5ReceiveFinish = 0;
                MQTT_Buff_Init();                   //��ʼ�����ͻ�����
            }
        }
    }



}


/*---------------------------------------------------*/
/*���������жϱ���״̬����������������                 */
/*��  ����sta       0.�ƹ���ֵ����    1.�ֶ�����      */
/*����ֵ����                                         */
/*--------------------------------------------------*/
void warn_State(unsigned char sta)
{
		char temp[TBUFF_UNIT];                   //����һ����ʱ������
		
		if (sta == 0)
    {
        LCD_ShowString(200, 290, "�ƹ���ֵ����", BLACK, WHITE);
				sprintf(temp,"{\"warnId\": 0}");  //��Ҫ�ظ�״̬��������
    }
    else if(sta == 1)
    {
        LCD_ShowString(200, 290, "�ֶ�����", BLACK, WHITE);
				sprintf(temp,"{\"warnId\": 1}");  //��Ҫ�ظ�״̬��������			
    }
		
		
    MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������		
}

/*-------------------------------------------------*/
/*���������жϲ��ռ��ɼ���ģ��״̬����������������                   */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void sys_State(void)
{
	  char disbuf[50] = "";					//LCD����ʾ����
		char temp[TBUFF_UNIT];  			//MQTT����
	
		unsigned int light_temp = 0;  //����ǿ�Ȼ���
		unsigned int light_date = 0;
		unsigned char Databuf[3] = {0};
	
		BH1750_ReadData(BH1750_SlaveAddress, Databuf);
        
		light_temp = Databuf[0];
		light_temp = (light_temp << 8) + Databuf[1];
		light_date = light_temp*10/12;
		
		if(light_date<10)
    {
				LED1_ON;			//����
				warn_State(0);    //����
		}
		else
		{
				LED1_OFF;
		}
		
		
		//��ӡ�������
		sprintf(disbuf,"����ǿ��:%3dug/m3",light_date);
    LCD_ShowString(200, 80, (u8 *)disbuf, BLACK, WHITE);
    
    sprintf(temp,"{\"light\": %d}",light_date);  //��Ҫ�ظ�״̬��������
    
		MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������	
}

