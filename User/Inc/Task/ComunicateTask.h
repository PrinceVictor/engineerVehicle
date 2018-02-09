#ifndef __COMUNICATETASK_H__
#define __COMUNICATETASK_H__

#include "MyFunc.h"
#include "PidTask.h"
#include "ChassisTask.h"
#include "stm32f4xx_can.h"


#define YAW_SENSITY 0.050f		//鼠标yaw轴灵敏度
#define PITCH_SENSITY 0.03f	//鼠标pitch轴灵敏度

#define LEFT_LIMINT_ANGLE 40.0f //角度限制
#define RIGHT_LIMINT_ANGLE -40.0f//角度限制

typedef struct RC_Ctl
{
	struct
	{
		uint16_t ch0;
		uint16_t ch1;
		uint16_t ch2;
		uint16_t ch3;
		uint8_t s1;
		uint8_t s2;
	}rc;
	struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
		uint8_t press_l;
		uint8_t press_r;
	}mouse;
	struct
	{
		uint16_t v;
	}key;
	int8_t (*process)(struct RC_Ctl* data,
										unsigned char* buffer);
}_RC_Ctl;

typedef struct canMessage{
	CanTxMsg canTx;
	CanRxMsg canRx;
}_canMessage;

typedef struct {
		uint32_t lasttime;
		uint32_t now;
		uint32_t count;
		float out;
		int lastKey;
		int isSameKey;
		uint32_t cnt;
		int32_t clock_cnt;
}_RampTime;

typedef struct{
	_RampTime W;
	_RampTime S;
	_RampTime A;
	_RampTime D;
}_moveKey;

int8_t canTrans(uint8_t, int8_t , _canMessage* , int16_t* );
void transferType(int8_t , _canMessage* , int16_t* );

extern _canMessage canM;

int8_t readRemote(_RC_Ctl*, unsigned char* );
int8_t commuiModeChange(int8_t* , _RC_Ctl* , _chassis* );
int8_t computerControl(_RC_Ctl*,_chassis*);
int8_t remoteControl(_RC_Ctl* , _chassis*);
int8_t remoteImitate(_RC_Ctl*,_chassis*);

extern _RC_Ctl remote;
float RampCal(_RampTime *RampT);

extern _RampTime RampTFB;
extern _RampTime RampTLF;
extern _RampTime Move_UD ;
extern _RampTime Move_LF ;

#endif
