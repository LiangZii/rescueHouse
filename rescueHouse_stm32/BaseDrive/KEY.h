#ifndef _KEY_H_
#define _KEY_H_

/*KEYÒý½Å¶¨Òå
//KEY_S1      PF8
//KEY_S2      PF9
//KEY_S3      PF10
*/
#define  	KEY_S1_READ()  	GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8)
#define  	KEY_S2_READ()  	GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9)
#define  	KEY_S3_READ()  	GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_10)

void KEYGpio_Init(void);
unsigned char KeyScan(void);

#endif
