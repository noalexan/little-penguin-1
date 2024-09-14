#include "debug.h"
#include <linux/mutex.h>

static char data[PAGE_SIZE];
static ssize_t data_len = 0;

ssize_t foo_write(struct file *file, const char __user *user_buffer, size_t user_len, loff_t *ppos)
{
	printk(KERN_INFO "debug: foo: write called\n");

	mutex_lock(&foo_mutex);
	data_len = simple_write_to_buffer(data, PAGE_SIZE, ppos, user_buffer, user_len);
	mutex_unlock(&foo_mutex);
	return data_len;
}

ssize_t foo_read(struct file *file, char __user *user_buffer, size_t user_len, loff_t *ppos)
{
	printk(KERN_INFO "debug: foo: read called\n");

	mutex_lock(&foo_mutex);
	ssize_t len = simple_read_from_buffer(user_buffer, user_len, ppos, data, data_len);
	mutex_unlock(&foo_mutex);
	return len;
}
