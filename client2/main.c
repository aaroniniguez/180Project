#include "client.h"
#include "thermistor.h"
#include <signal.h>
#include <mraa.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>
#include <mraa/i2c.h>
#include "LSM9DS0.h"
#include <stdbool.h>

static volatile int run_flag = 1;

void do_when_interrupted()
{
	run_flag = 0;
}

int client_handle_connection(int client_socket_fd)
{
	int n;
	FILE *pp;
	FILE *fpz, *fpz1;
	char buffer[256];
	float a_res;
	accel_scale_t a_scale = A_SCALE_4G;//only generate data wihin +-4
	a_res = calc_accel_res(a_scale);
	mraa_i2c_context accel;
	data_t accel_data;
	accel = accel_init();
	set_accel_scale(accel, a_scale);	
	accel_data = read_accel(accel, a_res);
	float rad_to_deg = 57.29577;
	float roll,pitch, yaw;
      	int rl, fb;
	time_t startTime = time(NULL);
	time_t countTime = time(NULL);
	char *line;
	char buf[100000];
	memset(buffer, 0, 256);
	while (run_flag) {
		memset(buffer, 0, 256);
		buffer[0]='g';
   //printf("5\n");
		n = write(client_socket_fd, buffer, strlen(buffer));
		//printf("6\n");
		if (n < 0) {
			return client_error("ERROR writing to socket");
		}
		memset(buffer, 0, 256);
//printf("7\n");
		n = read(client_socket_fd, buffer, 255);
		if (n < 0) {
			return client_error("ERROR reading from socket");
		}
   //printf("8\n");
  if(buffer[0]=='m')
  {
	printf("if statement %s\n", buffer);
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
memset(buffer, 0, 256);
}
else if(buffer[0]=='1')
{
	memset(buffer, 0, 256);
	printf("else statement %s\n", buffer);
	bool print1=0;
	bool print2=0;
	bool print3=0;
	countTime = time(NULL);
	printf("\nDo an action in 3 seconds...\n");
	while(time(NULL) - countTime <4){
	if(time(NULL)-countTime == 3 && !print3)
	{
		//printf("3...\n");
		print3=1;

	}
	else if(time(NULL) - countTime ==2 && !print2)
	{
		//printf("2...\n");
		print2=1;
	}
	else if(time(NULL)-countTime == 1 && !print1)
	{
		//printf("1..\n");
		print1=1;
	}
	}
	printf("NOW..\n");
	
	fpz = fopen("straight_linez.txt","w+");
	fpz1 = fopen("straight_linez.csv","w+");
	fprintf(fpz,"%s,%s,%s\n","Xacc", "Yacc", "Zacc");
	fprintf(fpz1,"%s,%s,%s\n","Xacc", "Yacc", "Zacc");
	startTime = time(NULL);
	while(time(NULL) - startTime < 3)
	{
	accel_data = read_accel(accel, a_res);
	float gZ=accel_data.z+1-0.04;
	float gX=accel_data.x+0.12;
	float gY=accel_data.y-0.1+0.06;
	fprintf(fpz, "%f,%f,%f\n", gX,gY,gZ);
	fprintf(fpz1, "%f,%f,%f\n", gX,gY,gZ);
	}	
	fclose(fpz);
	fclose(fpz1);
	pp = popen("exec python getGesture.py", "r");
	if (pp != NULL) {
		//line is either 0 or 1
		line = fgets(buf, sizeof buf, pp);
		if (line == NULL){
			line = "0,0";
			printf("null char\n");
		}
		printf("%s", line);
		sprintf(buffer, "m,%s", line);
		printf("sending: %s\n", buffer);
		n = write(client_socket_fd, buffer, strlen(buffer));
		printf("number of bits sent:%i\n",n);
		//printf("1\n");
		pclose(pp);
		//printf("2\n");
	}
//printf("3\n");
	memset(buffer, 0, 256);
	//printf("4/n");
}
else if(strlen(buffer) != 0){
	printf("not m or 1: %s\n",buffer);
}
	
  //printf("4\n");
  }
	printf("closed socket");
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
