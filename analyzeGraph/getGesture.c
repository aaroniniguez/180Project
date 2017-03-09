#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
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
	return 0;
}

