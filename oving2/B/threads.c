#include <stdio.h>
#include <pthread.h>


int glob_var = 0;

void func(void){
	int loc_var = 0;
	int i = 0;
	for (i = 0; i<10000; i++) {
		glob_var += 1;
		loc_var += 1;
	}
	printf("Global: %i \n\rLocal: %i\n\r", glob_var, loc_var);
}	

int main(void){
	
	
	pthread_t thread1;
	pthread_t thread2;
	
	pthread_create(&thread1, NULL, func, NULL);
	pthread_create(&thread2, NULL, func, NULL);
	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	
	return 1;
}
