#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>

MODULE_LICENSE("GPL");

static work_struct bug_work;
static void do_my_work(struct work_struct *);
	
char wq_cmd[256];
static DEFINE_MUTEX(bug_mutex);
static DEFINE_SPINLOCK(bug_lock);

static void do_my_work(struct work_struct *){
	if(!strcmp(wq_cmd, "mutex"))
	{
		mutex_lock(&bug_mutex);	
		mutex_lock(&bug_mutex);	
	}
	else
	if(!strcmp(wq_cmd, "spinlock"))
	{
	}
	if(!strcmp(wq_cmd, "spinlock_irq"))
	{
	}
}
static int wq_bug_entry_write(struct file *file,
const char __user *buffer,
size_t nbytes,
loff_t *ppos){
	char str[256];
	sscanf(buffer, "%s", wq_cmd);
	schedule_work(&bug_work);
}
struct proc_dir_entry_t *wq_bug_entry;
static int wq_bug_init(void){
	
	wq_bug_entry = create_proc_entry("wq_bug",0666);
	if(!wq_bug_entry){
		pr_info("Could not create proc entry wq_bug_entry\n");
		return 0;
	}
	
	wq_bug_entry->write_proc = wq_bug_entry_write;	
	wq_bug_entry->read_proc =  wq_bug_entry_read;
	wq_bug_entry->data = NULL;
	
	INIT_WORK(&bug_work, do_my_work);
	return 0;
}
	

static void wq_bug_exit(void){
}

module_init(wq_bug_init);
module_exit(wq_bug_exit);
