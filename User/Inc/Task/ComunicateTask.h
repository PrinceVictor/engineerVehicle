#ifndef __COMUNICATETASK_H_
#define __COMUNICATETASK_H_

#include "MyFunc.h"
#include "PidTask.h"
#include "ChassisTask.h"
#include "stm32f4xx_can.h"
#include "math.h"




typedef struct canMessage{
	CanTxMsg canTx;
	CanRxMsg canRx;
}_canMessage;


int8_t canTrans(uint8_t, int8_t , _canMessage* , int16_t* );
void transferType(int8_t , _canMessage* , int16_t* );

extern _canMessage canM;

#endif
