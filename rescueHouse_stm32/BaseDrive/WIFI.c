/*-------------------------------------------------*/
/*                                                 */
/*            操作602Wifi功能的源文件              */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f4xx.h"  //包含需要的头文件
#include "wifi.h"	    //包含需要的头文件
#include "delay.h"	    //包含需要的头文件
#include "LED.h"        //包含需要的头文件
#include "mqtt.h"       //包含需要的头文件
#include "lcd.h"
#include <string.h>
#include <stdio.h>

char wifi_mode = 0;     //联网模式 0：SSID和密码写在程序里   1：Smartconfig方式用APP发送
	
/*-------------------------------------------------*/
/*函数名：初始化WiFi的复位IO                       */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void WiFi_ResetIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    //  LED
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);   //使能GPIOB时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;               //LED对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //通用输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //输出推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;      //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //无上拉或下拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);                  //初始化 GPIO
    
	RESET_IO(1);                                              //复位IO拉高电平
}

/*-------------------------------------------------*/
/*函数名：Air724复位                                 */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_Reset(int timeout)
{
    Uart5RXlenth = 0;
    Uart5ReceiveFinish = 0;
    
    RESET_IO(0);                                    //复位IO拉低电平
	delay_ms(500);                                  //延时500ms
	RESET_IO(1);                                    //复位IO拉高电平	

	while(timeout--){                               //等待超时时间到0
		delay_ms(100);                              //延时100ms
        
        if(Uart5ReceiveFinish == 1)
        {
            Uart5RXlenth = 0;
            Uart5ReceiveFinish = 0;
            if(strstr((void*)Uart5ReceiveBuf,"ready"))  //如果接收到ready表示复位成功
                break;                              //主动跳出while循环
        }
		printf("%d ",timeout);                      //串口输出现在的超时时间
	}
	printf("\r\n");                                 //串口输出信息
    
	if(timeout<=0)return 1;                         //如果timeout<=0，说明超时时间到了，也没能收到ready，返回1
	else return 0;                                  //反之，表示正确，说明收到ready，通过break主动跳出while
}


/*-------------------------------------------------*/
/*函数名：WiFi发送设置指令                         */
/*参  数：cmd：指令                                */
/*参  数：timeout：超时时间（100ms的倍数）         */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_SendCmd(char *cmd,  int len, int timeout)
{
	Uart5RXlenth = 0;
    Uart5ReceiveFinish = 0;
	UART5_Senddata((void*)cmd, len);
	while(timeout--){                           //等待超时时间到0
		delay_ms(100);                          //延时100ms
		if(strstr((void*)Uart5ReceiveBuf,"OK"))     //如果接收到OK表示指令成功
			break;       						//主动跳出while循环
		printf("%d ",timeout);                  //串口输出现在的超时时间
	}
	printf("\r\n");                             //串口输出信息
	if(timeout<=0)return 1;                     //如果timeout<=0，说明超时时间到了，也没能收到OK，返回1
	else return 0;		         				//反之，表示正确，说明收到OK，通过break主动跳出while
}

/*-------------------------------------------------*/
/*函数名：WiFi加入路由器指令                       */
/*参  数：timeout：超时时间（1s的倍数）            */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_JoinAP(int timeout)
{
    char temp[100] = "";
    sprintf(temp,"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PASS);
    
	Uart5RXlenth = 0;
    Uart5ReceiveFinish = 0;
    UART5_Senddata((void*)temp, strlen(temp));
    
	while(timeout--){                               //等待超时时间到0
		delay_ms(1000);                             //延时1s
//		if(strstr((void*)Uart5ReceiveBuf ,"WIFI GOT IPOK")) //如果接收到WIFI GOT IP表示成功
		if(strstr((void*)Uart5ReceiveBuf,"\n\r\nOK"))   //连接成功
			break;       						    //主动跳出while循环
		printf("%d ",timeout);                      //串口输出现在的超时时间
	}
	printf("\r\n");                                 //串口输出信息
	if(timeout<=0)return 1;                         //如果timeout<=0，说明超时时间到了，也没能收到WIFI GOT IP，返回1
	return 0;                                       //正确，返回0
}
/*-------------------------------------------------*/
/*函数名：连接TCP服务器，并进入透传模式            */
/*参  数：timeout： 超时时间（100ms的倍数）        */
/*返回值：0：正确  其他：错误                      */
/*-------------------------------------------------*/
char WiFi_Connect_Server(int timeout)
{	
    char buf[100] = "";
    
    sprintf(buf, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ServerIP, ServerPort);
    
    Uart5RXlenth = 0;
    Uart5ReceiveFinish = 0;
    UART5_Senddata((void*)buf, strlen(buf));
    
	while(timeout--){                               //等待超时与否
		delay_ms(100);                              //延时100ms	
		if(strstr((void*)Uart5ReceiveBuf ,"CONNECT"))   //如果接受到CONNECT表示连接成功
			break;                                  //跳出while循环
		if(strstr((void*)Uart5ReceiveBuf ,"CLOSED"))    //如果接受到CLOSED表示服务器未开启
			return 1;                               //服务器未开启返回1
		if(strstr((void*)Uart5ReceiveBuf ,"ALREADY CONNECTED"))//如果接受到ALREADY CONNECTED已经建立连接
			return 2;                               //已经建立连接返回2
		printf("%d ",timeout);                   //串口输出现在的超时时间  
	}
	printf("\r\n");                        //串口输出信息
	if(timeout<=0)return 3;                   //超时错误，返回3
	else                                      //连接成功，准备进入透传
	{
		printf("连接服务器成功，准备进入透传\r\n");  //串口显示信息
		Uart5RXlenth = 0;
        Uart5ReceiveFinish = 0;
        UART5_Senddata("AT+CIPSEND\r\n", strlen("AT+CIPSEND\r\n")); 
		while(timeout--){                               //等待超时与否
			delay_ms(100);                              //延时100ms	
			if(strstr((void*)Uart5ReceiveBuf,"\r\nOK\r\n\r\n>"))    //如果成立表示进入透传成功
				break;                          //跳出while循环
			printf("%d ",timeout);           //串口输出现在的超时时间  
		}
		if(timeout<=0)return 4;                 //透传超时错误，返回4	
	}
	return 0;	                                //成功返回0	
}

/*-------------------------------------------------*/
/*函数名：WiFi连接服务器                           */
/*参  数：无                                       */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_Connect_IoTServer(void)
{	
	printf("准备复位模块\r\n");                     //串口提示数据
    LCD_ShowString(30, 50, "准备复位模块...          ", BLACK, WHITE);
	if(WiFi_Reset(50)){                                //复位，100ms超时单位，总计5s超时时间
		printf("复位失败，准备重启\r\n");           //返回非0值，进入if，串口提示数据
        LCD_ShowString(30, 50, "复位失败，准备重启     ", BLACK, WHITE);
		return 1;                                      //返回1
	}
    else {
        printf("复位成功\r\n");                   //串口提示数据
        LCD_ShowString(30, 50, "复位成功              ", BLACK, WHITE);
    }
	
	printf("准备设置STA模式\r\n");                  //串口提示数据
    LCD_ShowString(30, 50, "准备设置STA模式...       ", BLACK, WHITE);
	if(WiFi_SendCmd("AT+CWMODE=1\r\n",strlen("AT+CIPMODE=1\r\n"),50)){  //设置STA模式，100ms超时单位，总计5s超时时间
		printf("设置STA模式失败，准备重启\r\n");    //返回非0值，进入if，串口提示数据
        LCD_ShowString(30, 50, "设置STA模式失败，准备重启  ", BLACK, WHITE);
		return 2;                                      //返回2
	}else {
        printf("设置STA模式成功\r\n");            //串口提示数据
        LCD_ShowString(30, 50, "设置STA模式成功         ", BLACK, WHITE);
    }
	
	if(wifi_mode==0){                                      //如果联网模式=0：SSID和密码写在程序里 
		printf("准备取消自动连接\r\n");                 //串口提示数据
        LCD_ShowString(30, 50, "准备取消自动连接...       ", BLACK, WHITE);
		if(WiFi_SendCmd("AT+CWAUTOCONN=0\r\n",strlen("AT+CWAUTOCONN=0\r\n"),50)){  //取消自动连接，100ms超时单位，总计5s超时时间
			printf("取消自动连接失败，准备重启\r\n");   //返回非0值，进入if，串口提示数据
            LCD_ShowString(30, 50, "取消自动连接失败，准备重启 ", BLACK, WHITE);
			return 3;                                      //返回3
		}else {
            printf("取消自动连接成功\r\n");           //串口提示数据
            LCD_ShowString(30, 50, "取消自动连接成功       ", BLACK, WHITE);
        }

		printf("准备连接路由器\r\n");                   //串口提示数据
        LCD_ShowString(30, 50, "准备连接路由器...       ", BLACK, WHITE);        
		if(WiFi_JoinAP(30)){                               //连接路由器,1s超时单位，总计30s超时时间
			printf("连接路由器失败，准备重启\r\n");     //返回非0值，进入if，串口提示数据
            LCD_ShowString(30, 50, "连接路由器失败，准备重启 ", BLACK, WHITE);
			return 4;                                      //返回4	
		}else {
            printf("连接路由器成功\r\n");             //串口提示数据		
            LCD_ShowString(30, 50, "连接路由器成功      ", BLACK, WHITE);
        }            
	}
	
	printf("准备设置透传\r\n");                     //串口提示数据
    LCD_ShowString(30, 50, "准备设置透传...      ", BLACK, WHITE);
	if(WiFi_SendCmd("AT+CIPMODE=1\r\n",strlen("AT+CIPMODE=1\r\n"),50)){ //设置透传，100ms超时单位，总计5s超时时间
		printf("设置透传失败，准备重启\r\n");       //返回非0值，进入if，串口提示数据
        LCD_ShowString(30, 50, "设置透传失败，准备重启 ", BLACK, WHITE);
		return 8;                                      //返回8
	}else {
        printf("设置透传成功\r\n");               //串口提示数据
        LCD_ShowString(30, 50, "设置透传成功      ", BLACK, WHITE);
    }
	
	printf("准备关闭多路连接\r\n");                 //串口提示数据
    LCD_ShowString(30, 50, "准备关闭多路连接...  ", BLACK, WHITE);
	if(WiFi_SendCmd("AT+CIPMUX=0\r\n",strlen("AT+CIPMUX=0\r\n"),50)){   //关闭多路连接，100ms超时单位，总计5s超时时间
		printf("关闭多路连接失败，准备重启\r\n");   //返回非0值，进入if，串口提示数据
        LCD_ShowString(30, 50, "关闭多路连接失败，准备重启", BLACK, WHITE);
		return 9;                                      //返回9
	}else {
        printf("关闭多路连接成功\r\n");           //串口提示数据
        LCD_ShowString(30, 50, "关闭多路连接成功    ", BLACK, WHITE);
    }
	
	printf("准备连接服务器\r\n");                   //串口提示数据
    LCD_ShowString(30, 50, "准备连接服务器...    ", BLACK, WHITE);
	if(WiFi_Connect_Server(100)){                      //连接服务器，100ms超时单位，总计10s超时时间
		printf("连接服务器失败，准备重启\r\n");     //返回非0值，进入if，串口提示数据
        LCD_ShowString(30, 50, "连接服务器失败，准备重启", BLACK, WHITE);
		return 10;                                     //返回10
	}else {
        printf("连接服务器成功\r\n");             //串口提示数据	
        LCD_ShowString(30, 50, "连接服务器成功    ", BLACK, WHITE);
    }
	
	return 0;                                          //正确返回0
	
}
