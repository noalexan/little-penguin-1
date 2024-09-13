#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Noah Alexandre");
MODULE_DESCRIPTION("A hello world kernel module");
MODULE_VERSION("1.0");

static int __init hello_world_init(void)
{
	printk(KERN_INFO "Hello, World!\n");
	return 0;
}

static void __exit hello_world_exit(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);
