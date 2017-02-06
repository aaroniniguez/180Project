#include <stdio.h>
#include <mraa/i2c.h>
#include "LSM9DS0.h"

int main(){
	mraa_i2c_context accel, gyro, mag;

	accel = accel_init();
	gyro = gyro_init();
	mag = mag_init();

	return 0;
}
