#include <stdio.h>
#include <math.h>
#include <mraa/i2c.h>
#include "LSM9DS0.h"

int main() {
	float rad_to_deg = 57.29577;
	data_t accel_data, gyro_data, mag_data;
	data_t gyro_offset,acceleration;
	int16_t temperature;
	float a_res, g_res, m_res;
	float roll,pitch, yaw;
	mraa_i2c_context accel, gyro, mag;
	accel_scale_t a_scale = A_SCALE_4G;//only generate data wihin +-4
	gyro_scale_t g_scale = G_SCALE_245DPS;//
	mag_scale_t m_scale = M_SCALE_2GS;

	//initialize sensors, set scale, and calculate resolution.
	accel = accel_init();
	set_accel_scale(accel, a_scale);	
	a_res = calc_accel_res(a_scale);
	
	gyro = gyro_init();
	set_gyro_scale(gyro, g_scale);
	g_res = calc_gyro_res(g_scale);
	
	mag = mag_init();
	set_mag_scale(mag, m_scale);
	m_res = calc_mag_res(m_scale);

	gyro_offset = calc_gyro_offset(gyro, g_res);
	acceleration = read_accel(accel, a_res);
	
	//printf("x: %f", acceleration.x);
	//printf("x: %f y: %f z: %f\n", gyro_offset.x, gyro_offset.y, gyro_offset.z);

	printf("\n\t\tAccelerometer\t\t\t||");
	printf("\t\t\tGyroscope\t\t\t||");
	printf("\t\t\tMagnetometer\t\t\t||");
	printf("\tTemperature\n");
	
	//Read the sensor data and print them.
	while(1) {
		accel_data = read_accel(accel, a_res);
		if (accel_data.z > -1.1 && accel_data.z < -.9){
			printf("up");
		}
		//printf("z direction: %f \n", accel_data.z);
		//printf("x direction: %f \n", accel_data.x);
		//printf("y direction: %f \n", accel_data.y);

		gyro_data = read_gyro(gyro, g_res);
		mag_data = read_mag(mag, m_res);
		temperature = read_temp(accel); //you can put mag as the parameter too.
		if (accel_data.z > 0)
		{
			//printf("Up, %f\n", accel_data.z);
		}
		else{
			//printf("Down, %f\n", accel_data.z);
		}
		pitch = (atan2(accel_data.x,accel_data.z)+M_PI)*rad_to_deg;
		roll = (atan2(accel_data.y,accel_data.z)+M_PI)*rad_to_deg;
		yaw = (atan2(accel_data.y,accel_data.x)+M_PI)*rad_to_deg;
		if (( pitch > 160 && pitch <200  )  && (roll>160 && roll < 200) && (yaw > 340 ||  yaw < 10))
			printf("stable ");
		else{

			if(( pitch < 170 && pitch > 0))
			{
				printf("forward");
				if (roll > 160 && roll < 180){
				}
				else{
					if (roll > 160 )
						printf("right");
					if (roll < 180 )
						printf("left");
				}
			}
			if ((pitch > 190))
			{
				printf("backwords");
				if (roll > 160 && roll < 180){
				}
				else{
					if (roll > 160 )
						printf("right");
					if (roll < 180 )
						printf("left");
				}
			}
		}
			
		printf("pitch: y %f \n",pitch);
		printf("roll:  x %f \n",roll);
		printf("yaw:   z %f \n\n", yaw);
  		//printf("X: %f\t Y: %f\t Z: %f\t||", accel_data.x, accel_data.y, accel_data.z);
  		//printf("\tX: %f\t Y: %f\t Z: %f\t||", gyro_data.x - gyro_offset.x, gyro_data.y - gyro_offset.y, gyro_data.z - gyro_offset.z);
		//printf("\tX: %f\t Y: %f\t Z: %f\t||", mag_data.x, mag_data.y, mag_data.z);
		//printf("\t%ld\n", temperature);
		usleep(100000);
	}	
	return 0;	
}
