#include "PidTask.h"

int16_t pidGet(_pid_Para* pidPara,
							_pid_Out* pidOut,
							float target,
							int16_t feedback,
							uint8_t flag)
{
	pidOut->target = target;
	pidOut->feedback = (float)feedback;
	pidOut->error = target - (float)feedback;
if(!flag){
		return 0;
}
	else{
		pidOut->p_Out = pidPara->kp * pidOut->error;
		pidOut->Out = pidOut->p_Out;
}
	if(pidPara->i_flag){
		pidOut->i_interval += pidOut->error;
		pidOut->i_Out = pidOut->i_interval * pidPara->ki;
		amplitudeLimiting(pidPara->i_flag,
											pidOut->i_Out,
											pidPara->ki_limit);
		pidOut->Out = pidOut->Out + pidOut->i_Out;
	}
	if(pidPara->d_flag){
		pidOut->d_Out = (pidOut->error - pidOut->last_error)* pidPara->kd;
		pidOut->last_error = pidOut->error;
		pidOut->Out = pidOut->p_Out + pidOut->d_Out;
	}
	amplitudeLimiting(flag,
										pidOut->Out,
										pidPara->outlimit);
	return (int16_t)pidOut->Out;
}
