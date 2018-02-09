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
				sys->state = run;
			break;
		}
		case run:{
			commuiModeChange(&sys->remoteOrkeyboard,
											&remote, 
											&chassisPara);
			chassisControl(1);
			canTrans(1, 1, &canM, wheelInfo.out);
//			canTrans(flag, 1, &canM, output);
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

