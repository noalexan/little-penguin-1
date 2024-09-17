#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/nsproxy.h>
#include <linux/mount.h>
#include <../fs/mount.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Noah Alexandre");
MODULE_DESCRIPTION("A device to list mounted disks");
MODULE_VERSION("1.0");

static void print_mount(struct seq_file *file, struct mount *mnt)
{
	struct mount *tmp;
	struct path path;
	char *buffer;

	list_for_each_entry(tmp, &mnt->mnt_mounts, mnt_child)
	{
		buffer = kmalloc(PATH_MAX, GFP_KERNEL);

		path.mnt = &tmp->mnt;
		path.dentry = path.mnt->mnt_root;

		seq_printf(file, "%-15s%s\n", tmp->mnt_devname, d_path(&path, buffer, PATH_MAX));
		kfree(buffer);
		print_mount(file, tmp);
	}
}

static int mymounts_show(struct seq_file *file, void *arg)
{
	print_mount(file, current->nsproxy->mnt_ns->root);
	return 0;
}

static int __init mymounts_init(void)
{
	pr_info("mymounts: init.\n");

	if (proc_create_single("mymounts", 0, NULL, &mymounts_show) == NULL)
	{
		pr_err("mymounts: Unable to create '/proc/mymounts'.\n");
		return -ENOMEM;
	}
	pr_info("mymounts: '/proc/mymounts' created.\n");

	return 0;
}

static void __exit mymounts_exit(void)
{
	remove_proc_entry("mymounts", NULL);
	pr_info("mymounts: deinit.\n");
}

module_init(mymounts_init);
module_exit(mymounts_exit);
