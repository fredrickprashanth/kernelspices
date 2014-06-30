#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/spinlock.h>

MODULE_LICENSE("GPL");

unsigned int secs;
module_param(secs, uint, 0);

static DEFINE_SPINLOCK(try_it);
static void my_delay(unsigned long loops){
	while(loops--)
	{
		//spin_trylock(&try_it);
		__delay(1);
	}
	
}
static void my_udelay(int secs){
	unsigned long i,j;
	unsigned long msecs=secs*1000 ;
	for(i=0;i<msecs;i++)
		for(j=0;j<1000;j++)
		{
			udelay(1);
			spin_trylock(&try_it);
		}
	

}
static void my_ndelay(int secs){
	unsigned long i,j,k;
	unsigned long msecs=secs*1000 ;
	for(i=0;i<msecs;i++)
		for(j=0;j<1000;j++)
			for(k=0;k<1000;k++)
			{
				ndelay(1);
				spin_trylock(&try_it);
			}
	

}
static int proto_init(void){
	unsigned long long j1,j2;
	spin_lock(&try_it);

	printk(KERN_DEBUG "starting proto mod\n");

	printk(KERN_DEBUG "loops_per_jiffy = %lu\n", loops_per_jiffy);

	printk(KERN_DEBUG "calling my_delay with secs = %u at jiffies=%llu\n", secs, (j1=jiffies));
	my_delay(loops_per_jiffy*HZ*secs);
	printk(KERN_DEBUG "finished my_delay at jiffies =%llu\n", (j2=jiffies));
	printk(KERN_DEBUG "msec elapsed = %u\n", jiffies_to_msecs(j2-j1));

	printk(KERN_DEBUG "calling my_udelay with secs = %u at jiffies=%llu\n", secs, (j1=jiffies));
	my_udelay(secs);
	printk(KERN_DEBUG "finished my_udelay at jiffies =%llu\n", j2=jiffies);
	printk(KERN_DEBUG "msec elapsed = %u\n", jiffies_to_msecs(j2-j1));

	printk(KERN_DEBUG "calling my_ndelay with secs = %u at jiffies=%llu\n", secs, (j1=jiffies));
	my_ndelay(secs);
	printk(KERN_DEBUG "finished my_ndelay at jiffies =%llu\n", (j2=jiffies));
	printk(KERN_DEBUG "msec elapsed = %u\n", jiffies_to_msecs(j2-j1));

	spin_unlock(&try_it);
	
	return 0;
}

static void proto_exit(void){
}

module_init(proto_init);
module_exit(proto_exit);

