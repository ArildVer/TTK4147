#include <stdio.h>
#include <native/task.h>
#include <native/timer.h>

void asd(void) {
	printf("Task running");
}

int main() {
	mlockall(MCL_CURRENT|MCL_FUTURE);
	RT_TASK test;
	const char *desc = "Test task";
	if(rt_task_create(&test, &desc, 0, 50, T_JOINABLE) < 0) {
		printf("task creation failed");
	}
	rt_task_start(&test, &asd,NULL);
	while(1) {
		;
	}
	return 0;
}
