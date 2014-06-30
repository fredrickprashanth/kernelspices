#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/spinlock.h>
#include <linux/cpu.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/workqueue.h>

MODULE_LICENSE("GPL");

static void do_foo(struct work_struct *);

static DECLARE_DELAYED_WORK(do_something, do_foo);
static int count;

static void do_foo(struct work_struct *foo){

	printk(KERN_EMERG "doing foo..%d\n", count);
	count+=1;
	schedule_delayed_work(&do_something, 15*HZ);
		
}

static int wq_init(void){

	printk(KERN_EMERG "staring wq\n");
	schedule_delayed_work(&do_something, 5*HZ);
	return 0;
}

static void wq_exit(void){

	cancel_delayed_work(&do_something);
}

module_init(wq_init);
module_exit(wq_exit);
