#include "CanTask.h"

void transferType(int8_t mode,_canMessage* message, int16_t* data){
	switch(mode){
		case 0: 
			for(int8_t i=0; i<4; i++){
				message->canTx.Data[0+i*2] = (uint8_t)(0);
				message->canTx.Data[1+i*2] = (uint8_t)(0);
			}
			break;
		case 1:
			message->canTx.StdId = 0x200;
			message->canTx.IDE=CAN_ID_STD;					
			message->canTx.RTR=CAN_RTR_DATA;				 
			message->canTx.DLC=8;							
			for(int8_t i=0; i<4; i++){
				message->canTx.Data[0+i*2] = (uint8_t)(*(data+i) >> 8);
				message->canTx.Data[1+i*2] = (uint8_t)(*(data+i));
			}
			break;
		default:  return; break;}	
}

int8_t canTransfer(uint8_t flag, CanTxMsg* message)
{	
  uint8_t TransmitMailbox;//∑¢ÀÕ–≈œ‰∫≈
	
	TransmitMailbox = CAN_Transmit(CAN2,&TxMessage);
	t=0;
	while((CAN_TransmitStatus(CAN2,TransmitMailbox)!=CANTXOK)&&(t<500))
	{
		t++;
	}

	return 1;

}
