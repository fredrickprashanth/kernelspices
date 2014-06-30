#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
MODULE_LICENSE("GPL");

void 
do_double_free(void)
{
	void *ptr = kmalloc(4096*2, GFP_KERNEL);
	printk(KERN_DEBUG "Alloc-ed %p\n", ptr);
	printk(KERN_DEBUG "Freeing %p\n", ptr);
	kfree(ptr);
	kfree(ptr);
	printk(KERN_DEBUG "Freeing %p double\n", ptr);

}

static int double_free_init(void){
	do_double_free();
	return 0;
}
static void double_free_exit(void){
	;
}
module_init(double_free_init);
module_exit(double_free_exit);
	
