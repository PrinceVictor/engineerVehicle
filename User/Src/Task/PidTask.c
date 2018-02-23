#include "PidTask.h"


void pidInit(_pidStructure* pid, float* Kpid){
	pid->p = Kpid[0];
	pid->i = Kpid[1];
	pid->d = Kpid[2];
	pid->iOutLimit = Kpid[3];
	pid->enableFlag = (uint8_t)Kpid[4];
	pid->outLimit = Kpid[5];
}

void fbProcess(int16_t* data, float* processed){
	int8_t i=0;
	for(i =0; i<4; i++)
		processed[i] = (float)data[i];
}

int16_t pidGet(_pidStructure* pid, 
							float target, 
							int16_t feedback,
							float* p,
							float* p1,
							uint8_t iFlag,
							uint8_t dFlag){
	pid->target = target;
	pid->feedback = (float)feedback;
	pid->error = target - (float)feedback;
if(pid->enableFlag == 0){
		return 0;
}
	else{
		pid->pOut = pid->p * pid->error;
		pid->out = pid->pOut;
}
	if(iFlag){
		pid->out = (*p);
		pid->interval += pid->error;
		pid->iOut = pid->interval * pid->i;
		amplitudeLimiting(pid->iEnable,
											pid->iOut,
											pid->iOutLimit);
		pid->out = pid->out + pid->iOut;
		(*p) = pid->out;
	}
	if(dFlag){
		pid->lastError = (*p1);
		pid->dOut = (pid->error - pid->lastError)* pid->d;
		(*p1) = pid->error;
		pid->out = pid->out + pid->dOut;
	}
	return (int16_t)pid->out;
}
