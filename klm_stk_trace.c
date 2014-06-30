#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");

void
do_backtrace(void *data)
{
	//pid_t p = (pid_t)data;
	int cpu = raw_smp_processor_id();
	printk("Doing show_stack on CPU%d\n", cpu);
	printk("================================");
	sched_show_stack(NULL);
	printk("================================");

}

static int stk_bt_pid_proc_entry_write(struct file *file,
const char __user *buffer, unsigned long nbytes, void *data)
{
	pid_t pid;
	struct task_struct *bt_task;		

	smp_call_function(do_backtrace, NULL, 
			1/*wait for all cpus*/);

	sscanf(buffer, "%d", &pid);
	rcu_read_lock();
	bt_task = pid_task(find_vpid((pid)), PIDTYPE_PID);
	if(!bt_task)
		goto done;
	printk("Backtrace of Task [%s:%d]\n", bt_task->comm,
			bt_task->pid);
	show_stack(bt_task, NULL);
	done:
	rcu_read_unlock();

	return nbytes;

}
static struct proc_dir_entry *stk_bt_pid_proc_entry;
int
stk_trace_init(void)
{
	stk_bt_pid_proc_entry = create_proc_entry("stk_bt_pid", 0666, NULL);
	if(!stk_bt_pid_proc_entry)
	{
		printk(KERN_DEBUG "Could not create stk_bit_pid proc entry\n");
		return -1;
	}
	stk_bt_pid_proc_entry->write_proc = stk_bt_pid_proc_entry_write;
	return 0;
}
static void 
stk_trace_exit(void) 
{
	if(stk_bt_pid_proc_entry)
		remove_proc_entry("stk_bt_pid", NULL);
		
}


module_init(stk_trace_init);
module_exit(stk_trace_exit);

