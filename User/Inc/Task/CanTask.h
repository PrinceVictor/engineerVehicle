#ifndef __CANTASK_H_
#define __CANTASK_H_

#include "MyFunc.h"

typedef struct canMessage{
	CanTxMsg canTx;
	void (*transType)(int8_t mode,
	struct canMessage* message,
	float* data);
}_canMessage;

int8_t canTransfer(uint8_t, CanTxMsg*);

#endif



