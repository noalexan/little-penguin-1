#include "debug.h"

ssize_t jiffies_read(struct file *file, char __user *user_buffer, size_t user_len, loff_t *ppos)
{
	printk(KERN_INFO "debug: jiffies: read called\n");

	char buffer[0x100];
	int len = sprintf(buffer, "%li", jiffies);
	return simple_read_from_buffer(user_buffer, user_len, ppos, buffer, len);
}
