// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

int do_work(int *my_int)
{
	int x;
	int y = *my_int;

	for (x = 0; x < my_int; ++x)
		udelay(10);

	if (y < 10)
		/* That was a long sleep, tell userspace about it */
		pr_info("We slept a long time!");

	return x * y;
}

int my_init(void)
{
	int x = 10;

	x = do_work(&x);
	return x;
}

void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);
