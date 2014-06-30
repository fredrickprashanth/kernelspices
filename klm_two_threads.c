#include<linux/module.h>
#include<linux/kthread.h>
#include<linux/kernel.h>
#include<linux/sched.h>
#include<linux/delay.h>

MODULE_LICENSE("GPL");

static void my_udelay(int msecs){
	unsigned long i,j;
	for(i=0;i<msecs;i++)
		for(j=0;j<1000;j++)
		{
			udelay(1);
		}
	

}
int klm_thread_rr(void *data){
	struct sched_param param = { .sched_priority = MAX_RT_PRIO -1};
	struct task_struct *other_thread = *((struct task_struct **)data);
	sched_setscheduler(current, SCHED_FIFO, &param);
	set_current_state(TASK_INTERRUPTIBLE);
	while(!kthread_should_stop()){
		my_udelay(5);
		wake_up_process(other_thread);
		schedule();
		set_current_state(TASK_INTERRUPTIBLE);
	}
	__set_current_state(TASK_RUNNING);
	return 0;
	
}

int klm_thread_blip(void *data){
	struct sched_param param = { .sched_priority = MAX_RT_PRIO -1};
	sched_setscheduler(current, SCHED_FIFO, &param);
	while(!kthread_should_stop()){
		my_udelay(5);
		while(schedule_timeout_interruptible(5));
	}
	return 0;
	
}
#define NR_RR_THS 3

static struct task_struct *rr_threads[NR_RR_THS];

static void start_rr_threads(void){
	int i=0;
	for(i=0; i< NR_RR_THS; i++)
	{
		rr_threads[i] = kthread_create(klm_thread_rr, rr_threads + (i + 1)%NR_RR_THS, "klm_thread_rr_%d", i);
		if(!rr_threads[i]){
			pr_debug("Could not create klm_thread_rr_%d\n", i);
			continue;
		}
		kthread_bind(rr_threads[i], 0);
	}
	if(rr_threads[0])
		wake_up_process(rr_threads[0]);
}
static void stop_threads(struct task_struct** threads_array, int size){
	int i;
	for(i=0; i<size; i++)
		if(threads_array[i])
			kthread_stop(threads_array[i]);

}
static void stop_rr_threads(void){

	stop_threads(rr_threads, NR_RR_THS);
}
#define NR_BLIP_THS 1
static struct task_struct *blip_threads[NR_BLIP_THS];
static void start_blip_threads(void){
	int i;
	for(i=0; i<NR_BLIP_THS; i++)
	{
		blip_threads[i] = kthread_create(klm_thread_blip, NULL, "klm_thread_blip_%d", i);
		if(!blip_threads[i]){
			pr_debug("Could not create klm_thread_blip_%d\n", i);
			continue;
		}

		kthread_bind(blip_threads[i], 0);
		wake_up_process(blip_threads[i]);
	}
	
}
static void stop_blip_threads(void){

	stop_threads(blip_threads, NR_BLIP_THS);
}
static int klm_two_threads_init(void){

	start_rr_threads();
	start_blip_threads();
	return 0;
}

void klm_two_threads_exit(void){
	stop_rr_threads();
	stop_blip_threads();
}
module_init(klm_two_threads_init);
module_exit(klm_two_threads_exit);

