#ifndef _BEEP_H_
#define _BEEP_H_

//BEEP      PC13
#define  	BEEP_ON      	GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define  	BEEP_OFF      	GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define  	BEEP_REVERSE  	GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)!GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13))


void BEEPGpio_Init(void);

#endif
