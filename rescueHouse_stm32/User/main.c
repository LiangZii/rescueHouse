/****************************************Copyright (c)***************************************
**-------------------------------------------------------------------------------------------
** Created by:          qinyx
** Last modified Date:  2014-02-28
** Last Version:        V1.00
** Descriptions:        STM32F407嵌入式实验箱
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
void sys_State(void);       //函数声明，判断系统状态，并发布给服务器
unsigned char lamp_mode[4] = MANU;
unsigned int illuMin = ILLU_MIN;
unsigned int illuMax = ILLU_MAX;

int main(void)
{
		char *stringTemp;
		unsigned char keyValue;
		char disbuf[50] = "";
		
		SysTick_Init();                 //  系统滴答定时器初始化
    LEDGpio_Init();									//灯
    BEEPGpio_Init();								//蜂鸣器
    KEYGpio_Init(); 								//按键驱动
    I2C_GPIO_Configuration();       //  配置I2C的GPIO   ↓
	  BH1750_Init();                  //  初始化BH1750   光照传感器
    UART3_Configuration();          //  USART3配置
    //UART6_Configuration(9600);      //  PM2.5
    UART5_Configuration(115200);    //  WIFI串口


		LCD_Init();                     //  液晶初始化
    LCD_Clear(WHITE);               //  设置液晶背景
	  LCD_ShowString(250, 10, "智能救援仓管理系统-救援仓1", BLACK, WHITE);
    sprintf(disbuf,"SSID:%s   PASS:%s",SSID,PASS);
    LCD_ShowString(30, 30, (u8 *)disbuf, BLACK, WHITE);
    

		WiFi_ResetIO_Init();            //  初始化WiFi的复位IO
    AliIoT_Parameter_Init();	    //  初始化连接EMQX服务器MQTT服务器的参数


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
										warn_State(1);    //手动报警
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
            /*                     处理发送缓冲区数据                      */
            /*-------------------------------------------------------------*/
            if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr)                 //if成立的话，说明发送缓冲区有数据了
            {
                //3种情况可进入if
                //第1种：0x10 连接报文
                //第2种：0x82 订阅报文，且ConnectPack_flag置位，表示连接报文成功
                //第3种：SubcribePack_flag置位，说明连接和订阅均成功，其他报文可发
                if((MQTT_TxDataOutPtr[2] == 0x10) || ((MQTT_TxDataOutPtr[2] == 0x82) && (ConnectPack_flag == 1)) || (SubcribePack_flag == 1))
                {
                    printf("发送数据:0x%x\r\n", MQTT_TxDataOutPtr[2]);  //串口提示信息
                    UART5_Senddata(&MQTT_TxDataOutPtr[2], (MQTT_TxDataOutPtr[0]*256 + MQTT_TxDataOutPtr[1]));
                    MQTT_TxDataOutPtr += TBUFF_UNIT;                    //指针下移
                    if(MQTT_TxDataOutPtr == MQTT_TxDataEndPtr)          //如果指针到缓冲区尾部了
                        MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];          //指针归位到缓冲区开头
                }
            }//处理发送缓冲区数据的else if分支结尾
            
            /*-------------------------------------------------------------*/
            /*                     处理接收缓冲区数据                      */
            /*-------------------------------------------------------------*/
            if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr)   //if成立的话，说明接收缓冲区有数据了
            {
                printf("接收到数据:");
                /*-----------------------------------------------------*/
                /*                    处理CONNACK报文                  */
                /*-----------------------------------------------------*/
                //if判断，如果第一个字节是0x20，表示收到的是CONNACK报文
                //接着我们要判断第4个字节，看看CONNECT报文是否成功
                if(MQTT_RxDataOutPtr[2] == 0x20)
                {
                    switch(MQTT_RxDataOutPtr[5])
                    {
                    case 0x00 :
                        printf("CONNECT报文成功\r\n");                      //串口输出信息
                        LCD_ShowString(30, 50, "CONNECT成功          ", BLACK, WHITE);
                        ConnectPack_flag = 1;                               //CONNECT报文成功，订阅报文可发
                        break;                                              //跳出分支case 0x00
                    case 0x01 :
                        printf("连接已拒绝，不支持的协议版本，准备重启\r\n");  //串口输出信息
                        Connect_flag = 0;                                   //Connect_flag置零，重启连接
                        break;                                              //跳出分支case 0x01
                    case 0x02 :
                        printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n"); //串口输出信息
                        Connect_flag = 0;                                   //Connect_flag置零，重启连接
                        break;                                              //跳出分支case 0x02
                    case 0x03 :
                        printf("连接已拒绝，服务端不可用，准备重启\r\n");    //串口输出信息
                        Connect_flag = 0;                                   //Connect_flag置零，重启连接
                        break;                                              //跳出分支case 0x03
                    case 0x04 :
                        printf("连接已拒绝，无效的用户名或密码，准备重启\r\n"); //串口输出信息
                        Connect_flag = 0;                                   //Connect_flag置零，重启连接
                        break;                                              //跳出分支case 0x04
                    case 0x05 :
                        printf("连接已拒绝，未授权，准备重启\r\n");           //串口输出信息
                        Connect_flag = 0;                                   //Connect_flag置零，重启连接
                        break;                                              //跳出分支case 0x05
                    default   :
                        printf("连接已拒绝，未知状态，准备重启\r\n");          //串口输出信息
                        Connect_flag = 0;                                   //Connect_flag置零，重启连接
                        break;                                              //跳出分支case default
                    }
                }
                //if判断，第一个字节是0x90，表示收到的是SUBACK报文
                //接着我们要判断订阅回复，看看是不是成功
                else if(MQTT_RxDataOutPtr[2] == 0x90)
                {
                    switch(MQTT_RxDataOutPtr[6])
                    {
                    case 0x00 :
                    case 0x01 :
                        printf("订阅成功\r\n");                             //串口输出信息
                        LCD_ShowString(30, 50, "订阅成功              ", BLACK, WHITE);
                        SubcribePack_flag = 1;                              //SubcribePack_flag置1，表示订阅报文成功，其他报文可发送
                        Ping_flag = 0;                                      //Ping_flag清零
                        ping_cnt = 30;
                        break;                                              //跳出分支
                    default   :
                        printf("订阅失败，准备重启\r\n");                    //串口输出信息
                        
                        Connect_flag = 0;                                   //Connect_flag置零，重启连接
                        break;                                              //跳出分支
                    }
                }
                //if判断，第一个字节是0xD0，表示收到的是PINGRESP报文
                else if(MQTT_RxDataOutPtr[2] == 0xD0)
                {
                    printf("PING报文回复\r\n");                             //串口输出信息
                    Ping_flag = 0;                                      //要清除Ping_flag标志
                    ping_cnt = 30;
                }
                //if判断，如果第一个字节是0x30，表示收到的是服务器发来的推送数据
                //我们要提取控制命令
                else if((MQTT_RxDataOutPtr[2] == 0x30))
                {
                    printf("服务器等级0推送\r\n");                             //串口输出信息
                    MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);               //处理等级0推送数据
                }

                MQTT_RxDataOutPtr += RBUFF_UNIT;                            //指针下移
                if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)                  //如果指针到缓冲区尾部了
                    MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];                  //指针归位到缓冲区开头
            }//处理接收缓冲区数据的else if分支结尾
            
            /*-------------------------------------------------------------*/
            /*                     处理命令缓冲区数据                      */
            /*-------------------------------------------------------------*/
            if(MQTT_CMDOutPtr != MQTT_CMDInPtr)                             //if成立的话，说明命令缓冲区有数据了
            {
                printf("命令:%s\r\n", &MQTT_CMDOutPtr[2]);                                    //串口输出信息
                
								if(strstr((char *)MQTT_CMDOutPtr+2,"\"led2switch\": 1")){ //如果搜索到"params":{"PowerSwitch_2":1}说明服务器下发打开开关2	
										LED2_ON;
								}else if(strstr((char *)MQTT_CMDOutPtr+2,"\"led2switch\": 0")){ //如果搜索到"params":{"PowerSwitch_2":0}说明服务器下发关闭开关2
										LED2_OFF;
								}
								
								
                MQTT_CMDOutPtr += CBUFF_UNIT;                               //指针下移
                if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)                        //如果指针到缓冲区尾部了
                    MQTT_CMDOutPtr = MQTT_CMDBuf[0];                        //指针归位到缓冲区开头
            }//处理命令缓冲区数据的else if分支结尾
            
        }//Connect_flag=1的if分支的结尾
        
        else
        {
            printf("需要连接服务器\r\n");          //串口输出信息
            if(WiFi_Connect_IoTServer() == 0)       //如果Air724连接云服务器函数返回0，表示正确，进入if
            {
                printf("建立TCP连接成功\r\n");        //串口输出信息
                Connect_flag = 1;                   //Connect_flag置1，表示连接成功
                Uart5RXlenth = 0;                  //Air724接收数据量变量清零
                Uart5ReceiveFinish = 0;
                MQTT_Buff_Init();                   //初始化发送缓冲区
            }
        }
    }



}


/*---------------------------------------------------*/
/*函数名：判断报警状态，并发布给服务器                 */
/*参  数：sta       0.灯光阈值报警    1.手动报警      */
/*返回值：无                                         */
/*--------------------------------------------------*/
void warn_State(unsigned char sta)
{
		char temp[TBUFF_UNIT];                   //定义一个临时缓冲区
		
		if (sta == 0)
    {
        LCD_ShowString(200, 290, "灯光阈值报警", BLACK, WHITE);
				sprintf(temp,"{\"warnId\": 0}");  //需要回复状态给服务器
    }
    else if(sta == 1)
    {
        LCD_ShowString(200, 290, "手动报警", BLACK, WHITE);
				sprintf(temp,"{\"warnId\": 1}");  //需要回复状态给服务器			
    }
		
		
    MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器		
}

/*-------------------------------------------------*/
/*函数名：判断并收集采集类模块状态，并发布给服务器                   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void sys_State(void)
{
	  char disbuf[50] = "";					//LCD屏显示缓冲
		char temp[TBUFF_UNIT];  			//MQTT缓冲
	
		unsigned int light_temp = 0;  //光照强度缓冲
		unsigned int light_date = 0;
		unsigned char Databuf[3] = {0};
	
		BH1750_ReadData(BH1750_SlaveAddress, Databuf);
        
		light_temp = Databuf[0];
		light_temp = (light_temp << 8) + Databuf[1];
		light_date = light_temp*10/12;
		
		if(light_date<10)
    {
				LED1_ON;			//开灯
				warn_State(0);    //报警
		}
		else
		{
				LED1_OFF;
		}
		
		
		//打印输出调试
		sprintf(disbuf,"光照强度:%3dug/m3",light_date);
    LCD_ShowString(200, 80, (u8 *)disbuf, BLACK, WHITE);
    
    sprintf(temp,"{\"light\": %d}",light_date);  //需要回复状态给服务器
    
		MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器	
}

