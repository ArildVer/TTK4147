#include <sys/times.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

pthread_mutex_t forks[5];

void eat(void *temp){
	int *me;
	me = temp;
	printf("Philo %i initializing\n\r", *me);
	int left_fork = *me;
	int right_fork = *me+1;
	if(*me == 4) {
		right_fork = 0;
	}
	printf("Philo %i have left fork: %i, right fork: %i \n\r", *me, left_fork, right_fork);
	
	while(1) {
		pthread_mutex_lock(&forks[left_fork]);
		printf("Philo %i picked up left fork %i\n\r",*me,left_fork);
		pthread_mutex_lock(&forks[right_fork]);
		printf("Philo %i picked up right fork %i\n\r",*me,right_fork);

		pthread_mutex_unlock(&forks[left_fork]);
		printf("Philo %i dropped up left fork %i\n\r",*me,left_fork);
		pthread_mutex_unlock(&forks[right_fork]);
		printf("Philo %i dropped up right fork %i\n\r",*me,right_fork);
	}
}

int main(void){
	pthread_t philo[5];
	int args[5] = {0,1,2,3,4};
	int i = 0;
	for(i = 0; i<5; i++) {
		printf("Init fork %i\n\r", i);
		pthread_mutex_init(&forks[i], NULL);
	}

	for(i = 0; i<5; i++) {
		printf("Init philo %i\n\r", i);
		pthread_create(&philo[i], NULL, eat, &args[i]);
	}
	i = 4;
	
	int j = 0;
	for(j = 0; j<5; j++) {
		pthread_join(philo[0], NULL);
	}
	return 1;
}
