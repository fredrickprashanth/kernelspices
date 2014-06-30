CC := gcc
MAKE := make

obj-m := klm_test.o klm_wq.o klm_seq_file.o klm_bug_lock.o klm_proto.o klm_two_threads.o
obj-m += klm_sched_tp.o klm_delay.o klm_delay_spec.o klm_ratelimit.o klm_ptime.o klm_trace_mod.o
obj-m += klm_stk_ofl.o klm_hydra.o klm_mem_balloon.o klm_sb_cached_inodes.o
obj-m += klm_double_free.o klm_mutex.o klm_ioremap_test.o

KERNEL_DIR = /work/kernels/linux-2.6-stable

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean


