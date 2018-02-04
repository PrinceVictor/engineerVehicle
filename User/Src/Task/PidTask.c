#include "PidTask.h"

void pidGet(_pidStructure* pid, float target, float feedback){
	pid->target = target;
	pid->feedback = feedback;
	pid->error = target - feedback;
	
}

float pidCalculate(_pidStructure* pid, 
										uint8_t iFlag,
										uint8_t dFlag){
	if(pid->enableFlag == 0){
		return 0;
}
	else{
		pid->pOut = pid->p * pid->error;
		pid->interval += pid->error;
		pid->iOut = pid->interval * pid->i;
		AmplitudeLimiting(pid->iEnable,
											pid->iOut,
											pid->iOutLimit,
											-pid->iOutLimit);
		pid->dOut = (pid->error - pid->lastError)* pid->d;
		pid->lastError = pid->error;
		pid->out = pid->pOut;
}
	if(iFlag) pid->out = pid->out + pid->iOut;
	if(dFlag) pid->out = pid->out + pid->dOut;
	return pid->out;
}