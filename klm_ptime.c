#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>

MODULE_LICENSE("GPL");


static int ptime_init(void){

	struct timespec t;
	read_persistent_clock(&t);
	return 0;
}
	

static void ptime_exit(void){
}

module_init(ptime_init);
module_exit(ptime_exit);
