# 1.作品介绍

**基于STM32的应急救援仓系统的设计与实现**

该系统运用STM32F407以及Vue、SpringBoot等相关技术开发具有pm2.5，温湿度监测，太阳能供电系统和智能灯开关控制的应急救援仓，通过管理界面对环境信息、电源信息和远程报警等的相关信息进行显示和管理。

该系统主要基于**MQTT协议**进行STM32硬件与服务器间的双向全双工通信。

**主要功能包括：**

1.查看应急救援仓的环境监测数据 

（1） 查看当前温湿度

（2） 查看当前光照强度

（3） 查看当前PM2.5

（4） 查看当前所处位置

（5） 查看当前剩余电量

2.查看报警信息以及报警原因

3.自定义阈值自动或手动远程控制应急救援仓



# 2.目录介绍

* rescueHouse_stm32：基于STM32F407的硬件设计代码
* rescueHouse_SpringBoot：基于SpringBoot的后端代码设计与实现
* rescueHouse_Web：基于Vue的后台管理系统设计与实现
* images：README文档图片





# 3.系统设计

基于Stm32和前后端开发的应急救援仓系统可以用于对各种数据进行采集并实时展示，以便在紧急情况下提供及时的响应和救援。

该系统由以下几个组成部分：

**前端**：用户可以通过前端页面查看数据，并控制系统的运行。前端使用VUE框架，开发过程简单，页面精美。

**后端**：负责处理数据的存储、处理和分析，以及与前端的通信交互。后端使用SpringBoot框架开发，使用MySQL数据库进行数据存储。

**STM32单片机**：用于采集各种传感器的数据，并将其发送到后端进行处理。使用STM32F407系列的单片机，通过I2C接口连接传感器，并使用WiFi模块将数据发送到后端服务器。

**传感器**：用于测量各种物理量，例如温度、湿度、光照等。

此系统能够为应急救援工作提供支持，实时展示数据并进行分析，帮助救援人员更好的了解事态和做出决策。

## 3.1 信息采集模块

信息采集模块的原理如下：

1、 传感器把数据采集，并且通过总线把数据传回stm32。

2、 stm32接收到数据后，通过UART和WIFI模块，采用AT指令，把数据传输出去。

![img](https://github.com/LiangZii/rescueHouse/blob/master/iamges/clip_image002.gif)

*图3.1光照传感器BH1750*

![img](https://github.com/LiangZii/rescueHouse/blob/master/iamges/clip_image004.gif)

*图3.2 PM2.5传感器_PMS9003M*

 

## 3.2 网络传输模块

**3.2.1 ESP8266 wifi模块**

ESP8266是一款超低功耗的UART-WiFi 透传模块，拥有业内极富竞争力的封装尺寸和超低能耗技术，专为移动设备和物联网应用设计，可将用户的物理设备连接到Wi-Fi 无线网络上，进行互联网或局域网通信，实现联网功能。

ESP8266模块支持STA/AP/STA+AP 三种工作模式。

STA 模式：ESP8266模块通过路由器连接互联网，手机或电脑通过互联网实现对设备的远程控制。

AP 模式：ESP8266模块作为热点，实现手机或电脑直接与模块通信，实现局域网无线控制。

STA+AP 模式：两种模式的共存模式，即可以通过互联网控制可实现无缝切换，方便操作。

![img](https://github.com/LiangZii/rescueHouse/blob/master/iamges/clip_image010.gif)

*图3.3 ESP8266引脚图*



**3.2.2 MQTT** **协议**

MQTT协议(Message Queuing Telemetry Transport)：消息队列遥测传输协议。是一种基于客户端-服务端的发布/订阅模式。与 HTTP 一样，基于TCP/IP协议之上的通讯协议，提供有序、无损、双向连接，由IBM发布。

有三种身份：发布者（Publish）、代理（Broker）（服务器）、订阅者（Subscribe）。其中，消息的发布者和订阅者都是客户端，消息代理是服务器，消息发布者可以同时是订阅者。

MQTT传输的消息分为：主题（Topic）和负载（payload）两部分

Topic：可以理解为消息的类型，订阅者订阅（Subscribe）后，就会收到该主题的消息内容（payload）

Payload：消息的内容

 

![img](https://github.com/LiangZii/rescueHouse/blob/master/iamges/clip_image014.jpg)

*图3.4 MQTT的三种身份* 

![img](https://github.com/LiangZii/rescueHouse/blob/master/iamges/clip_image016.jpg)

*图3.5 MQTT消息传输过程*

## 3.3 数据存储模块

Stm32采集到数据后，通过WIFI模块把数据传输给后端，后端接收到数据后，把数据存到数据库中。

 

![img](https://github.com/LiangZii/rescueHouse/blob/master/iamges/clip_image018.gif)

*图3.6 数据库E-R图*



# 4.系统介绍

## 4.1 总体概况

用户进入平台后即进入总体概况界面，可以直接查看相关总体信息，如：总体概览、救援仓台账、环境监测、故障报警与检修等。界面如下图1所示。

![image-20240402103424440](https://github.com/LiangZii/rescueHouse/blob/master/iamges/image-20240402103424440.png)



## 4.2 救援仓台账

实时通过后端服务现实当前已部署救援仓的基本信息，包括救援仓ID、所属位置，当前报警数；以及详细信息，包括剩余电量、经纬度、温度、湿度、光照强度、PM2.5等环境数值。同时调用**百度地图API**，在一张地图中宏观的展示当前已部署且正在运行的救援仓的位置及信息。并对含有报警问题的救援仓提供特殊处理，以达到宏观查看和处理含有问题的救援仓。其实现效果如下图2和图3所示：

![image-20240402103619662](https://github.com/LiangZii/rescueHouse/blob/master/iamges/image-20240402103619662.png)

![image-20240402103625675](https://github.com/LiangZii/rescueHouse/blob/master/iamges/image-20240402103625675.png)

## 4.3 环境监测

该页面可以选择相应的救援仓以获取当前位置的PM2.5、温度、湿度、光照强度等关键环境信息，并配合Echarts生成统计信息和图标，值观展示最近变化。如下图4所示。

![image-20240402103721411](https://github.com/LiangZii/rescueHouse/blob/master/iamges/image-20240402103721411.png)



## 4.4 故障报警与检修

略。



## 4.5 灯光控制与报警

基于**MQTT协议**可实现实时查看传感器数值，并在到达设定的阈值时实现自动报警。当然，也可通过STM32的按钮自动触发报警系统。

除此之外，还可实现通过后台管理系统远程控制STM32模拟的应急救援仓的状态。

![image-20240402104912456](https://github.com/LiangZii/rescueHouse/blob/master/iamges/image-20240402104912456.png)





谢谢~~
