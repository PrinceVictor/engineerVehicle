#include "ControlTask.h"
_sysState sys = {1};
int16_t output[4] = {
	100, 100, 100, 100
};
int flag = 0;
int duoji_flag = 0;
int8_t runControl(_sysState* sys){
	switch(sys->state){
			case init:{
				if(!flag)
				{
					sys->state = run;
					flag = 1;
				}
				break;
			}
			case run:{
				
	//			mode 1 to transferType,  mode 2 to read speed, 3 to read position
	//			canTrans(flag, 1, &canM, wheelInfo.out);
				break;
			}
			case stop:{

	//			canTrans(flag, 0, &canM, wheelInfo.out);
				break;
			}
			case reset:{
				break;
			}
			default: break;
}		
	return 1;
}

