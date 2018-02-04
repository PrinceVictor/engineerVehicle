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
	void	(*pGet)(struct pidStructure* pid,
									float target,
									float feedback);
	float (*pCalculate)(struct pidStructure *pid,
											uint8_t iFlag,
											uint8_t dFlag);
}_pidStructure;

typedef	struct pidDouble{
	_pidStructure	shell;
	_pidStructure	core;	
}_pidDouble;

#endif 