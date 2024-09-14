// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("Reverser misc device");

char str[PAGE_SIZE];
size_t str_len;
struct mutex myfd_mutex;

//
// Copy buffer to user buffer
//
static ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
	size_t retval;

	mutex_lock(&myfd_mutex);
	retval = simple_read_from_buffer(user, size, offs, str, str_len);
	mutex_unlock(&myfd_mutex);
	return retval;
}

//
// Get buffer from user, store it and reverse the stored one
//
static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs)
{
	mutex_lock(&myfd_mutex);
	str_len = simple_write_to_buffer(str, PAGE_SIZE, offs, user, size);
	for (int i = 0, tmp = str[0]; i < str_len / 2; tmp = str[++i]) {
		str[i] = str[str_len - 1 - i];
		str[str_len - 1 - i] = tmp;
	}
	mutex_unlock(&myfd_mutex);
	return str_len;
}

static const struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = &myfd_read,
	.write = &myfd_write,
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops,
};

static int __init myfd_init(void)
{
	mutex_init(&myfd_mutex);
	return misc_register(&myfd_device);
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
}

module_init(myfd_init);
module_exit(myfd_cleanup);
