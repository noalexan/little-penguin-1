#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/string.h>

#define LOGIN "noalexan"
#define LOGIN_LEN 8

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Noah Alexandre");
MODULE_DESCRIPTION("A misc device kernel module");
MODULE_VERSION("1.0");

static int my_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "misc_device: open called\n");
	printk(KERN_INFO "misc_device: device number: %i %i\n", imajor(inode), iminor(inode));

	if (file->f_mode & FMODE_READ)
		printk(KERN_INFO "misc_device: read permission\n");

	if (file->f_mode & FMODE_WRITE)
		printk(KERN_INFO "misc_device: write permission\n");

	return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "misc_device: close called\n");
	return 0;
}

static ssize_t my_write(struct file *file, const char __user *user_buffer, size_t user_len, loff_t *ppos)
{
	char buffer[LOGIN_LEN];

	printk(KERN_INFO "misc_device: write called\n");

	if (user_buffer == NULL) {
		printk(KERN_ERR "misc_device: received buffer is NULL\n");
		return -EINVAL;
	}

	if (user_len != LOGIN_LEN) {
		printk(KERN_ERR "misc_device: entered value does not match 'noalexan'\n");
		return -EINVAL;
	}

	if (copy_from_user(buffer, user_buffer, user_len))
		return -EFAULT;

	if (strncmp(buffer, LOGIN, user_len)) {
		printk(KERN_ERR "misc_device: entered value does not match 'noalexan'\n");
		return -EINVAL;
	}

	*ppos += LOGIN_LEN;
	return LOGIN_LEN;
}

static ssize_t my_read(struct file *file, char __user *user_buffer, size_t user_len, loff_t *ppos)
{
	printk(KERN_INFO "misc_device: read called\n");

	if (user_buffer == NULL) {
		printk(KERN_ERR "misc_device: received buffer is NULL\n");
		return -EINVAL;
	}

	if (*ppos >= LOGIN_LEN) // EOF
		return 0;

	size_t len = (*ppos + user_len >= LOGIN_LEN) ? LOGIN_LEN - *ppos : user_len;

	if (copy_to_user(user_buffer, LOGIN + *ppos, len)) {
		printk(KERN_ERR "misc_device: error while copy_to_user\n");
		return -EFAULT;
	}

	*ppos += len;
	return len;
}

static const struct file_operations fops = {
	owner: THIS_MODULE,
	read: my_read,
	write: my_write,
	open: my_open,
	release: my_close,
};

static struct miscdevice my_device = {
	name: "fortytwo",
	minor: MISC_DYNAMIC_MINOR,
	fops: &fops,
	mode: 0666,
};

static int __init my_init(void)
{
	int status;

	printk(KERN_INFO "misc_device: init\n");

	status = misc_register(&my_device);
	if (status) {
		printk(KERN_ERR "misc_device: error during register\n");
		return -status;
	}

	return 0;
}

static void __exit my_exit(void)
{
	printk(KERN_INFO "misc_device: deregister\n");
	misc_deregister(&my_device);
}

module_init(my_init);
module_exit(my_exit);
