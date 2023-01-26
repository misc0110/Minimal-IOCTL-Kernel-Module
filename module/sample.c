#include "sample.h"
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/version.h>

MODULE_AUTHOR("Michael Schwarz");
MODULE_DESCRIPTION("Minimal IOCTL Kernel Module");
MODULE_LICENSE("GPL");

#define TAG "[sample-module] "

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
#define from_user raw_copy_from_user
#define to_user raw_copy_to_user
#else
#define from_user copy_from_user
#define to_user copy_to_user
#endif

static int device_open(struct inode *inode, struct file *file) {
  /* Lock module */
  try_module_get(THIS_MODULE);
  return 0;
}

static int device_release(struct inode *inode, struct file *file) {
  /* Unlock module */
  module_put(THIS_MODULE);
  return 0;
}

static long device_ioctl(struct file *file, unsigned int ioctl_num,
                         unsigned long ioctl_param) {
  switch (ioctl_num) {
  case MODULE_IOCTL_CMD_INC: {
    size_t val;
    // get value from user space
    from_user(&val, (void *)ioctl_param, sizeof(size_t));
    val++;
    // store value to user space
    to_user((void *)ioctl_param, &val, sizeof(size_t));
    return 0;
  }
  default:
    return -1;
  }

  return 0;
}

static struct file_operations f_ops = {.unlocked_ioctl = device_ioctl,
                                       .open = device_open,
                                       .release = device_release};

static struct miscdevice misc_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = MODULE_DEVICE_NAME,
    .fops = &f_ops,
    .mode = S_IRWXUGO,
};

int init_module(void) {
  int r;

  /* Register device */
  r = misc_register(&misc_dev);
  if (r != 0) {
    printk(KERN_ALERT TAG "Failed registering device with %d\n", r);
    return 1;
  }

  printk(KERN_INFO TAG "Loaded.\n");

  return 0;
}

void cleanup_module(void) {
  misc_deregister(&misc_dev);

  printk(KERN_INFO TAG "Removed.\n");
}
