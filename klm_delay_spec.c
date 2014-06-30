#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>

MODULE_LICENSE("GPL");

static DEFINE_SPINLOCK(lock_it);

static int
delay_thread(void *data){
	
	int i, j;
	unsigned long flags;
	
	struct sched_param param = { .sched_priority = MAX_RT_PRIO -1};
	sched_setscheduler(current, SCHED_FIFO, &param);

	pr_info("started delay_thread_task[%d]\n", (int)data);
	spin_lock_irqsave(&lock_it, flags);
	for(i=0; i<((int)data)*1000; i++)
		for(j=0; j<1000; j++, udelay(1));
	spin_unlock_irqrestore(&lock_it, flags);
	pr_info("ended delay_thread_task[%d]\n", (int)data);
	return 0;
}

int delay_spec_proc_write(struct file *file, const char __user *buffer, 
unsigned long nbytes, void *data){
	int secs = 0, cpu = 0;
	struct task_struct *delay_thread_task;
	char *p = buffer;

	sscanf(buffer, "%d", &secs);
	pr_info("delay_spec secs=%d", secs);
	if(secs<0 || secs>100){
		pr_err("invalid values\n");
		return nbytes;
	}
	while(*p++!=' ');
	while(*p)
	{
		sscanf(p, "%d", &cpu);
	
		if((delay_thread_task = kthread_create(delay_thread, (void *)secs, "delay_thread/%d[%d]", cpu, secs)) == NULL)
		{
			pr_err("Could not start delay_thread_task[%d]\n", secs);
			continue;
		}
		kthread_bind(delay_thread_task, cpu);
		wake_up_process(delay_thread_task);

		while(*p && *p++!=' ');
	}
	return nbytes;
	
}

int delay_spec_proc_read(char *page, char **start, off_t off,
int count, int *eof, void *data){
	return sprintf(page, "%s\n", "delay_spec_proc is ready");
} 
static struct proc_dir_entry *delay_spec_proc_entry;
static int
klm_delay_spec_init(void){

	delay_spec_proc_entry = create_proc_entry("delay_spec", 0666, NULL);
	if(!delay_spec_proc_entry){
		pr_debug("Could not create delay_spec proc entry\n");
		return 0;
	}
	delay_spec_proc_entry->write_proc = delay_spec_proc_write;
	delay_spec_proc_entry->read_proc = delay_spec_proc_read;
	delay_spec_proc_entry->data = NULL;
	return 0;
}

static void
klm_delay_spec_exit(void){

	if(delay_spec_proc_entry)
		remove_proc_entry("delay_spec", NULL);
}


module_init(klm_delay_spec_init);
module_exit(klm_delay_spec_exit);
