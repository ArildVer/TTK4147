#include <stdio.h>
#include <unistd.h>


int glob_var = 0;

int main(void){
	int loc_var = 0;
	vfork();
	int i = 0;
	for (i = 0; i<10000; i++) {
		glob_var += 1;
		loc_var += 1;
	}
	printf("Global: %i \n\rLocal: %i\n\r", glob_var, loc_var);
	return 1;
}
