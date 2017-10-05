#include "io.h"
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>

int set_cpu(int cpu_number){
	// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
	// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),&cpu);
}

void timespec_add_us(struct timespec *t, long us){
	// add microseconds to timespecs nanosecond counter
	t->tv_nsec += us*1000;
	// if wrapping nanosecond counter, increment second counter
	if (t->tv_nsec > 1000000000){
		t->tv_nsec = t->tv_nsec-1000000000;
		t->tv_sec += 1;
	}
}

void monitor_periodic(void *temp) {
	set_cpu(1);
	int *chan;
	chan = temp;
	struct timespec tim;
	tim.tv_sec = 0;
	tim.tv_nsec = 5000;
	struct timespec next;
	clock_gettime(CLOCK_REALTIME, &next);
	while (1) {
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
		if (!io_read(*chan)){
			io_write(*chan, 0);
			nanosleep(&tim, NULL);
			io_write(*chan, 1);
		}
		timespec_add_us(&next, 1);
	}
}

void monitor_busy(void *temp) {
	set_cpu(1);
	int *chan;
	chan = temp;
	struct timespec time;
	time.tv_sec = 0;
	time.tv_nsec = 5000;
	while (1) {
		if (!io_read(*chan)){
			io_write(*chan, 0);
			nanosleep(&time, NULL);
			io_write(*chan, 1);
		}
	}
}

void distrurbance(){
	set_cpu(1);
	int i = 0;
	int sum = 0;
	while(1) {
		for(i = 0; i < 9999999; i++) {
			sum = i * 2;
		}
	}
}

int main() {
	io_init();
	pthread_t tests[3];
	int args[3] = {1,2,3};

	pthread_t dist_threads[10];

	int i = 0;
	for(i = 0; i < 3; i++) {	
		pthread_create(&tests[i], NULL, monitor_periodic, &args[i]);
	}

	for(i = 0; i < 10; i++) {	
		pthread_create(&dist_threads[i], NULL, distrurbance, NULL);
	}

	printf("running\n\r");
	pthread_join(tests[0], NULL);
	pthread_join(tests[1], NULL);
	pthread_join(tests[2], NULL);
	return 0;
}
