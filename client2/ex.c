#include<stdio.h>
#include<string.h>
#include <mraa/i2c.h>
#include "LSM9DS0.h"
#include <math.h>

int main()
{
	FILE *fpz, *fpz1;
	fpz = fopen("straight_linez.csv","w+");
	fpz1 = fopen("straight_linez.txt","w+");
	data_t accel_data;
	float a_res;
	mraa_i2c_context accel, gyro, mag;
	accel_scale_t a_scale = A_SCALE_4G;
	accel = accel_init();
	set_accel_scale(accel, a_scale);	
	a_res = calc_accel_res(a_scale);
	fprintf(fpz,"%s,%s,%s\n","Xacc", "Yacc", "Zacc");
	fprintf(fpz1,"%s,%s,%s\n","Xacc", "Yacc", "Zacc");
time_t startTime = time(NULL);
while(time(NULL) - startTime < 3)
{
	accel_data = read_accel(accel, a_res);
	float gZ=accel_data.z+1-0.04;
	float gX=accel_data.x+0.12;
	float gY=accel_data.y-0.1+0.06;
 	fprintf(fpz, "%f,%f,%f\n", gX,gY,gZ);
 	fprintf(fpz1, "%f,%f,%f\n", gX,gY,gZ);
 //printf("%f\n", gY);
	
//usleep(10000); 
	}	
 
fclose(fpz);
fclose(fpz1);

FILE *pp;
pp = popen("exec python getGesture.py", "r");
long ret;
char *ptr;
if (pp != NULL) {
	char *line;
	char buf[100000];
	//line is either 0 or 1
	line = fgets(buf, sizeof buf, pp);
	printf("%s", line);
	pclose(pp);
}

}
