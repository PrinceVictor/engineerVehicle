#include "ControlTask.h"
_sysState sys = {1};

int8_t runControl(_sysState* sys){
	switch(sys->state){
		case init:{
			if(allParaInit(&wheelInfo, &chassisPara, wheelpid, chassispid))
				sys->state = run;
			break;
		}
		case run:{
			commuiModeChange(&sys->remoteOrkeyboard,
											&remote, 
											&chassisPara);
			chassisControl(0, &chassisPara, &wheelInfo);
			canTrans(0, 1, &canM, (int16_t*)wheelInfo.out);
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

