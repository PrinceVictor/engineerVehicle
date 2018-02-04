#ifndef __MYFUNC_H__
#define __MYFUNC_H__

#include "stm32f4xx.h"
#include "sys.h" 
#include "delay.h"
#include <stdio.h>
#include "math.h"

//����ֵ
#define abs(x) ((x)>0? (x):(-(x)))

//�����Сֵ
float Amplitude_Limiting(uint8_t flag , float input , float High , float Low);

//2016 �ֶ�PID
float Subsection_PID(uint8_t flag , float raw , uint8_t num , float * K , float * Scale);

//2017
float Subsection_PID_v2(uint8_t flag,float Delta,float scale1,float scale2,float k1,float k2);

//��Сֵ
float my_min( float  , float  );
//���ֵ
float my_max( float  , float  );



#endif

