#include "Imu.h"
#include "Clock.h"
#include "math.h" 
#include "PrepareData.h"
#include "6050.h"
volatile uint32_t lastUpdate, now; // �������ڼ��� ��λ us
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
struct _angle angle;
float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error


float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
    float norm;
//    float hx, hy, hz, bx, bz;
    float vx, vy, vz;//, wx, wy, wz;
    float ex, ey, ez,halfT;
    float tempq0,tempq1,tempq2,tempq3;

    float q0q0 = q0*q0;
    float q0q1 = q0*q1;
    float q0q2 = q0*q2;
    float q0q3 = q0*q3;
    float q1q1 = q1*q1;
    float q1q2 = q1*q2;
    float q1q3 = q1*q3;
    float q2q2 = q2*q2;   
    float q2q3 = q2*q3;
    float q3q3 = q3*q3;   

		
	  now = Get_Time_Micros();  //��ȡʱ�� ��λ��us   
    if(now<lastUpdate)
    {
    //halfT =  ((float)(now + (0xffffffff- lastUpdate)) / 2000000.0f);   //  uint 0.5s
    }
    else	
    {
       halfT =  ((float)(now - lastUpdate) / 2000000.0f);
    }
    lastUpdate = now;	//����ʱ��

    //������ƽ�����㷨
    norm = invSqrt(ax*ax + ay*ay + az*az);       
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;
//    //�ѼӼƵ���ά����ת�ɵ�λ������
//    norm = invSqrt(mx*mx + my*my + mz*mz);          
//    mx = mx * norm;
//    my = my * norm;
//    mz = mz * norm; 
//    // compute reference direction of flux
//    hx = 2.0f*mx*(0.5f - q2q2 - q3q3) + 2.0f*my*(q1q2 - q0q3) + 2.0f*mz*(q1q3 + q0q2);
//    hy = 2.0f*mx*(q1q2 + q0q3) + 2.0f*my*(0.5f - q1q1 - q3q3) + 2.0f*mz*(q2q3 - q0q1);
//    hz = 2.0f*mx*(q1q3 - q0q2) + 2.0f*my*(q2q3 + q0q1) + 2.0f*mz*(0.5f - q1q1 - q2q2);         
//    bx = sqrt((hx*hx) + (hy*hy));
//    bz = hz; 
    // estimated direction of gravity and flux (v and w)
    vx = 2.0f*(q1q3 - q0q2);
    vy = 2.0f*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;
//    wx = 2.0f*bx*(0.5f - q2q2 - q3q3) + 2.0f*bz*(q1q3 - q0q2);
//    wy = 2.0f*bx*(q1q2 - q0q3) + 2.0f*bz*(q0q1 + q2q3);
//    wz = 2.0f*bx*(q0q2 + q1q3) + 2.0f*bz*(0.5f - q1q1 - q2q2);  
    // error is sum of cross product between reference direction of fields and direction measured by sensors
    ex = (ay*vz - az*vy);// + (my*wz - mz*wy);
    ey = (az*vx - ax*vz);// + (mz*wx - mx*wz);
    ez = (ax*vy - ay*vx);// + (mx*wy - my*wx);

    if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
    {
			exInt = exInt + ex * Kii * halfT;
			eyInt = eyInt + ey * Kii * halfT;	
			ezInt = ezInt + ez * Kii * halfT;
			// �ò���������PI����������ƫ
			gx = gx + Kp*ex + exInt;
			gy = gy + Kp*ey + eyInt;
			gz = gz + Kp*ez + ezInt;
    }
    // ��Ԫ��΢�ַ���
    tempq0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    tempq1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    tempq2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    tempq3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

    // ��Ԫ���淶��
    norm = invSqrt(tempq0*tempq0 + tempq1*tempq1 + tempq2*tempq2 + tempq3*tempq3);
    q0 = tempq0 * norm;
    q1 = tempq1 * norm;
    q2 = tempq2 * norm;
    q3 = tempq3 * norm;
		
		angle.yaw= -atan2(2 * q1 * q2 + 2 * q0* q3, -2 * q2*q2 - 2 * q3 * q3 + 1)*RtA; // yaw        -pi----pi
    angle.pitch= -asin(-2 * q1 * q3 + 2 * q0 * q2)*RtA; // pitch    -pi/2    --- pi/2 
    angle.roll= atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* RtA; // roll       -pi-----pi 

		

		
		
		
		
	
}