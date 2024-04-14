/* 2024 Spring COSE341 Operating System */
/* Project 1 */
/* Park Minuk 2020320054*/

#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>

#define queue_size 4

int my_queue[queue_size] = {-1,-1,-1,-1}; 
int saturation = 0;

SYSCALL_DEFINE1(os2024_enqueue, int, a){
    int access = 1;
    int i=0;
    for(i=0; i<saturation; i++){
    	if(a == my_queue[i]){
	    access = 0;
	}
    }
    if (access == 1){
        my_queue[saturation] = a;
        saturation++;
    }
    else{
	    return -1;
    }
    printk("[System call] os2024_enqueue(); -----\n");
    printk("Queue Front -------------\n");
    for(i=0; i<saturation; i++){
        printk("%d\n",my_queue[i]);
    }
    printk("Queue Rear  -------------\n");
    return my_queue[saturation-1];
}

SYSCALL_DEFINE0(os2024_dequeue){
    int ret = my_queue[0];
    int i=0;
    saturation--;
    for(i=0; i<saturation; i++){
	    my_queue[i]=my_queue[i+1];
    }
    my_queue[saturation]=-1;
    printk("[System call] os2024_dequeue(); -----\n");
    printk("Queue Front -------------\n");
    for(i=0; i<saturation; i++){
        printk("%d\n",my_queue[i]);
    }
    printk("Queue Rear  -------------\n");
    return ret;
}