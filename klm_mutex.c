#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");

static DEFINE_MUTEX(my_mutex);

int proc_mutex_seq_show(struct seq_file *s, void *unused)
{
	seq_printf(s, "lock count = %d\n", atomic_read(&my_mutex.count));
	return 0;
}

ssize_t proc_mutex_write(struct file *file, const char __user *user_buffer,
	size_t nbytes, loff_t *off)
{
	int do_lock;
	char buffer[256];

	if (copy_from_user(buffer, user_buffer, sizeof(buffer)))
		return -EFAULT;

	sscanf(buffer, "%d", &do_lock);

	if (do_lock)
		mutex_lock(&my_mutex);
	else
		mutex_unlock(&my_mutex);

	return nbytes;
}

static int proc_mutex_open(struct inode *inode, struct file *file) {
	return single_open(file, proc_mutex_seq_show, inode);
}

static struct proc_dir_entry *proc_mutex_entry;

static struct file_operations proc_mutex_fops = {
	.open = proc_mutex_open,
	.read = seq_read,
	.write = proc_mutex_write,
	.release = seq_release,
};

static int
klm_mutex_init(void)
{
	proc_mutex_entry  = proc_create("my_mutex", 0666, NULL, &proc_mutex_fops);
	if (!proc_mutex_entry)
	{
		printk("proc create failed\n");
		return 0;
	}
	return 0;
}
static void
klm_mutex_exit(void){
	if (proc_mutex_entry)
		remove_proc_entry("my_mutex", NULL);
}


module_init(klm_mutex_init);
module_exit(klm_mutex_exit);
