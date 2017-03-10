#include<stdio.h>
#include<string.h>
#include <mraa/i2c.h>
#include "LSM9DS0.h"
#include <math.h>

int main()
{
	FILE *fpz;
	fpz = fopen("../straight_linez.txt","w+");
  
 data_t accel_data, gyro_data, mag_data;
//	data_t gyro_offset;
//	int16_t temperature;
	float a_res, g_res, m_res;
	mraa_i2c_context accel, gyro, mag;
	accel_scale_t a_scale = A_SCALE_4G;
//	gyro_scale_t g_scale = G_SCALE_245DPS;
//	mag_scale_t m_scale = M_SCALE_2GS;

	//initialize sensors, set scale, and calculate resolution.
	accel = accel_init();
	set_accel_scale(accel, a_scale);	
	a_res = calc_accel_res(a_scale);
fprintf(fpz,"%s,%s,%s\n","Xacc", "Yacc", "Zacc");

time_t startTime = time();
while(time() - startTime < 10)
{
	printf("hi counting");
	
}
 while(1) {
		accel_data = read_accel(accel, a_res);
		
	float gZ=accel_data.z+1-0.04;
	float gX=accel_data.x+0.12;
	float gY=accel_data.y-0.1+0.06;
 
 fprintf(fpz, "%f,%f,%f\n", gX,gY,gZ);
 //printf("%f\n", gY);
	
//usleep(10000); 
	}	
 
 fclose(fpz);

}
