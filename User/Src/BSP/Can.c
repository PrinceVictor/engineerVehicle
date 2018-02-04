#include "Can.h"

CanRxMsg RxMessage205;
CanRxMsg RxMessage206;
CanRxMsg RxMessage;

/***********************************************************************************
  * @函数描述：  CAN1通信初始化，波特率1MHz. CAN1_TX----PA12,CAN1_RX-----PA11
  * @入口参数：  无.
  * @返回值  :   无.
***********************************************************************************/



void CAN2_TX_IRQHandler(void) //CAN TX
{
  if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET)    //if transmit mailbox is empty 
  {
	   CAN_ClearITPendingBit(CAN2,CAN_IT_TME);   
  }
}


/***************************************************************************************
  * @函数描述：  CAN1接收中断函数,依据ID将数据送入不同全局变量
  * @入口参数：  无.
  * @返回值  :   无.
****************************************************************************************/
void CAN2_RX0_IRQHandler(void)
{
  
	
  if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
		CAN_Receive(CAN2,CAN_FIFO0,&RxMessage);
		
		if( RxMessage.StdId == 0x206 )//Pitch
		{
	
			Pitch_Hold_Info.can_angle_raw = RxMessage.Data[0] * 256 + RxMessage.Data[1];
			//RxMessage206 = RxMessage;
		}
		else if(  RxMessage.StdId == 0x205 )//Yaw
		{
			Chassis_Control_Info.Chassis_angle_raw = RxMessage.Data[0] * 256 + RxMessage.Data[1];
			yaw_Hold_Info.can_angle_raw = Chassis_Control_Info.Chassis_angle_raw;
			//RxMessage205 = RxMessage;
		}
		
		else if( (RxMessage.StdId >= 0x201)&&( RxMessage.StdId <= 0x204 ) )
		{
			Four_Wheel_Info.position[RxMessage.StdId - 0x201] = RxMessage.Data[0]*256 +RxMessage.Data[1];
			
			Four_Wheel_Info.speed_raw[RxMessage.StdId - 0x201] = RxMessage.Data[2]*256 +RxMessage.Data[3];
		}
		

   }
}





