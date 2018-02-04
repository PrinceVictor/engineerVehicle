#include "Clock.h"
void Body_Protect(uint8_t flag);
void Mode_Change(void);


int clock_cnt = 0;
int32_t Reset_cnt = 0;
uint8_t Diy_cnt = 0,Reset_can =1;
uint8_t Run_state = STATE_INIT;

int Load_Dis = 170;
//uint8_t Run_state = STATE_RUN;

int32_t mpu_cnt = 0;

void sysConfig(void){
	TIM_TimeBaseInitTypeDef tim;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	tim.TIM_Period = 0xFFFFFFFF;
	tim.TIM_Prescaler = 84 - 1;	 //1M ��ʱ��  
	tim.TIM_ClockDivision = TIM_CKD_DIV1;	
	tim.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_ARRPreloadConfig(TIM4, ENABLE);	
	TIM_TimeBaseInit(TIM4, &tim);
	TIM_Cmd(TIM4,ENABLE);	
}



void clockConfig(void)
{
	TIM_TimeBaseInitTypeDef tim;
	NVIC_InitTypeDef nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE); 
	
  tim.TIM_Period = 100-1; 	
	tim.TIM_Prescaler=840-1;  
	tim.TIM_CounterMode=TIM_CounterMode_Up; 
	tim.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM12,&tim);
	TIM_ITConfig(TIM12,TIM_IT_Update,ENABLE); 
	TIM_Cmd(TIM12,ENABLE); 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
	nvic.NVIC_IRQChannel=TIM8_BRK_TIM12_IRQn; 
	nvic.NVIC_IRQChannelPreemptionPriority=0; 
	nvic.NVIC_IRQChannelSubPriority=2; 
	nvic.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvic);
	
}
//

void TIM8_BRK_TIM12_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM12,TIM_IT_Update)==SET)
	{
		TIM_ClearITPendingBit(TIM12,TIM_IT_Update);
    TIM_ClearFlag(TIM12, TIM_FLAG_Update);
		
		
		clock_cnt ++;
		
		Shoot_Info.Call_cnt ++;

		RampTFB.clock_cnt++;
		RampTLF.clock_cnt++;
		Move_UD.clock_cnt++;  
		Move_LF.clock_cnt++;
		
		Diy_cnt++;
		
#if 0		 //����ʹ�� ����ʹ�ùر� ����ռ����Դ
		/****************************************/
		
		Holder_update_para();//��̨��������
		Wheel_para_update();//���Ӳ�������
		
#endif

		
		Mode_Change();//����ң��ģʽ�л�
		
		Wheel_Info_Update();
		
		Shoot_Keep();//�����̶�ʱ�䣬�������߹ر�Ħ����
		
		Detect_shoot();//Ħ���ּ��
		
		Detect_Cap();//���ּ��
		
		PowerData_Detect();
		
		Diy_Data_Proccess( Detect_Data ,&UserDefineData);
		
//���� ����ʱ����ʱ�ӵ� Ŀǰ״̬������3��Ч�����
	if(Detect_Data.Triple_Shoot_Flag == 1){
			
			if(Detect_Data.Triple_Shoot_count < 400){
				
				Detect_Data.Triple_Shoot_Invel ++ ;
				
				if(Detect_Data.Triple_Shoot_Invel > 132)
				
		{		
			Load_Motor_position_plus(Load_Dis);
			Detect_Data.Triple_Shoot_Invel = 0;
		}	
				
				

}
		if(Shoot_Info.load_command == 1){
			RC_Ctl.mouse.press_l = 1;
			Load_motor_control(1,1);
		}
}
		else if( Shoot_Info.Call_cnt >= 10 )//10ms����һ���͵��������
		{	
			Shoot_Info.Call_cnt = 0;

		
				Load_motor_control(1,HitMode);
		
}

	
		//��������
		ControlTask(Run_state);

		
		
		if( clock_cnt >= 1000)
		{
			clock_cnt = 0;
			
			PCout(1) = ~PCout(1);
		}

		if(Diy_cnt > 250){
			
			Diy_cnt  = 0;
			Detect_Data.Data_trans_Flag  = LED0;
}

}
}

uint32_t Get_Time_Micros(void)
{
	return TIM4->CNT;
}


void ControlTask(uint8_t flag)
{	
	if( mpu_cnt <4000 )//4s�ȶ�ʱ��
	{
		mpu_cnt ++;
	}
	else if(Detect_Data.Ctrl_Quit_Flag == 1){
		
		Run_state = STATE_INIT;
		
		Detect_Data.Ctrl_Quit_count++;
		
		if(Detect_Data.Ctrl_Quit_count > 10){
				
			Detect_Data.Ctrl_Quit_Flag = 0;
			
			Detect_Data.Ctrl_Quit_count = 0;

}


}
	else
	{
		Run_state = STATE_RUN;
	}

	
	switch (flag)
	{
		case STATE_INIT:{
      
			yaw_Hold_Info.angle_target = 0;
			Pitch_Hold_Info.angle_target = 0;
			yaw_Hold_Info.angle_temp = 0;
			Pitch_Hold_Info.angle_temp = 0;
			
			ComeToZero(COME_TO_ZERO);
		
			Wheel_Speed_control(0);
		
			yaw_Hold_Info.angle = 0;

		
			break;}
		
		case STATE_RUN:{
			
		
			/***����������������****/
			#if LOAD_MOTOR_DETECT_OR_NOT
			if( Shoot_Info.load_command == 1 )
			{
				Load_Motor_Fault_detet();
			}
			#endif
			/*******/
			
			
			Body_Protect(0);

		/**********����ҡ��************/
		#if 1
			if( IsComputerControl == 1 ){
			if(RC_Ctl.key.v & 0x0010)
			{
				Anaconda(1);
				
				Detect_Data.Attack_HitFlag  = 0;
	
			}
			else if((RC_Ctl.key.v & 0x4000)&&(RC_Ctl.key.v & 0x0020))
			{
				//Lets_Revolve(1);
					Reset_cnt ++;
					if( Reset_can &&( Reset_cnt > 1000)) {
					SoftReset();
					Reset_can = 0;
						
					Detect_Data.Attack_HitFlag  = 0;
					}
					
			}
			else if(RC_Ctl.key.v & 0x2000){
				
				Angle_Attack();
				
				Detect_Data.Attack_HitFlag  = 1;
			
			}
			else
			{
				Reset_can = 1;
				Reset_cnt = 0;
				Chassis_Control_Info.Chassis_angle_Target = 0;
				Detect_Data.Attack_HitFlag  = 0;
				time_ms_angle = 50;
				Anaconda(0);
//				Lets_Revolve(0);
			}
		}
			
			else
			{
				Chassis_Control_Info.Chassis_angle_Target = 0;
				Anaconda(0);
//				Lets_Revolve(0);
				Detect_Data.Attack_HitFlag  = 0;
			}
		#endif
			
			if( LaserAndPrep[TANK_SERIAL_NUMBER-1][10] == 1 )//֧������
			{
			Hit_Task(1);
			}
			else//��֧������
			{
				Holder_Control(ENABLE , &Pitch_para, &Pitch_Hold_Info);//pitch
				Holder_Control(ENABLE , &Yaw_para  , &yaw_Hold_Info);//YAW
				//Holder_Motor_output(HOLDER_OUT);
        Holder_Motor_output(1);
				Chassis_Control(1);
				Chassis_Remote_Dispack(1);
				Wheel_Speed_control(1);	
			}
		

			break;}
		
			case STATE_STOP:{
				Holder_Motor_output(0);//���Ϊ0�����͸���
				Wheel_Speed_control(0);
			break;}
		
		default:{
			
			break;}
		
	};
}

void Body_Protect(uint8_t flag)
{
	static float pro = 0.03f;
	
	if( flag == 0 )
	{
		return;
	}
	
	//����
	if( Pitch_Hold_Info.can_angle >= PITCH_POSITIVE_LIMIT_ANGLE )
	{
		Pitch_Hold_Info.angle_target -= pro;
	}
	
	//����
	else if(  Pitch_Hold_Info.can_angle <= PITCH_NEGTIVE_LIMIT_ANGLE)
	{
		Pitch_Hold_Info.angle_target += pro;
	}
	
}

void Mode_Change(void)
{
		/***********????************/
	if( RC_Ctl.rc.s1 == 2)
	{
		IsComputerControl = 1;
	}
	else 
	{
		IsComputerControl = 0;
	}
		
}
	/****��λ ���� ȷ������ʱ���ϴ��ɹ�
			 ʮλ ��������Ƿ�����
			 ��λ Ħ����
			 ǧλ �������ݽ���

	***/
void Diy_Data_Proccess(struct _FunctionDetect_DATA  Diy_data , struct _SEND_DIY_DATA  * Send_data){
	

	
	Diy_data.Shoot_motor_Flag = Shoot_Info.load_command;
	
	Send_data->data3 = Diy_data.Shoot_motor_Flag * 10  + \
	
	Diy_data.Hit_Flag * 100 + Diy_data.Hit_Identify_Flag * 1000 + 10000 + Diy_data.Data_trans_Flag ;
	
	if(Diy_data.MyColor == 0){
		
		if((Diy_data.BigRune0status == 0x03)||(Diy_data.BigRune1status == 0x03)){
			
		Send_data->data1 = 1;

}
		else {
			
		Send_data->data1 = 0;
		
}
		if(Diy_data.BlueAirPortSta == 0x03)  Send_data->data2 = 1;
		else Send_data->data2 = 0;
}
	else if(Diy_data.MyColor == 1){
		
			if((Diy_data.BigRune0status == 0x02)||(Diy_data.BigRune1status == 0x02)){
				
			Send_data->data1 = 1;
}
		else {
			
			Send_data->data1 = 0;

}

		if(Diy_data.RedAirPortSta == 0x03)  Send_data->data2 = 1;
		else Send_data->data2 = 0;


}	

	if(RC_Ctl.key.v &0x0100){

		Detect_Data.Triple_Shoot_count ++;
		
		Detect_Data.Triple_Shoot_Flag = 1;
		
		if(Detect_Data.Triple_Shoot_count > 1000){
			
			Detect_Data.Triple_Shoot_count = 0;
			Detect_Data.Triple_Shoot_Invel = 0;
}

}
else{
		
		Detect_Data.Triple_Shoot_Flag = 0;
		Detect_Data.Triple_Shoot_count = 0;
		Detect_Data.Triple_Shoot_Invel = 0;

}
}


void PowerData_Detect(){
			static int32_t Power_cnt =0;
	if((Judgment_01_data.power_W >= 200)||(Judgment_01_data.power_W < 0)){
		Detect_Data.PowerData_flag = 0;
}
	else if(Detect_Data.Last_PowerData!= Detect_Data.PowerData )
	{
		Detect_Data.PowerData_flag = 1;
		Detect_Data.Last_PowerData = Detect_Data.PowerData;
		Power_cnt = 0;
	}
		else {
			Power_cnt ++;
				
}
		if(Power_cnt > 100){
		Detect_Data.PowerData_flag = 0;
			Power_cnt = 0;
	}




}
