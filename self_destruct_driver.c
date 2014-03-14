#include <linux/init.h>
#include <linux/kernel.h> 
#include <linux/module.h> 

/*
#include <linux/fs.h> 
#include <linux/uaccess.h> 
 
#if CONFIG_MODVERSIONS == 1 
#define MODVERSIONS 
#include <linux/version.h> 
#endif 
 
#define DEVICE_NUM 0
 
static int device_num = 0;
static char buffer[1024] = "devsd";
static int open_nr = 0; 
 
static int devsd_open(struct inode *inode, struct file *filp); 
static int devsd_release(struct inode *inode, struct file* filp); 
static ssize_t devsd_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos); 
static ssize_t devsd_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos); 
 
static struct file_operations devsd_fops = { 
    .read    = devsd_read, 
    .write   = devsd_write, 
    .open    = devsd_open, 
    .release = devsd_release, 
};    
 
static int devsd_open(struct inode *inode, struct file *filp) 
{ 
    printk("\nMain device: %d; slave device: %d\n", MAJOR(inode->i_rdev), MINOR(inode->i_rdev)); 
    if (open_nr == 0) { 
        open_nr++; 
        try_module_get(THIS_MODULE); 
            return 0; 
    }  
    else { 
            printk(KERN_ALERT "Another process open the char device.\n");
            return -1; 
        }  
} 
 
static ssize_t devsd_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos) 
{ 
    if (copy_to_user(buf, buffer, sizeof(buffer)))
    { 
        return -1; 
    } 
    return sizeof(buffer); 
} 
 
static ssize_t devsd_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos) 
{ 
    if (copy_from_user(buffer, buf, sizeof(buffer)))
    { 
        return -1; 
    } 
    return sizeof(buffer); 
} 
 
static int devsd_release(struct inode *inode, struct file* filp) 
{ 
    open_nr--;
    printk("The device is released!\n"); 
    module_put(THIS_MODULE); 
        return 0; 
} 
 
*/
static int __init devsd_init(void) 
{ 
    printk("<1>Self destruct sequence initiated.");
    return 0;
    /*
    int result; 
 
    printk(KERN_ALERT "Self destruct device init.");

    result = register_chrdev(DEVICE_NUM, "devsd", &devsd_fops); 
 
    if (result < 0) { 
        printk(KERN_WARNING "devsd: register failure\n"); 
        return -1; 
    } 
    else { 
        printk("devsd: register success!\n"); 
        device_num = result; 
        return 0; 
    } 
    */
} 
 
static void __exit devsd_exit(void) 
{ 
    printk("<1>Self destruct sequence terminated.");
    /*
    printk(KERN_ALERT "Unloading...\n"); 
    unregister_chrdev(device_num, "devsd");
    printk("unregister success!\n"); 
    */
} 




module_init(devsd_init); 
module_exit(devsd_exit); 
MODULE_LICENSE("GPL");
