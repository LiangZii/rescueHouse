/*-------------------------------------------------*/
/*                                                 */
/*            ʵ��MQTTЭ�鹦�ܵ�ͷ�ļ�             */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __MQTT_H
#define __MQTT_H

#define PUBLISH_TIME        5000    // 5S

#define  R_NUM              5       //���ջ���������
#define  RBUFF_UNIT         300     //���ջ���������

#define  T_NUM              5       //���ͻ���������  
#define  TBUFF_UNIT         500     //���ͻ���������

#define  C_NUM              8       //�����������
#define  CBUFF_UNIT         300     //�����������

#define  PRODUCTKEY           "a1yDhJ1PZZa"                                 //��ƷID
#define  PRODUCTKEY_LEN       strlen(PRODUCTKEY)                            //��ƷID����
#define  DEVICENAME           "rescue1"                                       //�豸��  
#define  DEVICENAME_LEN       strlen(DEVICENAME)                            //�豸������
#define  DEVICESECRE          "3e8aceeefa68d9462a8fbbb193f72561"            //�豸��Կ   
#define  DEVICESECRE_LEN      strlen(DEVICESECRE)                           //�豸��Կ����
#define  S_TOPIC_NAME         "subscribe_topic"  //��Ҫ���ĵ�����  
#define  P_TOPIC_NAME         "publish_topic"   //��Ҫ����������     

extern unsigned char  MQTT_RxDataBuf[R_NUM][RBUFF_UNIT];       //�ⲿ�������������ݵĽ��ջ�����,���з��������������ݣ�����ڸû�����,��������һ���ֽڴ�����ݳ���
extern unsigned char *MQTT_RxDataInPtr;                        //�ⲿ����������ָ�򻺳���������ݵ�λ��
extern unsigned char *MQTT_RxDataOutPtr;                       //�ⲿ����������ָ�򻺳�����ȡ���ݵ�λ��
extern unsigned char *MQTT_RxDataEndPtr;                       //�ⲿ����������ָ�򻺳���������λ��
extern unsigned char  MQTT_TxDataBuf[T_NUM][TBUFF_UNIT];       //�ⲿ�������������ݵķ��ͻ�����,���з��������������ݣ�����ڸû�����,��������һ���ֽڴ�����ݳ���
extern unsigned char *MQTT_TxDataInPtr;                        //�ⲿ����������ָ�򻺳���������ݵ�λ��
extern unsigned char *MQTT_TxDataOutPtr;                       //�ⲿ����������ָ�򻺳�����ȡ���ݵ�λ��
extern unsigned char *MQTT_TxDataEndPtr;                       //�ⲿ����������ָ�򻺳���������λ��
extern unsigned char  MQTT_CMDBuf[C_NUM][CBUFF_UNIT];          //�ⲿ�����������������ݵĽ��ջ�����
extern unsigned char *MQTT_CMDInPtr;                           //�ⲿ����������ָ�򻺳���������ݵ�λ��
extern unsigned char *MQTT_CMDOutPtr;                          //�ⲿ����������ָ�򻺳�����ȡ���ݵ�λ��
extern unsigned char *MQTT_CMDEndPtr;                          //�ⲿ����������ָ�򻺳���������λ��

extern char ClientID[128];     //�ⲿ������������ſͻ���ID�Ļ�����
extern int  ClientID_len;      //�ⲿ������������ſͻ���ID�ĳ���
extern char Username[128];     //�ⲿ��������������û����Ļ�����
extern int  Username_len;	   //�ⲿ��������������û����ĳ���
extern char Passward[128];     //�ⲿ�����������������Ļ�����
extern int  Passward_len;	   //�ⲿ�����������������ĳ���
extern char ServerIP[128];     //�ⲿ������������ŷ�����IP��������
extern int  ServerPort;        //�ⲿ������������ŷ������Ķ˿ں�

extern char Ping_flag;         //�ⲿ����������ping����״̬      0������״̬���ȴ���ʱʱ�䵽������Ping����
                               //�ⲿ����������ping����״̬      1��Ping�����ѷ��ͣ����յ� �������ظ����ĵĺ� ��1��Ϊ0
extern char Connect_flag;      //�ⲿ����������ͬ����������״̬  0����û�����ӷ�����  1�������Ϸ�������
extern char ReConnect_flag;    //�ⲿ��������������������״̬    0�����ӻ�����  1�����ӶϿ�������
extern char ConnectPack_flag;  //�ⲿ����������CONNECT����״̬   1��CONNECT���ĳɹ�
extern char SubcribePack_flag; //�ⲿ�������������ı���״̬      1�����ı��ĳɹ�

void MQTT_Buff_Init(void);
void AliIoT_Parameter_Init(void);
void MQTT_ConectPack(void);
void MQTT_Subscribe(char *, int);
void MQTT_PingREQ(void);
void MQTT_PublishQs0(char *, char *, int);
void MQTT_DealPushdata_Qs0(unsigned char *);	
void TxDataBuf_Deal(unsigned char *, int);
void CMDBuf_Deal(unsigned char *, int);

#endif
