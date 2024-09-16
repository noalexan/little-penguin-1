#include "debug.h"
#include <linux/mutex.h>

static char data[PAGE_SIZE];
static ssize_t data_len;

int foo_open(struct inode *node, struct file *file)
{
	pr_info("debug: foo: open called\n");
	if (mutex_lock_interruptible(&foo_mutex))
		return -EINTR;
	return 0;
}

int foo_release(struct inode *node, struct file *file)
{
	pr_info("debug: foo: release called\n");
	mutex_unlock(&foo_mutex);
	return 0;
}

ssize_t foo_write(struct file *file, const char __user *user_buffer, size_t user_len, loff_t *ppos)
{
	pr_info("debug: foo: write called\n");
	if (*ppos + user_len > PAGE_SIZE)
		return -ENOMEM;
	ssize_t len = simple_write_to_buffer(data, PAGE_SIZE, ppos, user_buffer, user_len);
	data_len = *ppos;
	return len;
}

ssize_t foo_read(struct file *file, char __user *user_buffer, size_t user_len, loff_t *ppos)
{
	pr_info("debug: foo: read called\n");
	return simple_read_from_buffer(user_buffer, user_len, ppos, data, data_len);
}
