#include "ChassisTask.h"

_wheelPara wheelInfo = {0};
_chassis chassisPara = {0};

float wheelpid[] = {
	0,	// kp
	0,	// ki
	0,	// kd
	0,	// iLimit
	0,	// outlimit
	0,	// speedLimit
	0		// Kspeed
};

float chassispid[] = {
	0,	// kp
	0,	// ki
	0,	// kd
	0,	// iLimit
	0		// outlimit
};

int8_t allParaInit(_wheelPara* wheel, 
									_chassis* chassis, 
									float* paraWheel,
									float* paraChassis)
{
	wheel->pid.pidInit = pidInit;
	wheel->pid.pidInit(&wheel->pid, paraWheel);
	wheel->speedLimit = paraWheel[5];
	wheel->Kspeed = paraWheel[6];
	
	chassis->pid.pidInit = pidInit;
	chassis->pid.pidInit(&chassis->pid, paraChassis);
	return 1;
} 


int8_t chassisControl(uint8_t flag, _chassis* chassis, _wheelPara* para)
{	
	int i;
	if(flag == 0){
		return 0;
}
	else{
	//pid for angle
		chassis->pid.pGet(&chassis->pid, 
													chassisPara.yaw.target,
													chassisPara.yaw.angle);
		
		chassis->Rt = \
			chassis->pid.pCalculate(&chassisPara.pid,1,0);
	//wheel solute
		wheelSolute(para, chassis);
	//pid for wheels
		for(i=0; i<4; i++){
			para->pid.pGet(&para->pid,
											para->targetSpeed[i],
											para->feedback.Speed[i]);
			para->out[i] = para->pid.pCalculate(&chassis->pid,1,0);
		}
	return 1;
}
}

int8_t wheelSolute(_wheelPara* para, _chassis* chassis){
	int i;
	
	para->direction[0] = \
		chassis->x*(-chassis->Fb +chassis->Lr) + chassis->Rt;
	para->direction[1] = \
		chassis->x*(chassis->Fb +chassis->Lr) + chassis->Rt;
	para->direction[2] = \
		chassis->x*(chassis->Fb -chassis->Lr) + chassis->Rt;
	para->direction[3] = \
		chassis->x*(-chassis->Fb -chassis->Lr) + chassis->Rt;
	
	for(i=0; i<4; i++ ){
		para->targetSpeed[i] = \
			amplitudeLimiting(1, para->direction[i]*para->Kspeed, para->speedLimit);
	}
	return 1;
}


