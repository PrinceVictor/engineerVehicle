#ifndef __IMU_H_
#define __IMU_H_

#define Kp 1.0f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Kii 0.01f                     // integral gain governs rate of convergence of gyroscope biases

float invSqrt(float x);

#define RtA 		57.324841f		//  180/3.1415  �Ƕ��� ת��Ϊ������	

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);

struct _angle{
        float pitch;
				float roll;
        float yaw;
				float val;
};
extern struct _angle angle;

#endif
				

