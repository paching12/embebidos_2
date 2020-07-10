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
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0x437edb9e, "kthread_stop" },
	{ 0xad45e203, "wake_up_process" },
	{ 0x28b3938b, "kthread_create_on_node" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x89cfd022, "_copy_from_user" },
	{ 0x6ce54927, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x56470118, "__warn_printk" },
	{ 0x70c8d007, "kmem_cache_alloc_trace" },
	{ 0x84a98ec6, "kmalloc_caches" },
	{ 0x37a0cba, "kfree" },
	{ 0x955a832f, "___preempt_schedule" },
	{ 0xc5850110, "printk" },
	{ 0x1000e51, "schedule" },
	{ 0x4629334c, "__preempt_count" },
	{ 0x706c5a65, "preempt_count_sub" },
	{ 0x7a2af7b4, "cpu_number" },
	{ 0xc38259af, "__this_cpu_preempt_check" },
	{ 0xf229424a, "preempt_count_add" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "11B96D9120428CDC68CC868");
