#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");

unsigned int secs;
module_param(secs, uint, 0);

static int ratelimit_init(void){

	unsigned long end_jiffies;
	if(secs == 0) secs = 20;
	end_jiffies = jiffies + secs*HZ; 
	while(time_is_before_jiffies(end_jiffies))
		pr_emerg("testing ratelimit jiffies = %lu\n", jiffies);
	return 0;
	
}
	

static void ratelimit_exit(void){

}

module_init(ratelimit_init);
module_exit(ratelimit_exit);
