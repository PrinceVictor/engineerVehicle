#include "ComunicateTask.h"
_canMessage canM = {0};

void transferType(int8_t mode, _canMessage* message, int16_t* data){
	int i=0;
	switch(mode){
		case 0: 
			for(i=0; i<4; i++){
				message->canTx.StdId = 0x001;    //
				message->canTx.IDE=CAN_ID_STD;					
				message->canTx.RTR=CAN_RTR_DATA;				 
				message->canTx.DLC=8;	
				message->canTx.Data[0+i*2] = (uint8_t)(0);
				message->canTx.Data[1+i*2] = (uint8_t)(0);
			}
			break;
		case 1:{
			message->canTx.StdId = 0x002;
			message->canTx.IDE=CAN_ID_STD;					
			message->canTx.RTR=CAN_RTR_DATA;				 
			message->canTx.DLC=8;							
			for(i=0; i<4; i++){
				message->canTx.Data[0+i*2] = (uint8_t)(*(data+i) >> 8);
				message->canTx.Data[1+i*2] = (uint8_t)(*(data+i));
			}
			break;}
		case 2:{

					break;
		}
		case 3:{
					break;
		}
		default:  break;}	
}


int8_t canTrans(uint8_t flag, 
								int8_t mode,
								_canMessage* message, 
								int16_t* data)
{	
	uint8_t TransmitMailbox;//·¢ËÍÐÅÏäºÅ
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


