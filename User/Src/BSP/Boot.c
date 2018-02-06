#include "Boot.h"


void ALLTASk_BooT(void){
	
	delay_init(168);
	
	clockConfig();
	refereeConfig();
	can2Config();
	
	I2C_INIT();
	InitMPU6050();		
	Gyro_OFFEST();
	
	sysConfig();
	remoteConfig();

//	mainfoldConfig();
	delay_ms(1000);
	clockConfig();
}

