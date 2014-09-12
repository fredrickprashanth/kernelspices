#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x8b3819a4, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x53bab18a, __VMLINUX_SYMBOL_STR(kthread_stop) },
	{ 0x68e2f221, __VMLINUX_SYMBOL_STR(_raw_spin_unlock) },
	{ 0x67f7403e, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0xb34016ea, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0xc5a33a3c, __VMLINUX_SYMBOL_STR(kthread_bind) },
	{ 0xed433664, __VMLINUX_SYMBOL_STR(__per_cpu_offset) },
	{ 0xbb6b1571, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0xfe7c4287, __VMLINUX_SYMBOL_STR(nr_cpu_ids) },
	{ 0xc0a3d105, __VMLINUX_SYMBOL_STR(find_next_bit) },
	{ 0xfbe8ff5e, __VMLINUX_SYMBOL_STR(cpu_online_mask) },
	{ 0x7a2af7b4, __VMLINUX_SYMBOL_STR(cpu_number) },
	{ 0xf97456ea, __VMLINUX_SYMBOL_STR(_raw_spin_unlock_irqrestore) },
	{ 0x466c14a7, __VMLINUX_SYMBOL_STR(__delay) },
	{ 0xba497f13, __VMLINUX_SYMBOL_STR(loops_per_jiffy) },
	{ 0x21fb443e, __VMLINUX_SYMBOL_STR(_raw_spin_lock_irqsave) },
	{ 0xb3f7646e, __VMLINUX_SYMBOL_STR(kthread_should_stop) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xa3558d7, __VMLINUX_SYMBOL_STR(set_user_nice) },
	{ 0x9cb4a4a, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

