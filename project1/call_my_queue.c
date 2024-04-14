#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>

#define enqueue_syscall 335
#define dequeue_syscall 336

int main(){
    int r, i=0;
    int num;
    srand(time(NULL));
    for(i=0; i<3; i++){
	num = rand() % 100;
	r=syscall(enqueue_syscall, num);
	if(r == -1){
	    printf("Duplicated.\n");
	}
	else if(r!=0){
	    printf("Enqueued: %d\n",r);
	}
    }
    printf("----------------\n");
    for(i=0; i<3; i++){
        r=syscall(dequeue_syscall);
        if(r!=0){
            printf("Dequeued: %d\n",r);
        }
    }
    return 0;
}