#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/debugfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Noah Alexandre");
MODULE_DESCRIPTION("A debug kernel module");
MODULE_VERSION("1.0");

int foo_open(struct inode *node, struct file *file);
int foo_release(struct inode *node, struct file *file);
ssize_t foo_write(struct file *file, const char __user *user_buffer, size_t user_len, loff_t *ppos);
ssize_t foo_read(struct file *file, char __user *user_buffer, size_t user_len, loff_t *ppos);

extern struct mutex foo_mutex;

ssize_t id_write(struct file *file, const char __user *user_buffer, size_t user_len, loff_t *ppos);
ssize_t id_read(struct file *file, char __user *user_buffer, size_t user_len, loff_t *ppos);

ssize_t jiffies_read(struct file *file, char __user *user_buffer, size_t user_len, loff_t *ppos);
