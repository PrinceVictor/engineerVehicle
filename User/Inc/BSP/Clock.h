#ifndef _CLOCK_H
#define _CLOCK_H
#include "sys.h"
#include "referee.h"

#define STATE_INIT 1
#define STATE_RUN 2
#define STATE_STOP 0


/*����ʹ��*/
#define COME_TO_ZERO 1

#define HOLDER_OUT 1

#define WHEEL_OUT 1



void Clock_Int_Init(void);

void Sys_Configuration(void);


uint32_t Get_Time_Micros(void);

void ControlTask(uint8_t flag);

extern int clock_cnt;
extern uint8_t Run_state ;
void PowerData_Detect(void);

extern void Diy_Data_Proccess(struct _FunctionDetect_DATA , struct _SEND_DIY_DATA *);

extern int PowerData;
extern int PowerData_flag;

#endif
