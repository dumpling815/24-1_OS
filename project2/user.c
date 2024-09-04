#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define KU_CPU 337 // define system call

int main(int argc, char** argv){
	int jobTime;
	int delayTime;
	char name[4];
	int wait = 0;
	int response[2] = {0,0};
	
	if(argc < 4){
		printf("\nInsufficient Arguments..\n");
	}
	/* 
    first argument: job time (second)
    second argument: delay time before execution (second)
    third argument: process name
    fourth argument: priority (Not necessary)
    */
   	jobTime = atoi(argv[1]);
  	delayTime = atoi(argv[2]);
  	strcpy(name,argv[3]);

   //wait for 'delayTime' seconds before execution
   	sleep(delayTime);
   	printf("\nProcess %s : I will use CPU by %ds.\n",name,jobTime);
   	jobTime *= 10; // execute system call in every 0.1 second.

   //continue requesting the system call as long as the jobTime remains.
   	while(jobTime >= 0){
    // if request is rejected, increase wait time
    	if(!syscall(KU_CPU,name,jobTime)){ 
			jobTime--;
			printf("%s Worked!\n",name);
			if(response[0] == 0) response[0] = 1;
		}
    	else {
			if(response[0] == 0) response[1] ++;
			printf("%s Waiting..\n",name);
			wait ++;
		}
    	usleep(100000); // delay 0.1 second
   	}
   	// response need to be implemented.
   	printf("\n Process %s : Finish! My response time is %ds and total wait time is %ds.\n",name, response[1]/10 ,(wait+5)/10);

   	return 0;
}