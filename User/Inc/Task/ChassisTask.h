#ifndef __CHASSISTASK_H__
#define __CHASSISTASK_H__

#include "MyFunc.h"
#include "PidTask.h"

typedef struct{
	float target;
	float temp;
	float angle;
}_chassisYaw;

typedef struct{
	float Fb;
	float Lr;
	float Rt;
	float x,y;
	_chassisYaw yaw;
	_pidStructure pid;
}_chassis;

typedef struct{
	int16_t Speed[4];
	int16_t Postion[4];
}_feedback;

typedef struct wheelPara{
	_pidStructure pid;
	_feedback feedback;
	float speedLimit;
	float Kspeed;
	float speed[4];
	float targetSpeed[4];
	float direction[4];
	int16_t out[4];
}_wheelPara;

extern _wheelPara wheelInfo;
extern _chassis chassisPara;
extern float wheelpid[];
extern float chassispid[];

int8_t chassisControl(uint8_t);
int8_t wheelSolute(_wheelPara*, _chassis*);
int8_t allParaInit(float* ,
									float* );

#endif



