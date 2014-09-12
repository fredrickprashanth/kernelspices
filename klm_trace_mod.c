#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include "klm_trace_mod.h"
#define CREATE_TRACE_POINTS

static void proc_tp_read_probe(void *ignore, char *msg){
pr_debug("proc_read trace probe: msg=%s\n",msg);
}

static void proc_tp_write_probe(void *ignore, char *msg){
pr_debug("proc_write trace probe: msg=%s\n",msg);
}

DEFINE_TRACE(proc_tp_read);
DEFINE_TRACE(proc_tp_write);

int proc_tp_read(char *page, char **start, off_t off,
int count, int *eof, void *data){
	char *msg = "READ";
	trace_proc_tp_read(msg);
	pr_debug("proc_read done\n");
	*eof = 1;
	return sprintf(page, "%s\n", msg);
}

static ssize_t proc_tp_write(struct file *file, const char __user *buffer,
size_t count, loff_t *ppos){
	char msg[64];
	sscanf(buffer, "%s", msg);
	trace_proc_tp_write(msg);
	pr_debug("proc_write msg=%s\n",msg);
	return count;
}


static int
proc_tp_show(struct seq_file *s, void *unused) {
	char *msg = "READ";
	trace_proc_tp_read(msg);
	pr_debug("proc_read done\n");
	seq_printf(s, "%s\n", msg);
	return 0;
}

static int
proc_tp_open(struct inode *inode, struct file *file) {
	return single_open(file, proc_tp_show, inode);
}

static struct file_operations proc_tp_fops = {
	.open = proc_tp_open,
	.read = seq_read,
	.write = proc_tp_write,
	.release = single_release,
};

static struct proc_dir_entry *proc_tp_entry;
static int trace_mod_init(void){

	proc_tp_entry = proc_create("proc_tp", 0666, NULL, &proc_tp_fops);
	if(!proc_tp_entry)
	{
		pr_debug("Could not create proc proc_tp\n");
		return 0;
	}

	WARN_ON(register_trace_proc_tp_read(proc_tp_read_probe, NULL));
	WARN_ON(register_trace_proc_tp_write(proc_tp_write_probe, NULL));


	return 0;
}
	

static void trace_mod_exit(void){
	if(proc_tp_entry)
	{
		remove_proc_entry("proc_tp", NULL);
		WARN_ON(unregister_trace_proc_tp_read(proc_tp_read_probe, NULL));
		WARN_ON(unregister_trace_proc_tp_write(proc_tp_write_probe, NULL));
	}
}

module_init(trace_mod_init);
module_exit(trace_mod_exit);
MODULE_LICENSE("GPL");
