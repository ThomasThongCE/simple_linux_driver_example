#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>   // for fs function like alloc_chrdev_region
#include <linux/types.h>
#include <linux/device.h>   // for device_create and class_create

MODULE_LICENSE("GPL");
MODULE_AUTHOR("THOMASTHONG");
MODULE_VERSION("1.0.0");

#define PDEBUG(fmt,args...) printk(KERN_DEBUG"%s: "fmt,DRIVER_NAME, ##args)
#define PERR(fmt,args...) printk(KERN_ERR"%s: "fmt,DRIVER_NAME,##args)
#define PINFO(fmt,args...) printk(KERN_INFO"%s: "fmt,DRIVER_NAME, ##args)

#define DRIVER_NAME "register_device"
#define FIRST_MINOR 0

dev_t device_num ;
struct class * device_class;
struct device * device;

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
    
    return 0;
}

void __exit Driver_exit(void)
{
    //printk(KERN_INFO "driver module exit\n");
    PINFO("driver module exit\n");

    device_destroy(device_class,device_num); 
    class_destroy(device_class);
    unregister_chrdev_region(device_num, FIRST_MINOR); 
}

module_init(Driver_init);
module_exit(Driver_exit);
