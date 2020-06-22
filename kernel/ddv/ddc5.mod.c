#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0x8ed3c35e, "module_layout" },
	{ 0xe0e525e7, "cdev_del" },
	{ 0xf6315d4b, "device_destroy" },
	{ 0x822b35fd, "class_destroy" },
	{ 0xeb2592df, "device_create" },
	{ 0xb1be36ba, "__class_create" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xb57d92f8, "cdev_add" },
	{ 0xde9bbb3f, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x6ce54927, "_copy_to_user" },
	{ 0x89cfd022, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x56470118, "__warn_printk" },
	{ 0x70c8d007, "kmem_cache_alloc_trace" },
	{ 0x84a98ec6, "kmalloc_caches" },
	{ 0x37a0cba, "kfree" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "3B182CE5A55BE8F81086274");
