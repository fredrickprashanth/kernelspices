#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/rt.h>
#include <linux/ftrace.h>
#include <trace/events/sched.h>

MODULE_LICENSE("GPL");
static DEFINE_PER_CPU(unsigned long, rt_task_start_jiffies);
static DEFINE_PER_CPU(unsigned long, rt_task_end_jiffies);
static DEFINE_PER_CPU(unsigned long, ts_task_msecs);
static DEFINE_PER_CPU(unsigned long, rt_hog_print_count);
static DEFINE_PER_CPU(struct task_struct *, last_rt_task);

static void
rt_task_hog_warn(struct task_struct *prev, struct task_struct *next)
{

        #define WARN_RT_HOG_MSECS 3000
        #define WARN_RT_HOG_WINDOW_MSECS 1000
        #define IGNORE_TS_TASK_MSECS 100
        #define MAX_RT_HOG_PRINTS 8
        int cpu = raw_smp_processor_id();
        unsigned long delta;

        if(rt_task(prev))
        {

                delta = per_cpu(rt_task_start_jiffies, cpu)>0?
			jiffies_to_msecs(jiffies - per_cpu(rt_task_start_jiffies, cpu)) - per_cpu(ts_task_msecs, cpu):0;

		
                if(delta > WARN_RT_HOG_MSECS)
		{
			//prev != per_cpu(last_rt_task, cpu) && 
			//if(delta < WARN_RT_HOG_MSECS + WARN_RT_HOG_WINDOW_MSECS)
			{
                   		if( per_cpu(rt_hog_print_count, cpu) < MAX_RT_HOG_PRINTS )
				{
	
					pr_emerg("rt hog detected for %lums on CPU%d." 
					"last task [%s:%d]\n", delta, cpu, prev->comm, prev->pid);
		
					per_cpu(rt_hog_print_count, cpu) += 1;
					per_cpu(last_rt_task, cpu) = prev;
				
                		}
			}
			/*
			else
			{
				per_cpu(rt_task_start_jiffies, cpu) = jiffies;
				per_cpu(ts_task_msecs, cpu) = 0;
				per_cpu(rt_hog_print_count, cpu) = 0;
			}
			*/
		}
		
                if(!rt_task(next))
		{
                        per_cpu(rt_task_end_jiffies, cpu) = jiffies;
		}
		else
		{
			delta = per_cpu(rt_task_end_jiffies, cpu)>0?jiffies_to_msecs(jiffies - per_cpu(rt_task_end_jiffies, cpu)):0;
			if(delta > WARN_RT_HOG_WINDOW_MSECS)
			{
				per_cpu(rt_task_start_jiffies, cpu) = jiffies;
				per_cpu(ts_task_msecs, cpu) = 0;
				per_cpu(rt_hog_print_count, cpu) = 0;
			}
				
		}

        }
        else
        {

                if(rt_task(next))
                {
                        if(per_cpu(rt_task_end_jiffies, cpu) == 0)
			{
                                per_cpu(rt_task_start_jiffies, cpu) = jiffies;
				per_cpu(rt_hog_print_count, cpu) = 0;
				per_cpu(ts_task_msecs, cpu) = 0;
			}
                        else
                        {
                                delta = jiffies_to_msecs(jiffies - per_cpu(rt_task_end_jiffies, cpu));
                                if(delta > IGNORE_TS_TASK_MSECS)
                                {
                                        per_cpu(rt_task_start_jiffies, cpu) = jiffies;
                                        per_cpu(ts_task_msecs, cpu) = 0;
                                        per_cpu(rt_hog_print_count, cpu) = 0;
                                }
                                else
                                        per_cpu(ts_task_msecs, cpu) += delta;
                        }
                }


        }



}

/*
static DEFINE_PER_CPU(unsigned long, rt_task_start_jiffies);
static DEFINE_PER_CPU(unsigned long, rt_task_end_jiffies);

static void
rt_task_hog_warn(struct task_struct *prev, struct task_struct *next)
{

        #define WARN_RT_HOG_MSECS 3000
        #define WARN_RT_HOG_WINDOW_MSECS 1000
	#define IGNORE_TS_TASK_MSECS 500
        int cpu = raw_smp_processor_id();
        unsigned long delta;

        if(rt_task(prev))
        {

                delta = jiffies_to_msecs(jiffies - per_cpu(rt_task_start_jiffies, cpu));

                if(delta > WARN_RT_HOG_MSECS){
                                pr_emerg("rt hog detected for %lums. last task [%s:%d]\n", delta, prev->comm, prev->pid);
                }
                if(delta > WARN_RT_HOG_MSECS + WARN_RT_HOG_WINDOW_MSECS)
                                per_cpu(rt_task_start_jiffies, cpu) = jiffies;

                if(!rt_task(next))
                        per_cpu(rt_task_end_jiffies, cpu) = jiffies;

        }
        else
        {

                if(rt_task(next) && ( per_cpu(rt_task_end_jiffies, cpu) == 0 ||
                        jiffies_to_msecs(jiffies - per_cpu(rt_task_end_jiffies, cpu)) > IGNORE_TS_TASK_MSECS) )
                        per_cpu(rt_task_start_jiffies, cpu) = jiffies;

        }

}

static DEFINE_PER_CPU(unsigned long, rt_hog_jiffies);
static DEFINE_PER_CPU(unsigned long, rt_task_start_jiffies);

static void
rt_task_hog_warn(struct task_struct *prev, struct task_struct *next)
{

        #define WARN_RT_HOG_MSECS 3000
	#define WARN_RT_HOG_WINDOW_MSECS 1000
	int cpu = raw_smp_processor_id();
	unsigned long delta;

        if(rt_task(prev))
	{
                per_cpu(rt_hog_jiffies, cpu) += 
			(per_cpu(rt_task_start_jiffies, cpu)? jiffies - per_cpu(rt_task_start_jiffies, cpu): 0);

		delta = jiffies_to_msecs(per_cpu(rt_hog_jiffies, cpu));

		if(delta > WARN_RT_HOG_MSECS){
				pr_emerg("rt hog detected for %lums. last task [%s:%d]\n", delta, prev->comm, prev->pid);
		}
		if(delta > WARN_RT_HOG_MSECS + WARN_RT_HOG_WINDOW_MSECS)
                                per_cpu(rt_hog_jiffies, cpu) = 0;

	}

        if(rt_task(next))
        {
                per_cpu(rt_task_start_jiffies, cpu) = jiffies;

        }
        else
	{
                if( jiffies - per_cpu(rt_task_start_jiffies, cpu) > 1 )
                        per_cpu(rt_hog_jiffies, cpu) = 0;

	}
}
*/
static void 
probe_sched_switch(void *ignore, struct task_struct *prev, struct task_struct *next){
	rt_task_hog_warn(prev, next);			
}

static int klm_sched_tp_init(void){
	int ret;
	ret = register_trace_sched_switch(probe_sched_switch, NULL);
	return 0;
}
static void klm_sched_tp_exit(void){
	unregister_trace_sched_switch(probe_sched_switch, NULL);
	tracepoint_synchronize_unregister();
}

module_init(klm_sched_tp_init);
module_exit(klm_sched_tp_exit);
