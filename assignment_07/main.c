#include "debug.h"

static struct dentry *debug_dir;

static const struct file_operations id_fops = {
	owner: THIS_MODULE,
	read: id_read,
	write: id_write,
};

static const struct file_operations jiffies_fops = {
	owner: THIS_MODULE,
	read: jiffies_read,
};

struct mutex foo_mutex;

static const struct file_operations foo_fops = {
	owner: THIS_MODULE,
	open: foo_open,
	release: foo_release,
	read: foo_read,
	write: foo_write,
};

static int __init my_init(void)
{
	mutex_init(&foo_mutex);mutex_init(&foo_mutex);

	debug_dir = debugfs_create_dir("fortytwo", NULL);
	if (IS_ERR(debug_dir)) {
		printk(KERN_ERR "debug: Failed to create debugfs directory 'fortytwo'\n");
		return PTR_ERR(debug_dir);
	}
	printk(KERN_NOTICE "debug: Created debugfs directory 'fortytwo'\n");

	struct dentry *debugfs_file = debugfs_create_file("id", 0666, debug_dir, NULL, &id_fops);
	if (IS_ERR(debugfs_file)) {
		printk(KERN_ERR "debug: Failed to create debugfs file 'id'\n");
		debugfs_remove_recursive(debug_dir);
		return PTR_ERR(debugfs_file);
	}
	printk(KERN_NOTICE "debug: Created debugfs file 'id'\n");

	debugfs_file = debugfs_create_file("jiffies", 0444, debug_dir, NULL, &jiffies_fops);
	if (IS_ERR(debugfs_file)) {
		printk(KERN_ERR "debug: Failed to create debugfs file 'jiffies'\n");
		debugfs_remove_recursive(debug_dir);
		return PTR_ERR(debugfs_file);
	}
	printk(KERN_NOTICE "debug: Created debugfs file 'jiffies'\n");

	debugfs_file = debugfs_create_file("foo", 0644, debug_dir, NULL, &foo_fops);
	if (IS_ERR(debugfs_file)) {
		printk(KERN_ERR "debug: Failed to create debugfs file 'foo'\n");
		debugfs_remove_recursive(debug_dir);
		return PTR_ERR(debugfs_file);
	}
	printk(KERN_NOTICE "debug: Created debugfs file 'foo'\n");

	return 0;
}

static void __exit my_exit(void)
{
	debugfs_remove_recursive(debug_dir);
	printk(KERN_NOTICE "debug: cleanup\n");
}

module_init(my_init);
module_exit(my_exit);
