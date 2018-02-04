#include "MyFunc.h"

//��Сֵ�Ƚ�
float my_min( float a , float b )
{
	if(a>b)	return b;
	else	return a;
}

//���ֵ�Ƚ�
float my_max( float a , float b )
{
	if(a>b)	return a;
	else	return b;
}

//�޷����
float Amplitude_Limiting(uint8_t flag , float input , float High , float Low)
{
	float result;
	
	if( flag == 0 )	return input;
	
	if( input > High)	result = High;
	else if( input < Low )	result = Low;
	else	result = input;
	
	return result;
}
	

//�ֶ�PID
float Subsection_PID(uint8_t flag , float raw , uint8_t num , float * K , float * Scale)
{
	uint8_t cnt = 0;
	
	if( flag == 0 )
	{
		return raw;
	}
	//
	for( cnt = 0;cnt<num-1;cnt++ )
	{
		if( ( abs(raw) < (*(Scale + cnt + 1)) )&&( abs(raw) >= (*(Scale + cnt)) ) )
		{
			break;
		}
	}
	
	
	return ( (*(K+cnt)) * raw );
 
}


//֮ǰ��Χ������������ֱ�������Թ�ʽ   û�в���
float Subsection_PID_v2(uint8_t flag,float Delta,float scale1,float scale2,float k1,float k2)
{
  float delta_k,scale_delta,delta_scale;
  if(flag==0)
  {
    return Delta;
  }
  if(( abs(Delta) >= scale1 ) && ( abs(Delta) <= scale2 ))
  {
    delta_scale = abs(scale2-scale1);
    scale_delta = ( Delta-scale1 ) / delta_scale;
    delta_k = abs(k2-k1);
    return (Delta*( k1 + scale_delta*delta_k ));
  }else
  {
    return k2 * Delta;
  }
}
