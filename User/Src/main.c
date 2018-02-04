#include "main.h"

int main(void)
{     
	float speed_main[4] = {1200,1200,1200,1200};
	RC_Ctl.rc.ch0 = 1024;
	RC_Ctl.rc.ch1 = 1024;
	RC_Ctl.rc.ch2 = 1024;
	RC_Ctl.rc.ch3 = 1024;
	
	RC_Ctl.rc.ChangeFlag = 0;
	IsComputerControl = 1;//电脑模式
	//IsComputerControl = 0;//手柄模式
	
	ALLTASk_BooT();  //所有功能的初始化和启动
	
	while(1)
	{
		
		IMU_update();   //更新IMU陀螺仪加速度计参数
			
		/***********调试************/
		cnt++;
	
		#if 1   
		if(cnt > 50 ){
		SendtoReferee(Send_data);
		New_Send_Data( Send_data , 21);
    cnt = 0;		
}
		#endif

	}
	
}
