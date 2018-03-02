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
			if(allParaInit())
			{
				sys->state = run;
				flag = 1;
			}
			break;
		}
		case run:{
			if(PAin(1)==0 && duoji_flag==0)
				{
					TIM_SetCompare4(TIM3,11);//11,22
					duoji_flag=1;
				}
	
			if(remote.rc.s2==1)
				{
					TIM_SetCompare4(TIM3,22);
					duoji_flag=0;
				}
			sys->state = commuiModeChange(&sys->remoteOrkeyboard,
															&remote, 
															&chassisPara);
			chassisControl(1);
//			mode 1 to transferType,  mode 2 to read speed, 3 to read position
			canTrans(flag, 1, &canM, wheelInfo.out);
			break;
		}
		case stop:{
			sys->state = commuiModeChange(&sys->remoteOrkeyboard,
															&remote, 
															&chassisPara);
			canTrans(flag, 0, &canM, wheelInfo.out);
			break;
		}
		case reset:{
			break;
		}
		default: break;
}		
	return 1;
}

