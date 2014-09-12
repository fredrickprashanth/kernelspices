#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");


static int
hydra_thread(void *data){
	
	int i, j;
	unsigned long flags;
	
	DEFINE_SPINLOCK(lock_it);
	struct sched_param param = { .sched_priority = MAX_RT_PRIO -1};
	sched_setscheduler(current, SCHED_FIFO, &param);

	pr_info("started hydra_thread_task[%d]\n", (int)data);
	spin_lock_irqsave(&lock_it, flags);
	for(i=0; i<((int)data)*1000; i++)
		for(j=0; j<1000; j++, udelay(1));
	spin_unlock_irqrestore(&lock_it, flags);
	pr_info("ended hydra_thread_task[%d]\n", (int)data);
	return 0;
}

ssize_t hydra_fwrite(struct file *file, const char __user *user_buffer,
		     size_t nbytes, loff_t *off) 
{
	int secs = 0, cpu = 0;
	struct task_struct *hydra_thread_task;
	char buffer[255], *p;

	if (copy_from_user(buffer, user_buffer, sizeof(buffer)))
		return -EFAULT;

	p = buffer;
	sscanf(buffer, "%d", &secs);
	pr_info("hydra secs=%d", secs);
	if(secs<0 || secs>100){
		pr_err("invalid values\n");
		return nbytes;
	}
	while(*p++!=' ');
	while(*p)
	{
		sscanf(p, "%d", &cpu);
	
		if((hydra_thread_task = kthread_create(hydra_thread, (void *)secs, "hydra_thread/%d[%d]", cpu, secs)) == NULL)
		{
			pr_err("Could not start hydra_thread_task[%d]\n", secs);
			continue;
		}
		kthread_bind(hydra_thread_task, cpu);
		wake_up_process(hydra_thread_task);

		while(*p && *p++!=' ');
	}
	return nbytes;
	
}


static int
hydra_seq_show(struct seq_file *s, void *unused) {
	seq_printf(s, "hdyra is ready.\n");
	return 0;
}

static int
hydra_fopen(struct inode *inode, struct file *file) {
	return single_open(file, hydra_seq_show, inode);	
}

struct file_operations hydra_fops = {
	.open = hydra_fopen,
	.read = seq_read,
	.write = hydra_fwrite,
	.release = single_release,
};

static struct proc_dir_entry *hydra_proc_entry;

static int
klm_hydra_init(void){

	hydra_proc_entry = proc_create("hydra", 0666, NULL, &hydra_fops);
	if(!hydra_proc_entry){
		pr_debug("Could not create hydra proc entry\n");
		return 0;
	}
	return 0;
}

static void
klm_hydra_exit(void){

	if(hydra_proc_entry)
		remove_proc_entry("hydra", NULL);
}


module_init(klm_hydra_init);
module_exit(klm_hydra_exit);
