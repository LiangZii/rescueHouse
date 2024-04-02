#include "mp3.h"
#include "USART3.h"

void setMp3Dev(unsigned char dev)
{
    unsigned char device[5]  = {0x7E, 0x03, 0x09, 0x00, 0xEF};      // 指定设备 0：U 4:FLASH
    device[3] = dev;
    UART5_Senddata(device, 5);
}

void setMp3Vol(unsigned char vol)
{
    unsigned char volume[5]  = {0x7E, 0x03, 0x06, 0x00, 0xEF};      //	音量 0-30
    volume[3] = vol;
    UART5_Senddata(volume, 5);
}

void mp3Play(void)
{
    unsigned char Play[4]  = {0x7E, 0x02, 0x0D, 0xEF};                  //	播放
    UART5_Senddata(Play, 4); 
}

void mp3Stop(void)
{
    unsigned char Stop[4]  = {0x7E, 0x02, 0x10, 0xEF};                  //	停止
    UART5_Senddata(Stop, 4); 
}

