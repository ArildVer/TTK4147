#include <sys/times.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

sem_t resource;

void access_resource(){
	int i = 0;
	for(i = 1; i<5; i++){
		sem_wait(&resource);
		printf("thread starting round %i\n\r", i);
		usleep(100000);
		printf("thread ending round %i\n\r", i);
		sem_post(&resource);
	}
}

int main(){
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	pthread_t thread4;
	pthread_t thread5;

	sem_init(&resource, 1, 3);
	
	pthread_create(&thread1, NULL, access_resource, NULL);
	pthread_create(&thread2, NULL, access_resource, NULL);
	pthread_create(&thread3, NULL, access_resource, NULL);
	pthread_create(&thread4, NULL, access_resource, NULL);
	pthread_create(&thread5, NULL, access_resource, NULL);
	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);
	pthread_join(thread5, NULL);
	printf("All threads done\n");
	return 0;
}
