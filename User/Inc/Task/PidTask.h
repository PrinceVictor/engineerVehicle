#ifndef __PIDTASK_H__
#define __PIDTASK_H__

#include "MyFunc.h"

typedef struct pidStructure{
	uint8_t enableFlag;
	float p;
	float	i;
	uint8_t iEnable;
	float interval;
	float d;
	float	pOut;
	float iOut;
	float iOutLimit;
	float dOut;
	float target;
	float lastError;
	float error;
	float feedback;
	float out;
	float outLimit;
	void	(*pInit)(struct pidStructure* pid,
										float* Kpid);
}_pidStructure;

typedef	struct pidDouble{
	_pidStructure	shell;
	_pidStructure	core;	
}_pidDouble;

int16_t pidGet(_pidStructure* , float , int16_t, 	float* ,float* ,uint8_t ,uint8_t );
void pidInit(_pidStructure* , float* );
#endif 
