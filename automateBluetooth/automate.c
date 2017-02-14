#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	FILE *pp;
	pp = popen("exec bash automateBluetooth.sh", "r");
	long ret;
	char *ptr;
	if (pp != NULL) {
	  while (1) {
		char *line;
	      	char buf[100000];
	    	line = fgets(buf, sizeof buf, pp);
	  	if (line == NULL) break;
		if (strstr(line, "RSSI return value:") != NULL){
			//print out the negative RSSI values
			char * pch;
			pch = strtok (line," ");
			char* a = pch;
			while (pch != NULL)
			{
			a=pch;
			//printf ("%d\n",atoi(pch));
			pch = strtok (NULL, " ,.");

			}
			printf ("%d\n",atoi(a));
		}
		//if (line[0] == 'd') printf("%s", line); /* line includes '\n' */
	  }
      	  pclose(pp);
	}
	return 0;
}

