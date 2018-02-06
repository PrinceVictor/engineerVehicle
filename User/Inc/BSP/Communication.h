#ifndef __COMMUNICATION_H_
#define __COMMUNICATION_H_

#include "MyFunc.h"
#include "ComunicateTask.h"
#include "ControlTask.h"

void can2Config(void);
void remoteConfig(void);

#define BSP_USART1_DMA_RX_BUF_LEN	30u

#define YAW_SENSITY 0.050f		// Û±Íyaw÷·¡È√Ù∂»
#define PITCH_SENSITY 0.03f	// Û±Ípitch÷·¡È√Ù∂»

//extern unsigned char sbus_rx_buffer[18];


#endif

