#include "main.h"

int main(void)
{     
	float speed_main[4] = {1200,1200,1200,1200};
	RC_Ctl.rc.ch0 = 1024;
	RC_Ctl.rc.ch1 = 1024;
	RC_Ctl.rc.ch2 = 1024;
	RC_Ctl.rc.ch3 = 1024;
	
	RC_Ctl.rc.ChangeFlag = 0;
	IsComputerControl = 1;//����ģʽ
	//IsComputerControl = 0;//�ֱ�ģʽ
	
	ALLTASk_BooT();  //���й��ܵĳ�ʼ��������
	
	while(1)
	{
		
		IMU_update();   //����IMU�����Ǽ��ٶȼƲ���
			
		/***********����************/
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
