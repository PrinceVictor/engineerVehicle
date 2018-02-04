#include "BootTask.h"


void ALLTASk_BooT(void){
	
	Shoot_PWM_Configuration(); 
	Shoot_Fire(1000);
	
	Chessis_Info_Init();													//���̲�����ʼ��
	Holder_update_para();													//��̨������ʼ��
	
	delay_init(168);
	
		if( LaserAndPrep[TANK_SERIAL_NUMBER-1][10] == 1 )//֧������
	{
		Hit_flag_init();//
		CalculateKey();
	}	
	
	LED_Init();
  Referee_Configuration(); 											//����ϵͳ��ʼ��
	Mainfold_Receive_Configuration(); 						//������ճ�ʼ��
	Load_Motor_PWM_Init();												//�������
	Shoot_Info_Init();														//���������ʼ��
	
	Wheel_Info_Init();														//���ӱջ�������ʼ��
	Wheel_para_update();													//���Ӳ�������
	
	
	CAN2_Configuration();
	Holder_Motor_output(0);												//���Ϊ0�����͸���
	Wheel_Speed_control(0);
	
	delay_ms(1000);

	Encoder_Init();																//�������������
	
	Bullet_Cap_Conf();														//���ֶ����ʼ��
	
	Remote_Config();															//Զ��ң�س�ʼ��
	
	I2C_INIT();
	InitMPU6050();																//6050��ʼ��
	Gyro_OFFEST();
	
	Sys_Configuration();													//1ms ���ж�
	delay_ms(1000);
	
	Laser_Init();																	//����ʳ�ʼ��
	
	Clock_Int_Init();															//1ms
}


/*******************************************************************************
* Function Name  : IMU_update
* Description    : IMU PARAMOUNT UPDATE
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/

void IMU_update(void){
	
		PrepareForIMU(1);
		IMUupdate(sensor.gyro.radian.x,
							sensor.gyro.radian.y,
						  sensor.gyro.radian.z,
							sensor.acc.averag.x,
							sensor.acc.averag.y,
							sensor.acc.averag.z);					
		Attitude_update();
		

}

