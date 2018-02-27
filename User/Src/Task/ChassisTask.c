#include "ChassisTask.h"

_wheelPara wheelInfo = {0};
_chassis chassisPara = {0};
float observeData[4] = {0};
float observeData1[4] = {0};

float wheelpid[] = {
	10,	// kp
	0,	// ki
	0,	// kd
	0,	// iLimit
	1,	// enablefag
	100,	// outlimit   range:+- 32768
	2500,	// speedLimit
	1			// Kspeed

};

float chassispid[] = {
	10,	// kp
	0,	// ki
	0,	// kd
	0,	// iLimit
	1,	// enablefag
	0,		// outlimit     range:+- 32768
	3,
	2
};

int8_t allParaInit(void)
{
	return 1;
} 



int8_t chassisControl(uint8_t flag)
{	
	int i;
	if(!flag){
		return 0;
}
	else{
	//pid for angle
										 pidGet(&chassisPara.pid.shell.k_para,
														&chassisPara.pid.shell.pid,		
														chassisPara.yaw.target,
														chassisPara.yaw.angle,
														chassisPara.pid_flag);
	//pid for angle_speed
		chassisPara.Rt = pidGet(&chassisPara.pid.core.k_para,
														&chassisPara.pid.core.pid,		
														chassisPara.pid.shell.pid.Out,
														chassisPara.yaw.angle,
														chassisPara.pid_flag);
	//wheel solute
		wheelSolute(&wheelInfo, &chassisPara);
	//pid for wheels
		for(i=0; i<4; i++){
			wheelInfo.out[i] = pidGet(&wheelInfo.kpid,
																&wheelInfo.pid[i],
																wheelInfo.targetSpeed[i],
																wheelInfo.feedback.Speed[i],
																wheelInfo.pid_flag);
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
			amplitudeLimiting(1, para->direction[i]*para->K_speed, para->speedLimit);
	}
	return 1;
}


