#include "ComunicateTask.h"

_RampTime RampTFB ;
_RampTime RampTLF ;
_RampTime Move_UD ;
_RampTime Move_LF ;

uint16_t NORMAL_FORWARD_BACK_SPEED 	= 720;   //640
uint16_t NORMAL_LEFT_RIGHT_SPEED   	=	620;    //600
const int32_t HIGH_FORWARD_BACK_SPEED 		=	650;
const int32_t HIGH_LEFT_RIGHT_SPEED   		=	650;
int forward_back_speed = 0;
int left_right_speed = 0;
_RC_Ctl remote = {0};
_canMessage canM = {0};

int8_t commuiModeChange(int8_t* flag, _RC_Ctl* data, _chassis* chassis){
	if( data->rc.s1 == 2)	*flag = 0;
	else *flag = 1;
	if(flag) remoteControl(data , chassis); 
	else computerControl(data , chassis);	
	return 1;
}

int8_t readRemote(_RC_Ctl* data, unsigned char * buffer){

	data->rc.ch0 = (buffer[0]| (buffer[1] << 8)) & 0x07ff; //!< Channel 0
	data->rc.ch1 = ((buffer[1] >> 3) | (buffer[2] << 5)) & 0x07ff; //!< Channel 1
	data->rc.ch2 = ((buffer[2] >> 6) | (buffer[3] << 2) |(buffer[4] << 10)) & 0x07ff; //!< Channel 2
	data->rc.ch3 = ((buffer[4] >> 1) | (buffer[5] << 7)) & 0x07ff; //!< Channel 3
	data->rc.s1 = ((buffer[5] >> 4)& 0x000C) >> 2; //!< Switch left
	data->rc.s2 = ((buffer[5] >> 4)& 0x0003); //!< Switch right
	data->mouse.x = -(buffer[6] | (buffer[7] << 8)); //!< Mouse X axis
	data->mouse.y = buffer[8] | (buffer[9] << 8); //!< Mouse Y axis
	data->mouse.z = buffer[10] | (buffer[11] << 8); //!< Mouse Z axis
	data->mouse.press_l = buffer[12]; //!< Mouse Left Is Press 
	data->mouse.press_r = buffer[13]; //!< Mouse Right Is Press 
	data->key.v = buffer[14] | (buffer[15] << 8); //!< KeyBoard value
	return 1;
}

float RampCal(_RampTime *RampT)
{
	if(RampT->isSameKey ==1 )
	{
		RampT->now = (*RampT).clock_cnt;	
	}
	else {
		RampT->lasttime = (*RampT).clock_cnt;
		RampT->now = (*RampT).clock_cnt;	
	}
	RampT->count = RampT->now  - RampT->lasttime;//按键持续的时间
	RampT->out = (float)RampT->count / 600;

	if(RampT->out > 1) RampT->out = 1;

	else {
		RampT->cnt = 0;
}
		if(RampT->cnt > 200) RampT->cnt =200;
	return RampT->out;
	
}

int8_t remoteControl(_RC_Ctl* data, _chassis* chassis){
	
	forward_back_speed =  NORMAL_FORWARD_BACK_SPEED;
	left_right_speed = NORMAL_LEFT_RIGHT_SPEED;

	if(abs( data->rc.ch1-1024)> 50){
			Move_UD.isSameKey = 1;
		if(( data->rc.ch1-1024)> 50){
		forward_back_speed = forward_back_speed*RampCal(&Move_UD);
		}
		else if(( data->rc.ch1-1024)< -50){
		forward_back_speed = -forward_back_speed*RampCal(&Move_UD);
		}
	 }
	else{
			Move_UD.isSameKey = 0;
			forward_back_speed = 0; 
			RampCal(&Move_UD);
	 }
	if(abs( data->rc.ch0-1024)> 50){
			Move_LF.isSameKey = 1;
		if(( data->rc.ch0-1024)> 50){
		left_right_speed = left_right_speed*RampCal(&Move_LF);
		}
		else if(( data->rc.ch0-1024)< -50){
		left_right_speed = -left_right_speed*RampCal(&Move_LF);
		}
	 }
	else{
			Move_LF.isSameKey = 0;
		 left_right_speed = 0;
		 RampCal(&Move_LF);
	 }
	
	chassis->Fb = forward_back_speed;
	chassis->Lr = left_right_speed;
	
	chassis->yaw.temp = \
		chassis->yaw.target +( 1024 - data->rc.ch2 ) * 0.006f;//测试视觉时，注释此举
	
	amplitudeLimiting(1 , chassis->yaw.temp , 60);
	
	chassis->yaw.target = chassis->yaw.temp ;
	 
	return 1;
}


int8_t computerControl(_RC_Ctl* data, _chassis* chassis){

	if(data->key.v & 0x10)
	{
		forward_back_speed =  HIGH_FORWARD_BACK_SPEED;
		left_right_speed = HIGH_LEFT_RIGHT_SPEED;
	}
	else
	{
		forward_back_speed =  NORMAL_FORWARD_BACK_SPEED;
		left_right_speed = NORMAL_LEFT_RIGHT_SPEED;
	}
	
	if(data->key.v & 0x01)//  w
	{
		if(RampTFB.lastKey & 0x01){
			RampTFB.isSameKey =1;
		}
		else {
			RampTFB.isSameKey =0;
		}
		forward_back_speed = forward_back_speed*RampCal(&RampTFB);
	}
	else if(data->key.v & 0x02) //   s
	{
		if(RampTFB.lastKey & 0x02){
			RampTFB.isSameKey =1;
		}
		else {
			RampTFB.isSameKey =0;
		}
		forward_back_speed = -forward_back_speed*RampCal(&RampTFB);
	}
	else
	{
		forward_back_speed=0;
		RampTFB.isSameKey =0;
	}
	
	if(data->key.v & 0x04)  //    d
	{
		if(RampTLF.lastKey & 0x04){
			RampTLF.isSameKey =1;
		}
		else {
			RampTLF.isSameKey =0;
		}
		left_right_speed = left_right_speed*RampCal(&RampTLF);//缓慢加速的过程，按键时间超过500ms则原数输出
	}
	else if(data->key.v & 0x08)    //   a
	{
		if(RampTLF.lastKey & 0x08){
			RampTLF.isSameKey =1;
		}
		else {
			RampTLF.isSameKey =0;
		}
		left_right_speed = -left_right_speed*RampCal(&RampTLF);
	}
	else
	{
		left_right_speed=0;
		RampTLF.isSameKey =0;
	}
	RampTFB.lastKey =data->key.v;
	RampTLF.lastKey =data->key.v;

	chassis->Fb = forward_back_speed;
	chassis->Lr = -left_right_speed;
					
		/*yaw 向左*/
		if( data->mouse.x > 0 )
			{
				if(chassis->yaw.angle < LEFT_LIMINT_ANGLE)
				{
					chassis->yaw.temp = chassis->yaw.target + ( data->mouse.x ) * YAW_SENSITY ;
				}
			}
			else if( data->mouse.x < 0 )
			{
				if(chassis->yaw.angle > RIGHT_LIMINT_ANGLE)
				{
					chassis->yaw.temp = chassis->yaw.target + ( data->mouse.x ) * YAW_SENSITY ;
				}
			}
			chassis->yaw.target = chassis->yaw.temp;		
	return 1;
}

void transferType(int8_t mode, _canMessage* message, int16_t* data){
	int i=0;
	switch(mode){
		case 0: 
			for(i=0; i<4; i++){
				message->canTx.Data[0+i*2] = (uint8_t)(0);
				message->canTx.Data[1+i*2] = (uint8_t)(0);
			}
			break;
		case 1:{
			message->canTx.StdId = 0x200;
			message->canTx.IDE=CAN_ID_STD;					
			message->canTx.RTR=CAN_RTR_DATA;				 
			message->canTx.DLC=8;							
			for(i=0; i<4; i++){
				message->canTx.Data[0+i*2] = (uint8_t)(*(data+i) >> 8);
				message->canTx.Data[1+i*2] = (uint8_t)(*(data+i));
			}
			break;}
		case 2:{
			if( (message->canRx.StdId >= 0x201)&&( message->canRx.StdId <= 0x204 ) ){
			//speed
					data[message->canRx.StdId - 0x201] \
							= message->canRx.Data[2]*256 +message->canRx.Data[3];
					break;
		}
		}
		case 3:{
			if( (message->canRx.StdId >= 0x201)&&( message->canRx.StdId <= 0x204 ) ){	//position
					data[message->canRx.StdId - 0x201] \
							= message->canRx.Data[0]*256 +message->canRx.Data[1];  
					break;
		}
		}
		default:  break;}	
}


int8_t canTrans(uint8_t flag, 
								int8_t mode,
								_canMessage* message, 
								int16_t* data)
{	
	uint8_t TransmitMailbox;//发送信箱号
	int16_t t;
	if(!flag) return 0;
		else{
		transferType(mode, message, data);
		TransmitMailbox = CAN_Transmit(CAN2, &message->canTx);
		t=0;
		while((CAN_TransmitStatus(CAN2,TransmitMailbox)!=CANTXOK)&&(t<0xff))
		{
			t++;
		}
		return 1;
}
}


