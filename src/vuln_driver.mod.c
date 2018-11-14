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
	{ 0xe89b6b83, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x7a838142, __VMLINUX_SYMBOL_STR(misc_deregister) },
	{ 0xe86ba3de, __VMLINUX_SYMBOL_STR(misc_register) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x7c2d098f, __VMLINUX_SYMBOL_STR(krealloc) },
	{ 0xb14f552d, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xe44e3f36, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x4e4d6462, __VMLINUX_SYMBOL_STR(commit_creds) },
	{ 0x8a2144cd, __VMLINUX_SYMBOL_STR(prepare_creds) },
	{ 0x8b81418e, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x6bf1c17f, __VMLINUX_SYMBOL_STR(pv_lock_ops) },
	{ 0xe259ae9e, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "C3FF53344D914A7E3259222");
