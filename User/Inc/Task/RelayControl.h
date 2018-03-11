#ifndef __RELAYCONTROL_H__
#define __RELAYCONTROL_H__

#include "MyFunc.h"
#include "IO.h"

typedef struct{
	int8_t forward_leg_flag;
	int8_t backward_leg_flag;
	int8_t support_leg_flag;
	int8_t middle_leg_flag;
	int8_t rescue_left_flag;
	int8_t rescue_right_flag;
	int8_t distribute_Bigbullet_flag;
	int8_t distribute_up_Samllbullet_flag;
	int8_t distribute_down_Samllbullet_flag;
	int8_t distribute_stretch_flag;
	int8_t bullet_take_flag;
	int8_t bullet_take_stretch_flag;
}_relay;

typedef struct{
	int8_t front_left_flag;
	int8_t front_right_flag;
	int8_t mid_left_flag;
	int8_t mid_right_flag;
}_redlaser;

#endif
