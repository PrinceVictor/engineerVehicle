#include "ControlTask.h"
_sysState sys = {1};
int16_t output[4] = {
	100, 100, 100, 100
};
int flag = 0;
int8_t runControl(_sysState* sys){
	switch(sys->state){
		case init:{
			if(allParaInit(wheelpid, chassispid))
			{
				sys->state = run;
				flag = 1;
			}
			break;
		}
		case run:{
			commuiModeChange(&sys->remoteOrkeyboard,
											&remote, 
											&chassisPara);
			chassisControl(1);
//			canTrans(1, 1, &canM, wheelInfo.out);
//			flag 1 to transferType,  flag 2 to read speed, 3 to read position
			canTrans(flag, 1, &canM, output);
			break;
		}
		case stop:{
			break;
		}
		case reset:{
			break;
		}
}		
	return 1;
}

