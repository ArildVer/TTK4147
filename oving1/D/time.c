#include <sys/times.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void busy_wait_delay(int seconds)
{
	int i, dummy;
	int tps = sysconf(_SC_CLK_TCK);
	clock_t start;
	struct tms exec_time;
	times(&exec_time);
	start = exec_time.tms_utime;
	while( (exec_time.tms_utime - start) < (seconds * tps))
	{
		for(i=0; i<1000; i++)
		{
			dummy = i;
		}
		times(&exec_time);
	}
}

void* print_busy(){
	printf("I am busy\n");
	busy_wait_delay(5);
	printf("I am busy\n");
}

void* print_sleep(){
	printf("I am asleep\n");
	sleep(5);
	printf("I am asleep\n");
}

int main(){
	pthread_t waitThread1;
	pthread_t waitThread2;
	
	pthread_create(&waitThread1, NULL, print_sleep, NULL);
	pthread_create(&waitThread2, NULL, print_sleep, NULL);
	
	pthread_join(waitThread1, NULL);
	pthread_join(waitThread2, NULL);
	printf("Hello from main!\n");
	return 0;
}
