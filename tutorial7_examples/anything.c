#include <stdbool.h>
#include <stdio.h>
int main(){
	bool a;
	for ( a = 128; a > 0; a >>= 1){
		printf("hi");
	}
	return 0;
}
