#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/string.h>

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
	int status;

	printk(KERN_INFO "misc_device: write called\n");

	printk(KERN_INFO "You entered '%s' (length: %lu)\n", user_buffer, user_len);

	if (strncmp(user_buffer, "noalexan\n", user_len)) {
		printk(KERN_ERR "misc_device: entered value does not match 'noalexan'\n");
		return -EINVAL;
	}

	return 9;
}

static ssize_t my_read(struct file *file, char __user *user_buffer, size_t user_len, loff_t *ppos)
{
	int status;

	printk(KERN_INFO "misc_device: read called\n");
	status = copy_to_user(user_buffer, "noalexan\n", 9);

	if (status) {
		printk(KERN_ERR "misc_device: error while copy_to_user\n");
		return -status;
	}

	return 9;
}

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
	.open = my_open,
	.release = my_close,
};

static struct miscdevice my_device = {
	.name = "fortytwo",
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &fops
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
