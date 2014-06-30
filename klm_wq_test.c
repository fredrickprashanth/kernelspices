#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>

MODULE_LICENSE("GPL");

static void do_my_work(struct work_struct *);
	
static int wq_test_entry_write(struct file *file,
const char __user *buffer,
size_t nbytes,
loff_t *ppos){
	char str[256];
	sscanf(buffer, "%s", str);
	if(!strcmp(str, "mutex"))
	{
		
	}
	else
	if(!strcm
}
struct proc_dir_entry_t *wq_test_entry;
static int wq_test_init(void){
	
	wq_test_entry = create_proc_entry("wq_test",0666);
	if(!wq_test_entry){
		pr_info("Could not create proc entry wq_test_entry\n");
		return 0;
	}
	wq_test_entry->write_proc = wq_test_entry_write;	
	wq_test_entry->read_proc =  wq_test_entry_read;
	wq_test_entry->data = NULL;
	return 0;
}
	

static void wq_test_exit(void){
}

module_init(wq_test_init);
module_exit(wq_test_exit);
