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
	{ 0xbb2b9e2e, __VMLINUX_SYMBOL_STR(seq_release) },
	{ 0xa1fe0f02, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0x8c056038, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x2c997591, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0xb1b443d4, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x504f5651, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xcebfc0dc, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x644c5e33, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

