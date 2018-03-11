#ifndef __IO_H__
#define __IO_H__

#include "MyFunc.h"

#define forward_leg PCout(13)
#define backward_leg PCout(1)
#define support_leg PAout(5)
#define middle_leg PCout(4)

#define rescue_left PCout(15)
#define rescue_right PCout(0)
#define bullet_take PAout(4)
#define bullet_take_stretch PAout(7)

#define distribute_Bigbullet PBout(6)
#define distribute_up_Samllbullet PBout(3)
#define distribute_down_Samllbullet PAout(9)
#define distribute_stretch PCout(9)

#define front_left PAin(6)
#define front_right PBin(0)
#define mid_left PAin(0)
#define mid_right PCin(3)

void IO_Init(void);
#endif
