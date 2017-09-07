#include <sys/times.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

int running = 1;

int var1 = 0;
int var2 = 0;

pthread_mutex_t mutual_exclusion;

void increment(void){
	while (running){
		pthread_mutex_lock(&mutual_exclusion);
		var1 = var1 + 1;
		var2 = var1;
		pthread_mutex_unlock(&mutual_exclusion);
	}
}

void func(){
	int i = 0;
	for(i = 1; i<20; i++){
		pthread_mutex_lock(&mutual_exclusion);
		printf("Number 1 is %i, number 2 is %i\n\r", var1, var2);
		pthread_mutex_unlock(&mutual_exclusion);
		usleep(100000);
	}
	running = 0;
}

int main(void){
	pthread_mutex_init(&mutual_exclusion, NULL);

	pthread_t thread1;
	pthread_t thread2;
	
	pthread_create(&thread1, NULL, increment, NULL);
	pthread_create(&thread2, NULL, func, NULL);
	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
}
