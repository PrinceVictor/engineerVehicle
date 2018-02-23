#include "main.h"
float Send_data[4];

int main(void)
{  
	uint16_t co_unt = 0;
	boot();
	while(1){
		imu(1);	
		
		Send_data[0] = speed.Fb; 
		Send_data[1] = speed.Lr;
		
		if(co_unt >30){
			send_odm_msg(Send_data);
			co_unt = 0;
		}
		co_unt ++;
	}
}
