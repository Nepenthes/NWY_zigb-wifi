#ifndef MOD_DETECT_H
#define MOD_DETECT_H

#include "Driver_USART.h"
#include "IO_Map.h"
#include "delay.h"

#include "stdio.h"
#include "string.h"

#define ModDEC_P1	(((GPIO_ReadInputData(GPIOA) & 0x01f0) >> 4) & 0x1f)
#define ModDEC_P2	(((GPIO_ReadInputData(GPIOB) & 0x00f8) >> 3) & 0x1f)
#define ModDEC_P3	(((GPIO_ReadInputData(GPIOD) & 0x00f8) >> 3) & 0x1f)

#define DataTransP1_CHK	PDin(10)
#define DataTransP2_CHK	PDin(11)
#define DataTransP3_CHK	PDin(12)

#define DataTransP1_OK	PAout(0)
#define DataTransP2_OK	PEout(5)
#define DataTransP3_OK	PAout(11)

typedef enum{

	typeDT_NULL = 0x1f,
	typeDT_ZIGB = 1,
	typeDT_WIFI = 2,
}type_DTMod;

void modDetect_Thread(const void *argument);

void threadModDetect_Active(void);

#endif

