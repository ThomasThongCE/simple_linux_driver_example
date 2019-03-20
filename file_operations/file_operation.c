#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>   // for fs function like alloc_chrdev_region / operation file
#include <linux/types.h>
#include <linux/device.h>   // for device_create and class_create
#include <linux/cdev.h>     // cdev operate function
#include <linux/uaccess.h>  // for copy to/from user function

MODULE_LICENSE("GPL");
MODULE_AUTHOR("THOMASTHONG");
MODULE_VERSION("1.0.0");

#define PDEBUG(fmt,args...) printk(KERN_DEBUG"%s: "fmt,DRIVER_NAME, ##args)
#define PERR(fmt,args...) printk(KERN_ERR"%s: "fmt,DRIVER_NAME,##args)
#define PINFO(fmt,args...) printk(KERN_INFO"%s: "fmt,DRIVER_NAME, ##args)

#define DRIVER_NAME "file_operation"
#define FIRST_MINOR 0

dev_t device_num ;
struct class * device_class;
struct device * device;
struct cdev device_cdev;
static char message[100] = {0};
static int message_size ;

static int device_open (struct inode *inode, struct file *filp)
{
    PINFO ("device open\n");
    return 0;
}

static int device_release (struct inode *inode, struct file *filp)
{
   PINFO ("device release\n");
   return 0;
}

static ssize_t device_read (struct file *filp , char __user * buff, size_t count, loff_t *off)
{
    PINFO ("device reading %d byte\n with %lld offset", message_size, *off);
    if ((copy_to_user(buff, message, message_size)) != 0)
        return -EFAULT;
    return message_size;
}

static ssize_t device_write(struct file *filp, const char __user * buff, size_t count, loff_t *off)
{
    PINFO ("device write %ld byte\n", count);
    if (!buff)
        return 0;
    if (count > 100)
        return 0;
    if (copy_from_user(message, buff, count) != 0)
        return -EFAULT;
    message_size = strlen(message);

    PINFO ("Message after write is: %s", message);
    return count;
}

static const struct file_operations ops =
{
    .owner  = THIS_MODULE,
    .open   = device_open,
    .read   = device_read,
    .write  = device_write,
    .release = device_release,
};

static int __init Driver_init (void)
{
    int res;

    //printk(KERN_INFO "driver module init\n");
    PINFO ("driver module init\n");

    // register a device with major and minor number without create device file
    res = alloc_chrdev_region(&device_num, FIRST_MINOR, 1, DRIVER_NAME); 
    if (res){
        PERR("Can't register driver, error code: %d \n", res); 
        return -1;
    }
    PINFO("Succes register driver with major is %d, minor is %d \n", MAJOR(device_num), MINOR(device_num));

    // create class 
    device_class = class_create(THIS_MODULE, DRIVER_NAME);
    if (device_class == NULL)
    {
        PERR("Class create fail\n");
        return -1;
    }

    // create device file
    device = device_create(device_class , NULL, device_num,NULL,  DRIVER_NAME);
    if (device == NULL)
    {
        PERR("Can't Create device file\n");
        return -1;
    }
    
    // create file operation
    cdev_init(&device_cdev, &ops);
    res = cdev_add(&device_cdev, device_num, 1);
    if (res)
    {
        PERR("Can't add cdev, error code: %d\n", res);
        return -1;
    }

    // init message
    sprintf(message, "hahahahha\n");
    message_size = strlen(message);

    return 0;
}

void __exit Driver_exit(void)
{
    //printk(KERN_INFO "driver module exit\n");
    PINFO("driver module remove from kernel\n");
    
    cdev_del(&device_cdev);
    device_destroy(device_class,device_num); 
    class_destroy(device_class);
    unregister_chrdev_region(device_num, FIRST_MINOR); 
}

module_init(Driver_init);
module_exit(Driver_exit);
