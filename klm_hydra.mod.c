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
	{ 0x9e87024b, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0xa1fe0f02, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0x8c056038, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0xf087137d, __VMLINUX_SYMBOL_STR(__dynamic_pr_debug) },
	{ 0x2c997591, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0x644c5e33, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0xb1b443d4, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0xb34016ea, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0xc5a33a3c, __VMLINUX_SYMBOL_STR(kthread_bind) },
	{ 0xbb6b1571, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xf97456ea, __VMLINUX_SYMBOL_STR(_raw_spin_unlock_irqrestore) },
	{ 0xeae3dfd6, __VMLINUX_SYMBOL_STR(__const_udelay) },
	{ 0x21fb443e, __VMLINUX_SYMBOL_STR(_raw_spin_lock_irqsave) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x796497be, __VMLINUX_SYMBOL_STR(sched_setscheduler) },
	{ 0x9cb4a4a, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

