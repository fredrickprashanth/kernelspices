#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>

static unsigned long ioremap_addr;
static int ioremap_size;
volatile char *ioremap_ptr;

#define MAX_SZ 64
static ssize_t
ioremap_test_fwrite(struct file *file, const char __user *buf_usr,
	size_t nbytes, loff_t *off) {
	
	char buf[MAX_SZ];
	if (strncpy_from_user(buf, buf_usr, sizeof(buf)) <0)
		return -EFAULT;

	if (ioremap_ptr) {
		iounmap(ioremap_ptr);
	}
	sscanf(buf, "%lx %d", &ioremap_addr, &ioremap_size);
	if (ioremap_size > PAGE_SIZE/2)
		ioremap_size = PAGE_SIZE/2;
	ioremap_ptr = ioremap(ioremap_addr, ioremap_size);
	if (!ioremap_ptr) {
		printk(KERN_EMERG "ioremap failed addr %p len %d\n", 
			(void *)ioremap_addr, ioremap_size);
	}
	
	return nbytes;
}
static int
ioremap_test_seq_show(struct seq_file *s, void *unused) {
	
	if (!ioremap_ptr)
		return 0;

	seq_write(s, ioremap_ptr, ioremap_size);

	return 0;
} 

static int
ioremap_test_open(struct inode *inode, struct file* file) {
	return single_open(file, ioremap_test_seq_show, inode);
}

struct proc_dir_entry* ioremap_test_proc_entry;

static struct file_operations ioremap_test_fops = {
	.open = ioremap_test_open,
	.read = seq_read,
	.write = ioremap_test_fwrite,
	.release = seq_release,
};

static int
klm_ioremap_test_init(void) {
	ioremap_test_proc_entry = 
		proc_create("ioremap_test", 0666, NULL, &ioremap_test_fops);	
	if (!ioremap_test_proc_entry) {
		printk(KERN_EMERG "ioremap_test proc entry create failed\n");
		return 0;	
	}
	return 0;
}

static void
klm_ioremap_test_exit(void) {
	if (ioremap_test_proc_entry) 
		remove_proc_entry("ioremap_test", NULL);
}


module_init(klm_ioremap_test_init);
module_exit(klm_ioremap_test_exit);
