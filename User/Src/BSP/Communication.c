#include "Communication.h"
void Computer_Control(void);
void Remote_Control(void);
uint8_t IsComputerControl =  1;//����ģʽ  1������ģʽ 0���ֱ�ģʽ


unsigned char sbus_rx_buffer[18];
RC_Ctl_t RC_Ctl;

#define BSP_USART1_DMA_RX_BUF_LEN	30u

void can2Config(void){
	CAN_InitTypeDef        canInit;
	CAN_FilterInitTypeDef  canFilter;
	GPIO_InitTypeDef       gpio;
	NVIC_InitTypeDef       nvic;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2); 

	gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12 ;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &gpio);

	nvic.NVIC_IRQChannel = CAN2_RX0_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	CAN_DeInit(CAN2);
	CAN_StructInit(&canInit);

	canInit.CAN_TTCM = DISABLE;
	canInit.CAN_ABOM = DISABLE;    
	canInit.CAN_AWUM = DISABLE;    
	canInit.CAN_NART = DISABLE;    
	canInit.CAN_RFLM = DISABLE;    
	canInit.CAN_TXFP = DISABLE;     
	canInit.CAN_Mode = CAN_Mode_Normal; 
	canInit.CAN_SJW  = CAN_SJW_1tq;
	canInit.CAN_BS2 = CAN_BS2_4tq;
	canInit.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
	CAN_Init(CAN2, &canInit);
	
	canFilter.CAN_FilterNumber=14;
	canFilter.CAN_FilterMode=CAN_FilterMode_IdMask;
	canFilter.CAN_FilterScale=CAN_FilterScale_32bit;
	canFilter.CAN_FilterIdHigh=0x0000;
	canFilter.CAN_FilterIdLow=0x0000;
	canFilter.CAN_FilterMaskIdHigh=0x0000;
	canFilter.CAN_FilterMaskIdLow=0x0000;
	canFilter.CAN_FilterFIFOAssignment=0;//the message which pass the filter save in fifo0
	canFilter.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&canFilter);
	
	CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);

}

void RemoteConfig(void){
	USART_InitTypeDef USART1_InitStructure;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;
	DMA_InitTypeDef   dma;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7 ,GPIO_AF_USART1);
	
	gpio.GPIO_Pin = GPIO_Pin_7 ;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&gpio);
		
	USART_DeInit(USART1);
	USART1_InitStructure.USART_BaudRate = 100000;   //D-BUS 100K baudrate
	USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART1_InitStructure.USART_StopBits = USART_StopBits_1;
	USART1_InitStructure.USART_Parity = USART_Parity_Even;
	USART1_InitStructure.USART_Mode = USART_Mode_Rx;
	USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART1_InitStructure);
			
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	
	nvic.NVIC_IRQChannel = USART1_IRQn;                          
	nvic.NVIC_IRQChannelPreemptionPriority = 0;   //pre-emption priority 
	nvic.NVIC_IRQChannelSubPriority = 0;		    //subpriority 
	nvic.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&nvic);	
	
	DMA_DeInit(DMA2_Stream5);
	DMA_StructInit(&dma);
	dma.DMA_Channel= DMA_Channel_4;
	dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
	dma.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;
	dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
	dma.DMA_BufferSize = 30;
	dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma.DMA_Mode = DMA_Mode_Circular;
	dma.DMA_Priority = DMA_Priority_VeryHigh;
	dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
	dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream5,&dma);
	
	DMA_Cmd(DMA2_Stream5,ENABLE);
	
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);        //usart rx idle interrupt  enabled
	USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void)
{	
	static uint32_t this_time_rx_len = 0;
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		//clear the idle pending flag 
		(void)USART1->SR;
		(void)USART1->DR;

		//Target is Memory0
		if(DMA_GetCurrentMemoryTarget(DMA2_Stream5) == 0)
		{
			DMA_Cmd(DMA2_Stream5, DISABLE);
			this_time_rx_len = BSP_USART1_DMA_RX_BUF_LEN - DMA_GetCurrDataCounter(DMA2_Stream5);
			DMA2_Stream2->NDTR = (uint16_t)BSP_USART1_DMA_RX_BUF_LEN;     //relocate the dma memory pointer to the beginning position
			DMA2_Stream2->CR |= (uint32_t)(DMA_SxCR_CT);                  //enable the current selected memory is Memory 1
			DMA_Cmd(DMA2_Stream5, ENABLE);
      if(this_time_rx_len == 18)
			{
				RC_Ctl.rc.ch0 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff; //!< Channel 0
				RC_Ctl.rc.ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff; //!< Channel 1
				RC_Ctl.rc.ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) |(sbus_rx_buffer[4] << 10)) & 0x07ff; //!< Channel 2
				RC_Ctl.rc.ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff; //!< Channel 3
				RC_Ctl.rc.s1 = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2; //!< Switch left
				RC_Ctl.rc.s2 = ((sbus_rx_buffer[5] >> 4)& 0x0003); //!< Switch right
				RC_Ctl.mouse.x = -(sbus_rx_buffer[6] | (sbus_rx_buffer[7] << 8)); //!< Mouse X axis
				RC_Ctl.mouse.y = sbus_rx_buffer[8] | (sbus_rx_buffer[9] << 8); //!< Mouse Y axis
				RC_Ctl.mouse.z = sbus_rx_buffer[10] | (sbus_rx_buffer[11] << 8); //!< Mouse Z axis
				RC_Ctl.mouse.press_l = sbus_rx_buffer[12]; //!< Mouse Left Is Press ?
				RC_Ctl.mouse.press_r = sbus_rx_buffer[13]; //!< Mouse Right Is Press ?
				RC_Ctl.key.v = sbus_rx_buffer[14] | (sbus_rx_buffer[15] << 8); //!< KeyBoard value
					
				if(IsComputerControl)
				{
					Computer_Control();					
				}
			
				/*�ֶ�ģʽ*/
				else 
				{
					
					Remote_Control();
			}

		}
		
		else 
		{

}
		
				
}       
}
}


void Computer_Control(void){
		float angle_plus;
		float pitch_plus;

	
		ComputerControl();
	
		yaw_Hold_Info.can_angle = Chassis_Control_Info.Chassis_angle;
					
		/*yaw ����*/
		if( RC_Ctl.mouse.x > 0 )
			{
				if(yaw_Hold_Info.can_angle < LEFT_LIMINT_ANGLE)
				{
					yaw_Hold_Info.angle_temp = yaw_Hold_Info.angle_target + ( RC_Ctl.mouse.x ) * YAW_SENSITY ;
				}
			}
			else if( RC_Ctl.mouse.x < 0 )
			{
				if(yaw_Hold_Info.can_angle > RIGHT_LIMINT_ANGLE)
				{
					yaw_Hold_Info.angle_temp = yaw_Hold_Info.angle_target + ( RC_Ctl.mouse.x ) * YAW_SENSITY ;
				}
			}

			yaw_Hold_Info.angle_target = yaw_Hold_Info.angle_temp ;
	
								
			/**********����pitch********/
			Pitch_Hold_Info.angle_temp = Pitch_Hold_Info.angle_target - ( RC_Ctl.mouse.y ) * PITCH_SENSITY;
			
			if( Pitch_Hold_Info.angle_temp > PITCH_POSITIVE_LIMIT_ANGLE)
			{
				Pitch_Hold_Info.angle_temp = PITCH_POSITIVE_LIMIT_ANGLE;
			}
			else if( Pitch_Hold_Info.angle_temp < PITCH_NEGTIVE_LIMIT_ANGLE)
			{
				Pitch_Hold_Info.angle_temp = PITCH_NEGTIVE_LIMIT_ANGLE;
			}
			Pitch_Hold_Info.angle_target = Pitch_Hold_Info.angle_temp ;

				pitch_plus = -( RC_Ctl.mouse.y ) * PITCH_SENSITY ;
	
			
			
			if( pitch_plus > 0 )
			{
				angle_plus = my_min(  pitch_plus, (PITCH_POSITIVE_LIMIT_ANGLE - Pitch_Hold_Info.can_angle) );
				
				if( angle_plus < 0 )
				{
					angle_plus = 0;
				}	
				Pitch_Hold_Info.angle_temp = Pitch_Hold_Info.angle_target + angle_plus;
				
			}
			
			
			else if( pitch_plus < 0 )
			{
				angle_plus = my_max( pitch_plus , (PITCH_NEGTIVE_LIMIT_ANGLE - Pitch_Hold_Info.can_angle) );
				
				if( angle_plus > 0 )
				{
					angle_plus = 0;
				}	
				
				Pitch_Hold_Info.angle_temp = Pitch_Hold_Info.angle_target + angle_plus;
			}
			
			Pitch_Hold_Info.angle_target = Pitch_Hold_Info.angle_temp ;					

}

void Remote_Control(void){
				//1.����õ�������̨�Ƕ�
				Remote_Imitate();
	
					Pitch_Hold_Info.angle_temp = Pitch_Hold_Info.angle_target + (-1024 + RC_Ctl.rc.ch3 ) * 0.0035f;//pitch ң��������ת���ɽǶ�
		
				
					if( Pitch_Hold_Info.angle_temp > PITCH_POSITIVE_LIMIT_ANGLE)
					{
						Pitch_Hold_Info.angle_temp = PITCH_POSITIVE_LIMIT_ANGLE;
					}
					else if( Pitch_Hold_Info.angle_temp < PITCH_NEGTIVE_LIMIT_ANGLE)
					{
						Pitch_Hold_Info.angle_temp = PITCH_NEGTIVE_LIMIT_ANGLE;
					}
	
					Pitch_Hold_Info.angle_target = Pitch_Hold_Info.angle_temp ;	
					
					yaw_Hold_Info.angle_temp = yaw_Hold_Info.angle_target + ( 1024 - RC_Ctl.rc.ch2 ) * 0.006f;//�����Ӿ�ʱ��ע�ʹ˾�
					
					//Amplitude_Limiting(ENABLE , yaw_Hold_Info.angle_temp , 60 , -60);
					
					yaw_Hold_Info.angle_target = yaw_Hold_Info.angle_temp ;

}



