#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
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
int proc_tp_write(struct file *file, const char __user *buffer,
unsigned long nbytes, void *data){
	char msg[64];
	sscanf(buffer, "%s", msg);
	trace_proc_tp_write(msg);
	pr_debug("proc_write msg=%s\n",msg);
	return nbytes;
}

static struct proc_dir_entry *proc_tp_entry;
static int trace_mod_init(void){

	proc_tp_entry = create_proc_entry("proc_tp", 0666, NULL);
	if(!proc_tp_entry)
	{
		pr_debug("Could not create proc proc_tp\n");
		return 0;
	}
	proc_tp_entry->write_proc = proc_tp_write;
	proc_tp_entry->read_proc = proc_tp_read;
	proc_tp_entry->data = NULL;

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
