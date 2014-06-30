
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/spinlock.h>
#include <linux/cpu.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/mm.h>

MODULE_LICENSE("Dual GPL/BSD");

int major_num;



static int
klm_copy_user_init (void) {

	
	major_num = register_chrdev(0, "copyuser", &copy_user_fops);
	if (major_num < 0)
		printk(KERN_EMERG "char dev register failed with %d\n", 
					major_num);
	
	
}
static void
klm_copy_user_exit (void) {
}
module_init(klm_copy_user_init);
module_exit(klm_copy_user_exit);
