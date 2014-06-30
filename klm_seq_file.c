#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/spinlock.h>
#include <linux/cpu.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");

static int junk_proc_seq_show(struct seq_file *seq, void *v){
	int i;
	for(i=0;i<300;i++){
		seq_printf(seq, "Printing something junk %d\n", i);
		seq_printf(seq, "Do you want something..? Here it is something you want..I guess..it is right..\n");
	}
	printk("using seq show in %s", __FUNCTION__);
	return 0;
}

static int junk_proc_seq_open(struct inode *inode, struct file *file){
	return single_open(file, junk_proc_seq_show, NULL);
}

static int x_junk_proc_write(struct file *file, const char __user *buffer, size_t nbytes, loff_t *ppos){
	char str[256];
	sscanf(buffer, "%s", str);
	printk("Junk read from user %s\n", str);
	return nbytes;
}

struct file_operations junk_proc_fops ={
	.owner = THIS_MODULE,
	.open  = junk_proc_seq_open,
	.read  = seq_read,
	.write = x_junk_proc_write,
	.llseek = seq_lseek,
	.release = single_release
};

#define PROC_NAME "junk"
static int klm_seq_file_init(void) {

	//create /proc/junk
	struct proc_dir_entry *junk_proc_entry;
	junk_proc_entry = create_proc_entry(PROC_NAME, 0666, NULL);
	if(junk_proc_entry){
		junk_proc_entry->proc_fops = &junk_proc_fops;
		printk(KERN_INFO "created proc %s entry\n", PROC_NAME);
	}
	return 0;
}

static void klm_seq_file_exit(void){

	remove_proc_entry(PROC_NAME, NULL);
	printk(KERN_INFO "remove proc %s entry\n", PROC_NAME);
}

module_init(klm_seq_file_init);
module_exit(klm_seq_file_exit);
