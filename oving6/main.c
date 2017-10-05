#include <stdio.h>
#include "io.h"
#include <pthread.h>
#include <sched.h>
#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>

void monitor(void * temp) {
	int *chan;
	chan = temp;
	rt_printf("task %i started\n", *chan);
	struct timespec time;
	time.tv_sec = 0;
	time.tv_nsec = 5000;

	while(1) {
		if (!io_read(*chan)){
			io_write(*chan, 0);
			nanosleep(&time, NULL);
			io_write(*chan, 1);
		}
		rt_task_wait_period(NULL);
	}
}

int set_cpu(int cpu_number){
	// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
	// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),&cpu);
}

void disturbance(){
	set_cpu(1);
	int i = 0;
	int sum = 0;
	while(1) {
		for(i = 0; i < 9999999; i++) {
			sum = i * 2;
		}
	}
}

void create_busywork(){
	pthread_t dist_threads[10];
	int i = 0;
	for(i = 0; i < 10; i++) {	
		pthread_create(&dist_threads[i], NULL, disturbance, NULL);
	}
}

int main() {
	io_init();
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);

	create_busywork();

	int args[3] = {1,2,3};
	RT_TASK monitors[3];
	const char *desc[3] = {"task A", "task B", "task C"};
	RTIME period = 20000;

	int i = 0;
	for(i = 0; i < 3; i++) {
		if(rt_task_create(&monitors[i], desc[i], 0, 99, T_JOINABLE|T_CPU(1)) < 0) {
			rt_printf("task creation failed\n");
		}
		if (rt_task_set_periodic(&monitors[i],TM_NOW,period) != 0) {
			rt_printf("task period failed\n");
		}
		if(rt_task_start(&monitors[i], &monitor,&args[i]) != 0) {
			rt_printf("task start failed\n");
		}
	}

	while(1) {
		;
	}
	return 0;
}



