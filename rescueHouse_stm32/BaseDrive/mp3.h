#ifndef _MP3_H_
#define _MP3_H_


#define UDISK   0
#define FLASH0   4


void setMp3Dev(unsigned char dev);
void setMp3Vol(unsigned char vol);
void mp3Play(void);
void mp3Stop(void);

#endif
