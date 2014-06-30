#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");


static int empty_init(void){

	return 0;
}
	

static void empty_exit(void){
}

module_init(empty_init);
module_exit(empty_exit);
