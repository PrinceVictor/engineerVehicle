#include "RelayControl.h"
_relay relay ={0};
const _relay relay_init ={0};
_redlaser redlaser ={0};
/***************************************
b00000000 -0x00  所有气缸闭合
b00000001 -0x01  抬升所有底盘轮子和辅助轮
b00000010 -0x02  收起前轮
b00000011 -0x03  收起后轮
b00000100 -0x04  收起辅助轮

***************************************/
void scan_relayflag(const _relay relay_flag){
	
	forward_leg = relay_flag.forward_leg_flag;
	backward_leg = relay_flag.backward_leg_flag;
	support_leg = relay_flag.support_leg_flag;
	middle_leg = relay_flag.middle_leg_flag;
	rescue_left = relay_flag.rescue_left_flag;
	rescue_right = relay_flag.rescue_right_flag;
	bullet_take = relay_flag.distribute_Bigbullet_flag;
	bullet_take_stretch = relay_flag.distribute_up_Samllbullet_flag;
	distribute_Bigbullet = relay_flag.distribute_down_Samllbullet_flag;
	distribute_up_Samllbullet = relay_flag.distribute_stretch_flag;
	distribute_down_Samllbullet = relay_flag.bullet_take_flag;
	distribute_stretch = relay_flag.bullet_take_stretch_flag;
	
}

int8_t signal_process(const uint8_t signal_flag, const int8_t flag){
	if(!flag) return 0;
	else{
		switch(signal_flag){
			case 0x00:{
					relay = relay_init;
					break;
				}
				case 0x01:{
					relay.forward_leg_flag = 1;
					relay.backward_leg_flag = 1;
					relay.support_leg_flag =1;
					break;
				}
				case 0x02:{
					relay.forward_leg_flag = 0;
					break;
				}
				case 0x03:{
					relay.backward_leg_flag = 0;
					break;
				}
				case 0x04:{
					relay.support_leg_flag =0;
					break;
				}
				default: break;

}
}
}