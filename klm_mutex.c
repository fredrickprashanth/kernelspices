#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL");

static DEFINE_MUTEX(my_mutex);

int proc_mutex_read(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	sprintf(page, "lock count = %d\n", atomic_read(&my_mutex.count));
}
int proc_mutex_write(struct file *file, const char __user *buffer,
	unsigned long nbytes, void *data)
{
	int do_lock;
	sscanf(buffer, "%d", &do_lock);
	if (do_lock)
		mutex_lock(&my_mutex);
	else
		mutex_unlock(&my_mutex);
	return nbytes;
}
static struct proc_dir_entry *proc_mutex_entry;


static int
klm_mutex_init(void)
{
	proc_mutex_entry  = create_proc_entry("my_mutex", 0666, NULL);
	if (!proc_mutex_entry)
	{
		printk("proc create failed\n");
		return 0;
	}
	proc_mutex_entry->read_proc = proc_mutex_read;
	proc_mutex_entry->write_proc = proc_mutex_write;
	proc_mutex_entry->data = NULL;

	return 0;
}
static void
klm_mutex_exit(void){
	if (proc_mutex_entry)
		remove_proc_entry("my_mutex", NULL);
}
