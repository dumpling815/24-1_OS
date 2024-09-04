/* 2024 Spring COSE341 Operating System*/
/* Project 2*/
/* Park Minuk */

#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/sched.h>
#include <linux/slab.h>
#define queue_size 101
#define IDLE -1

pid_t waiting_queue[queue_size] = {0,};
pid_t now = IDLE;
int length = 0;
typedef struct _job_t{
	pid_t pid;
	int jobTime;
}job_t;

int ku_push(pid_t pid){
	// fail -> return 1
	// success -> return 0
	if(length >= queue_size -1) return 1;	
	waiting_queue[length] = pid;
	length++;
	return 0;
}
int ku_pop(void){
	// success -> return first element's pid in Q
	// Fail -> return IDLE
	int i;
	pid_t res;
	if(length <=0) return IDLE;
	res = waiting_queue[0];
        for(i=1; i<length; i++){
		waiting_queue[i-1] = waiting_queue[i];
	}
	waiting_queue[length-1] = 0;
	length--;
	return res;
}
int ku_is_empty(void){
	// empty -> return 0
	// not empty -> return 1
	if(length == 0) return 0;
	else return 1;
}
int ku_is_new_id(pid_t pid){
	// already exist in waiting Q -> return 1
	// new id -> return 0
	int i;
	for(i=0; i<length; i++){
		if(waiting_queue[i] == pid) return 1;
	}
	return 0;
}



SYSCALL_DEFINE1(os2024_pid_print, char*, name){
	pid_t pid = current -> pid;
	printk("Process name: %s pid: %d\n",name,pid);
	return 0;
}

SYSCALL_DEFINE2(ku_cpu_fcfs, char*, name, int, jobTime){
	// store pid of current process as pid_t type
	job_t newJob = {current->pid, jobTime};
	
	// register the process if virtual CPU is idle
	if(now==IDLE) now = newJob.pid;
	
	//If the process that sent the request is currently using virtual CPU
	if(now == newJob.pid){
		//If the job has finished
		if(jobTime == 0){
			printk("Process Finished: %s\n",name);
			//if queue is empty, virtual CPU becomes idle
			if(ku_is_empty()==0) now = IDLE;
			//if not, get next process from queue
			else now = ku_pop();
		}
		else printk("Working: %s\n", name);
		// request accepted
		return 0;
	}
	else{
		//if the request is not from currently handling process
		if(ku_is_new_id(newJob.pid)==0) ku_push(newJob.pid);
		printk("Working Denied:%s \n",name);
		//request rejected
		return 1;
	}	
}
SYSCALL_DEFINE2(ku_cpu_srtf, char*, name, int, jobTime){
	job_t newJob = {current->pid, jobTime};
	if(now == IDLE) now = newJob.pid;

	if(now == newJob.pid){
		if(newJob.jobTime == 0){
			printk("Process %s finished.\n",name);
			if(ku_is_empty()==0) now = IDLE;
			else now = ku_pop();
		}
		else printk("Working %s.\n",name);
		return 0;
	}	
	else{
		if(ku_is_new_id(newJob.pid)==0) ku_push(newJob.pid);
		printk("Working Denied.\n");
		return 1;
	}
}
SYSCALL_DEFINE2(ku_cpu_rr, char*, name, int, jobTime){
	job_t newJob = {current->pid, jobTime};
	if(now == IDLE) now = newJob.pid;
	if(now == newJob.pid){
		if(newJob.jobTime == 0){
			printk("Process %s finished.\n",name);
			if(ku_is_empty()==0) now = IDLE;
			else now = ku_pop();
		}
		else printk("Working %s.\n",name);
		return 0;
	}
	else{
		if(ku_is_new_id(newJob.pid)==0) ku_push(newJob.pid);
		printk("Working Denied.\n");
		return 1;
	}

}