#include "client.h"
#include "thermistor.h"
#include <signal.h>
#include <mraa.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>
#include <mraa/i2c.h>
#include "LSM9DS0.h"

static volatile int run_flag = 1;

void do_when_interrupted()
{
	run_flag = 0;
}

int client_handle_connection(int client_socket_fd)
{
	int n;
	char buffer[256];
 float rad_to_deg = 57.29577;
	data_t accel_data;
	float a_res;
	float roll,pitch, yaw;
      	int rl, fb;
	mraa_i2c_context accel;
	accel_scale_t a_scale = A_SCALE_4G;//only generate data wihin +-4

	//initialize sensors, set scale, and calculate resolution.
	accel = accel_init();
	set_accel_scale(accel, a_scale);	
	a_res = calc_accel_res(a_scale);

//	struct timeval tv;
//	double sec_since_epoch;

	memset(buffer, 0, 256);
	//sprintf(buffer, "time (epoch),temperature/C");

	//n = write(client_socket_fd, buffer, strlen(buffer));
	//if (n < 0) {
	//	return client_error("ERROR writing to socket");
	//}

	//memset(buffer, 0, 256);

	//n = read(client_socket_fd, buffer, 255);
	//if (n < 0) {
	//	return client_error("ERROR reading from socket");
	//}

	//printf("msg from server: %s\n", buffer);

	while (run_flag) {
//		gettimeofday(&tv, NULL);
//		sec_since_epoch = (double) tv.tv_sec + (double) tv.tv_usec/1000000.0;
accel_data = read_accel(accel, a_res);
		if (accel_data.z > -1.1 && accel_data.z < -.9){
		//	printf("up");
		}
		//printf("z direction: %f \n", accel_data.z);
		//printf("x direction: %f \n", accel_data.x);
		//printf("y direction: %f \n", accel_data.y);
//you can put mag as the parameter too.
		if (accel_data.z > 0)
		{
			//printf("Up, %f\n", accel_data.z);
		}
		else{
			//printf("Down, %f\n", accel_data.z);
		}
		 rl = 63;
		 fb = 63;
		 
		pitch = (atan2(accel_data.x,accel_data.z)+M_PI)*rad_to_deg;
		roll = (atan2(accel_data.y,accel_data.z)+M_PI)*rad_to_deg;
		yaw = (atan2(accel_data.y,accel_data.x)+M_PI)*rad_to_deg;
		if (( pitch > 340 ||  pitch <15  )  && (roll> 340 || roll < 15))
		{ //printf("stable ");
		  }
		else{

			if(( pitch < 120 && pitch > 20))
			{
			//	printf("forward");
				fb =126+20+  (pitch-120)*63/100;
				if (fb>126)
					fb=126;
			}
			if ((pitch > 300 ))
			{
			//	printf("backwords");
				fb = (pitch -300)*63/50-20;
				if(fb<0)
					fb=0;
			}
			if (roll > 10  && roll < 350){
				if (roll > 10 && roll<250)
				{//printf("right \n");
					 rl = 63+30+(roll-10)*63/240;
					 if (rl>126)
						 rl=126;
				}

				if (roll > 250 && roll<350 )
				{//	printf("left \n");
					
					rl = (roll-250)*63/90 - 20;
					if(rl<0)
						rl=0;
				}
				}

		}
//		printf("%i , %i \n",fb,rl);			
		memset(buffer, 0, 256);
		sprintf(buffer, "m,%i,%i", fb,rl);

		n = write(client_socket_fd, buffer, strlen(buffer));
		if (n < 0) {
			return client_error("ERROR writing to socket");
		}
		memset(buffer, 0, 256);

		n = read(client_socket_fd, buffer, 255);
		if (n < 0) {
			return client_error("ERROR reading from socket");
		}

		printf("msg from server: %s\n", buffer);
//		usleep(500000);
	}
	close(client_socket_fd);
	return 1;
}

int main(int argc, char *argv[])
{
//	THERMISTOR *thermistor;
	int client_socket_fd;

	signal(SIGINT, do_when_interrupted);

//	mraa_init();

//	thermistor = thermistor_init(0, 'C');
	client_socket_fd = client_init(argc, argv, 10);
	if (client_socket_fd < 0) {
		perror("ERROR connecting to server");
		return -1;
	}

	client_handle_connection(client_socket_fd);

	return 0;
}
