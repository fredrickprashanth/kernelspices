#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/spinlock.h>
#include <linux/cpu.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/mm.h>

MODULE_LICENSE("Dual GPL/BSD");

static DEFINE_SPINLOCK(bug_lock);

static DEFINE_PER_CPU(struct task_struct *, klm_test_task);

#define MAX_RELAX_SECS 240
static void inline make_some_delay(void){
	int loops=loops_per_jiffy*HZ*MAX_RELAX_SECS;
	while(loops--)
		__delay(1);

	/*
	int i,j;
	for(i=0;i<MAX_RELAX_SECS;i++)
		for(j=0;j<1000*1000;j++);//10^6 usec is 1 sec
			udelay(1);
	*/
}
int klm_test_kthread(void *data){

	unsigned long flags;
	int cpu=(int)data;	

	set_user_nice(current, 0);
	printk("Started klm_test_kthread on CPU/%d\n", cpu);
	/*	
	for( ;!kthread_should_stop(); ) {
		printk("Going to sleep klm_test_kthread/%d\n", cpu);
		while(schedule_timeout_interruptible(MAX_RELAX_SECS*HZ));
		printk("Waked up klm_test_kthread/%d\n", cpu);
		if(kthread_should_stop())
			break;
	}
	*/		
	while(!kthread_should_stop()) {
		printk("test_task/%d try bug_lock\n", cpu);
		spin_lock_irqsave(&bug_lock, flags);
		printk("test_task/%d got bug_lock\nnow spining for %d secs\n", cpu, MAX_RELAX_SECS);
		make_some_delay();
		spin_unlock_irqrestore(&bug_lock, flags);
	
	}
	/*
	while(1){

		spin_lock(&bug_lock);
		printk("test_task/%d got bug_lock\nnow spining for %d secs\n", cpu, MAX_RELAX_SECS);
		for(i=0;i<MAX_RELAX_SECS;i++)
			for(j=0;j<1000*1000;j++);//10^6 usec is 1 sec
				udelay(1);
		spin_unlock(&bug_lock);
	}
	*/
	
	printk("Stopping klm_test_kthread/%d\n", cpu);
	return 0;
	
}

static int klm_test_init(void){

	int cpu;
	struct task_struct *tsk;
	int this_cpu=raw_smp_processor_id();
	printk("Loading klm_test..\n");
	for_each_online_cpu(cpu){
		
		if(cpu == this_cpu){

			printk("Skipping CPU/%d\n", cpu);
			continue;
		}

		printk("Starting klm_test_kthread on CPU/%d\n", cpu);
		tsk=kthread_create(klm_test_kthread, (void *)cpu, "test_task/%d", cpu);
		if(!tsk){
			printk("Could not create kthread test_task/%d\n", cpu);
			continue;
		}
			
		per_cpu(klm_test_task, cpu) = tsk;
		kthread_bind(tsk, cpu);			
		wake_up_process(tsk);
	
	}
	return 0;
	
	

}

static void klm_test_exit(void){
	int cpu;
	struct task_struct *tsk;
	for_each_online_cpu(cpu){
		tsk=per_cpu(klm_test_task, cpu);
		if(!tsk)
		{
			wake_up_process(tsk);
			kthread_stop(tsk);
		}
		spin_lock(&bug_lock);//if i am able to get the lock both threads have stopped, if order is enforced
		spin_unlock(&bug_lock);
	}
	printk("Unloading klm_test\n");
	
}
	
module_init(klm_test_init);
module_exit(klm_test_exit);

