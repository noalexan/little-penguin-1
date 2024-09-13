#include "debug.h"

#define LOGIN "noalexan"
#define LOGIN_LEN 8

ssize_t id_write(struct file *file, const char __user *user_buffer, size_t user_len, loff_t *ppos)
{
	char buffer[LOGIN_LEN];

	printk(KERN_INFO "debug: write called\n");

	if (user_buffer == NULL) {
		printk(KERN_ERR "debug: received buffer is NULL\n");
		return -EINVAL;
	}

	if (user_len != LOGIN_LEN) {
		printk(KERN_ERR "debug: entered value does not match 'noalexan'\n");
		return -EINVAL;
	}

	if (copy_from_user(buffer, user_buffer, user_len))
		return -EFAULT;

	if (strncmp(buffer, LOGIN, user_len)) {
		printk(KERN_ERR "debug: entered value does not match 'noalexan'\n");
		return -EINVAL;
	}

	*ppos += LOGIN_LEN;
	return LOGIN_LEN;
}

ssize_t id_read(struct file *file, char __user *user_buffer, size_t user_len, loff_t *ppos)
{
	printk(KERN_INFO "debug: read called\n");

	if (user_buffer == NULL) {
		printk(KERN_ERR "debug: received buffer is NULL\n");
		return -EINVAL;
	}

	if (*ppos >= LOGIN_LEN) // EOF
		return 0;

	size_t len = (*ppos + user_len >= LOGIN_LEN) ? LOGIN_LEN - *ppos : user_len;

	if (copy_to_user(user_buffer, LOGIN + *ppos, len)) {
		printk(KERN_ERR "debug: error while copy_to_user\n");
		return -EFAULT;
	}

	*ppos += len;
	return len;
}
