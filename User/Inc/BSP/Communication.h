#ifndef __COMMUNICATION_H_
#define __COMMUNICATION_H_

#include "MyFunc.h"

#define YAW_SENSITY 0.050f		//���yaw��������
#define PITCH_SENSITY 0.03f	//���pitch��������

#define LEFT_LIMINT_ANGLE 40.0f //��̨���Ƕ�����
#define RIGHT_LIMINT_ANGLE -40.0f//��̨�Ҳ�Ƕ�����

#define PITCH_NEGTIVE_LIMIT_ANGLE -9.0f		//��̨�Ϸ��Ƕ�����
#define PITCH_POSITIVE_LIMIT_ANGLE 23.0f		//��̨�·��Ƕ�����


#define LEFT_CHESSIS_LIMIT 25.0f //�����������
#define RIGHT_CHESSIS_LIMIT -25.0f//�����ұ�����



//#define YAW_INITIAL_VAL  6896.0f //YAW ����������ֵ
//#define PITCH_INITIAL_VAL  1500.0f //PITCH ����������ֵ
#define K_CODE_2_ANGLE  0.04132f  //������ֵ���㵽�Ƕ�ϵ��

#define K_ANGLESPEED_2_ANGLE 0.00003272f //�����ǻ��ֻ�ýǶ�ϵ��



typedef struct
{
	struct
	{
		uint16_t ch0;
		uint16_t ch1;
		uint16_t ch2;
		uint16_t ch3;
		uint8_t s1;
		uint8_t s2;
		uint8_t ChangeFlag;
	}rc;
	struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
		uint8_t press_l;
		uint8_t press_r;
	}mouse;
	struct
	{
		uint16_t v;
	}key;
}RC_Ctl_t;

extern RC_Ctl_t RC_Ctl;


extern unsigned char sbus_rx_buffer[18];
extern uint8_t IsComputerControl;

void Remote_Config(void);

uint8_t In_rangex( float low , float high , float var );




#endif

