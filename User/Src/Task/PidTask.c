#include "PidTask.h"

void pidInit(_pidStructure* pid, float* Kpid){
	pid->p = Kpid[0];
	pid->i = Kpid[1];
	pid->d = Kpid[2];
	pid->iOutLimit = Kpid[3];
	pid->outLimit = Kpid[4];
	pid->pGet = pidGet;
	pid->pCalculate = pidCalculate;
}

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
		pid->out = pid->pOut;
}
	if(iFlag){
		pid->interval += pid->error;
		pid->iOut = pid->interval * pid->i;
		amplitudeLimiting(pid->iEnable,
											pid->iOut,
											pid->iOutLimit);
		pid->out = pid->out + pid->iOut;
	}
	if(dFlag){
		pid->dOut = (pid->error - pid->lastError)* pid->d;
		pid->lastError = pid->error;
		pid->out = pid->out + pid->dOut;
	}
	return pid->out;
}
