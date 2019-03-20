#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("THOMASTHONG");
MODULE_VERSION("1.0.0");

static int __init Driver_init (void)
{
    printk(KERN_INFO "driver module init\n");

    return 0;
}

void __exit Driver_exit(void)
{
    printk(KERN_INFO "driver module exit\n");
}

module_init(Driver_init);
module_exit(Driver_exit);
