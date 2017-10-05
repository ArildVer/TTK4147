#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <native/task.h>
#include <native/timer.h>
#include <native/mutex.h>
#include <native/sem.h>
#include <sys/mman.h>
#include <rtdk.h>

//#define assignmentA
//#define assignmentB
//#define assignmentBSem
#define assignmentC
#define assignmentCICPP


#define prioA 60
#define prioB 60

RT_SEM semA1;
RT_SEM semB1;
RT_MUTEX mutB1;
RT_MUTEX mutCA;
RT_MUTEX mutCB;


void funcA1() {
	rt_printf("Task A1 waiting on sem (low prio)\n");
	rt_sem_p(&semA1, TM_INFINITE);
	rt_printf("Task A1 passed sem (low prio)\n");
}

void funcA2() {
	rt_printf("Task A2 waiting on sem (low prio)\n");
	rt_sem_p(&semA1, TM_INFINITE);
	rt_printf("Task A2 passed sem (high prio)\n");
}

void taskA3() {
	const char *sem_names = "sem A1";
	rt_sem_create(&semA1, sem_names,0,S_PRIO);

	RT_TASK taskA1;
	RT_TASK taskA2;
	const char *descA1 = "task A1";
	const char *descA2 = "task A2";
	if(rt_task_create(&taskA1, descA1, 0, 1, T_JOINABLE|T_CPU(1)) < 0) {
		rt_printf("%i\n", (rt_task_create(&taskA1, descA1, 0, 1, T_JOINABLE|T_CPU(1)) < 0));
		rt_printf("task A1 creation failed\n");
	}
	if(rt_task_create(&taskA2, descA2, 0, 2, T_JOINABLE|T_CPU(1)) < 0) {
		rt_printf("task A2 creation failed\n");
	}
	
	rt_printf("Starting tasks\n");

	if(rt_task_start(&taskA1, &funcA1,NULL) != 0) {
		rt_printf("task A1 start failed\n");
	}
	if(rt_task_start(&taskA2, &funcA2,NULL) != 0) {
		rt_printf("task A2 start failed\n");
	}

	struct timespec time;
	time.tv_sec = 0;
	time.tv_nsec = 100000000;
	nanosleep(&time, NULL);

	rt_printf("Broadcasting\n");
	rt_sem_broadcast(&semA1);
	nanosleep(&time, NULL);

	rt_sem_delete(&semA1);
}


void print_pri(RT_TASK *task, char *s){
	struct rt_task_info temp;
	rt_task_inquire(task, &temp);
	rt_printf("b:%i c:%i ", temp.bprio, temp.cprio);
	rt_printf(s);
}

int rt_task_sleep_ms(unsigned long delay){
	return rt_task_sleep(1000*1000*delay);
}

void busy_wait_ms(unsigned long delay){
	unsigned long count = 0;
	while (count <= delay*10){
		rt_timer_spin(1000*
		100);
		count++;
	}
}


void funcBL() {
	rt_printf("Task BL started\n");
#ifdef assignmentBSem
	rt_sem_p(&semB1, TM_INFINITE);
	rt_printf("Task BL took the sem and is working\n");
#else
	rt_mutex_acquire(&mutB1, TM_INFINITE);
	rt_printf("Task BL took the mutex and is working\n");
#endif
	busy_wait_ms(3);
#ifdef assignmentBSem
	rt_printf("Task BL realising sem\n");
	rt_printf("Task BL completed\n");
	rt_sem_v(&semB1);
#else
	rt_printf("Task BL realising mutex\n");
	rt_printf("Task BL completed\n");
	rt_mutex_release(&mutB1);
#endif
}

void funcBM() {
	rt_printf("Task BM started\n");
	rt_task_sleep_ms(1);
	rt_printf("Task BM woke and is working\n");
	busy_wait_ms(5);
	rt_printf("Task BM completed\n");
}

void funcBH() {
	rt_printf("Task BH started\n");
	rt_task_sleep_ms(2);
	rt_printf("Task BH woke\n");
#ifdef assignmentBSem
	rt_sem_p(&semB1, TM_INFINITE);
	rt_printf("Task BH took the sem and is working\n");
#else
	rt_mutex_acquire(&mutB1, TM_INFINITE);
	rt_printf("Task BH took the mutex and is working\n");
#endif
	busy_wait_ms(2);
#ifdef assignmentBSem
	rt_printf("Task BH realising sem\n");
	rt_printf("Task BH completed\n");
	rt_sem_v(&semB1);
#else
	rt_printf("Task BH realising mutex\n");
	rt_printf("Task BH completed\n");
	rt_mutex_release(&mutB1);
#endif
}

void funcCL() {
	rt_printf("Task low startin\n");
	rt_mutex_acquire(&mutCA, TM_INFINITE);
	rt_printf("Task low got A\n");
#ifdef assignmentCICPP
	rt_task_set_priority(NULL, prioA);
#endif
	busy_wait_ms(3);
	rt_mutex_acquire(&mutCB, TM_INFINITE);
	rt_printf("Task low got B\n");
	busy_wait_ms(3);
	rt_mutex_release(&mutCB);
	rt_mutex_release(&mutCA);
	rt_printf("Task low released B then A\n");
#ifdef assignmentCICPP
	rt_task_set_priority(NULL, 50);
#endif
	busy_wait_ms(1);
	rt_printf("Task low completed\n");
}

void funcCH() {
	rt_printf("Task high startin\n");
	rt_task_sleep_ms(1);
	rt_mutex_acquire(&mutCB, TM_INFINITE);
	rt_printf("Task high got B\n");
#ifdef assignmentCICPP
	rt_task_set_priority(NULL, prioA);
#endif
	busy_wait_ms(1);;
	rt_mutex_acquire(&mutCA, TM_INFINITE);
	rt_printf("Task high got B\n");
	busy_wait_ms(2);;
	rt_mutex_release(&mutCA);
	rt_mutex_release(&mutCB);
	rt_printf("Task high released A then B\n");
#ifdef assignmentCICPP
	rt_task_set_priority(NULL, 51);
#endif
	busy_wait_ms(1);
	rt_printf("Task high completed\n");
}

int main() {
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);

	const char *descMain = "Task main";
	rt_task_shadow(NULL, descMain, 99, T_CPU(1));

#ifdef assignmentA
	RT_TASK taskA;
	const char *descA = {"task A"};
	
	if(rt_task_create(&taskA, descA, 0, 99, T_JOINABLE|T_CPU(1)) < 0) {
		rt_printf("task A creation failed\n");
	}
	if(rt_task_start(&taskA, &taskA3,NULL) != 0) {
		rt_printf("task A start failed\n");
	}
#endif

#ifdef assignmentB
	const char *sem_name = "sem B1";
	const char *mut_name = "mut B1";
	rt_sem_create(&semB1, sem_name,1,S_PRIO);
	rt_mutex_create(&mutB1, mut_name);
	
	RT_TASK taskB[3];
	const char *descB[3] = {"task L", "task M", "task H"};
	int i = 0;
	for(i = 0; i < 3; i++) {
		if(rt_task_create(&taskB[i], descB[i], 0, 50+i, T_JOINABLE|T_CPU(1)) < 0) {
			rt_printf("task creation failed\n");
		}
	}
	if(rt_task_start(&taskB[0], &funcBL,NULL) != 0) {
			rt_printf("task BL start failed\n");
	}
	if(rt_task_start(&taskB[1], &funcBM,NULL) != 0) {
			rt_printf("task BM start failed\n");
	}
	if(rt_task_start(&taskB[2], &funcBH,NULL) != 0) {
			rt_printf("task BH start failed\n");
	}
	
	rt_task_join(&taskB[0]);
	rt_task_join(&taskB[1]);
	rt_task_join(&taskB[2]);
	rt_printf("All tasks done, ending\n");
	rt_sem_delete(&semB1);
	rt_mutex_delete(&mutB1);
#endif
#ifdef assignmentC
	const char *mut_nameA = "mut CA";
	const char *mut_nameB = "mut CB";
	rt_mutex_create(&mutCA, mut_nameA);
	rt_mutex_create(&mutCB, mut_nameB);
	
	RT_TASK taskC[2];
	const char *descC[3] = {"task L", "task H"};
	int i = 0;
	for(i = 0; i < 2; i++) {
		if(rt_task_create(&taskC[i], descC[i], 0, 50+i, T_JOINABLE|T_CPU(1)) < 0) {
			rt_printf("task creation failed\n");
		}
	}
	if(rt_task_start(&taskC[0], &funcCL,NULL) != 0) {
			rt_printf("task BL start failed\n");
	}
	if(rt_task_start(&taskC[1], &funcCH, NULL) != 0) {
			rt_printf("task BM start failed\n");
	}
	
	rt_task_join(&taskC[0]);
	rt_task_join(&taskC[1]);
	rt_printf("All tasks done, ending\n");
	rt_mutex_delete(&mutCA);
	rt_mutex_delete(&mutCB);
#endif
	



	/*int args[3] = {1,2,3};
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
	}*/
	return 0;
}



