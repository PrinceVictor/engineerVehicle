#ifndef __PIDTASK_H__
#define __PIDTASK_H__

#include "MyFunc.h"

typedef struct pidStructure{
	uint8_t enableFlag;
	float P;
	float	I;
	float D;

}pidStructure;

typedef	struct _pidStructure{
	pidStructure	shell;
	pidStructure	core;
	float	pOut;
	float iOut;
	float dOut;
	
	float target;
	float out;
	float	feedback;
	
}_pidStructure;

#endif 