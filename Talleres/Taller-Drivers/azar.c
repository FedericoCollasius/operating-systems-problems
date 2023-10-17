#include <linux/init.h>
#include <linux/module.h> // THIS_MODULE
#include <linux/kernel.h> // Kernel cosas
#include <linux/fs.h>     // File operations
#include <linux/cdev.h>   // Char devices
#include <linux/device.h> // Nodos filesystem
#include <linux/uaccess.h> // copy_to_user
#include <linux/slab.h>    // kmalloc
#include <linux/random.h>  // get_random_bytes

static int upper_bound = -1;

static ssize_t azar_read(struct file *filp, char __user *data, size_t size, loff_t *offset) {
    unsigned int rand;
    unsigned char* buffer;
    size_t form_size, eff_size;
    if (upper_bound > 0) {
        buffer = kmalloc(size, GFP_KERNEL);
        get_random_bytes(&rand, sizeof(unsigned int));
        form_size = (size_t) snprintf(buffer, size, "%d\n", rand % upper_bound);
        eff_size = min(size, form_size);
        copy_to_user(data, buffer, eff_size);
        kfree(buffer);
        return eff_size;
    }
    return -EPERM;
}

static ssize_t azar_write(struct file *filp, const char __user *data, size_t size, loff_t *offset) {
    char *buffer = kmalloc(size + 1, GFP_KERNEL);
    copy_from_user(buffer, data, size);
    buffer[size] = 0;
    if (kstrtoint(buffer, 10, &upper_bound) == 0 && upper_bound >= 0) {
        kfree(buffer);
        return size;
    }
    kfree(buffer);
    return -EPERM;
}

static struct file_operations azar_fops = {
  .owner = THIS_MODULE,
  .read = azar_read,
  .write = azar_write,
};

static dev_t azar_devno;
static struct cdev azar_cdev;
static struct class *azar_class;

static int __init azar_init(void) {
    cdev_init(&azar_cdev, &azar_fops);
    alloc_chrdev_region(&azar_devno, 0, 1, "SO-azar");
    cdev_add(&azar_cdev, azar_devno, 1);

    azar_class = class_create(THIS_MODULE, "azar");
    device_create(azar_class, NULL, azar_devno, NULL, "azar");

    printk(KERN_ALERT "Loading module 'azar'\n");
    return 0;
}

static void __exit azar_exit(void) {
    printk(KERN_ALERT "Unloading module 'azar'\n");

    device_destroy(azar_class, azar_devno);
    class_destroy(azar_class);

    unregister_chrdev_region(azar_devno, 1);
    cdev_del(&azar_cdev);
}

module_init(azar_init);
module_exit(azar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Generador de números al azar");
