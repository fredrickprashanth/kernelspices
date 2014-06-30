#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>


static unsigned long ioremap_addr;
static int ioremap_size;
volatile char *ioremap_ptr;


#define MAX_SZ 64
static int
ioremap_test_proc_write(struct file *file, const char __user *buf_usr,
	unsigned long nbytes, void *data) {
	
	char buf[MAX_SZ];
	if (strncpy_from_user(buf, buf_usr, sizeof(buf)) <0)
		return EFAULT;

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
ioremap_test_proc_read(char *page, char **start, off_t off,
	int count, int *eof, void *data) {
	
	int i;

	if (!ioremap_ptr)
		return 0;

	for (i = 0; i<ioremap_size; i++)
		page[i] = ioremap_ptr[i];
	*eof = 1;
	return ioremap_size;
} 

struct proc_dir_entry*
ioremap_test_proc_entry;

static int
klm_ioremap_test_init(void) {
	ioremap_test_proc_entry = 
		create_proc_entry("ioremap_test", 0666, NULL);	
	if (!ioremap_test_proc_entry) {
		printk(KERN_EMERG "ioremap_test proc entry create failed\n");
		return 0;	
	}
	ioremap_test_proc_entry->read_proc = ioremap_test_proc_read;
	ioremap_test_proc_entry->write_proc = ioremap_test_proc_write;
	return 0;
}

static void
klm_ioremap_test_exit(void) {
	if (ioremap_test_proc_entry) 
		remove_proc_entry("ioremap_test", NULL);
}


module_init(klm_ioremap_test_init);
module_exit(klm_ioremap_test_exit);
