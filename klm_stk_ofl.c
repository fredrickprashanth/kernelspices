#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

static void stk_thrash(int i){
	int a[20]; //20*4 = 80 bytes
	stk_thrash(1);
}
static int klm_stk_ofl_init(void){

	stk_thrash(1);
	return 0;
}
	

static void klm_stk_ofl_exit(void){
}

module_init(klm_stk_ofl_init);
module_exit(klm_stk_ofl_exit);
