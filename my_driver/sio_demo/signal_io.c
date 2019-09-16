#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <asm/io.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/poll.h>
#define  DRV_BUF_SIZE  128


#define  DEV_NAME      "signal_io"  // drives create name
#define  CLASS_NAME    DEV_NAME  // class create name
char buf[DRV_BUF_SIZE] = {0};
struct cdev   *cdev;
struct class  *cdev_cls;
struct device *c_device;
struct fasync_struct *fapp;
dev_t  devno;    // 设备号
int major = 0;   // 主设备号

static int drv_open(struct inode *node, struct file *filp)
{
    printk("open cdrv-demo\n");
    return 0;
}

static ssize_t drv_write(struct file *file, const char __user *u_buf,size_t size, loff_t *offset)
{
    int ret = 0;
    if (size > DRV_BUF_SIZE) {
        return -EINVAL;
    }
    ret = copy_from_user(buf, u_buf, size);
    if (ret) {
        printk("copy from  user failed :%d\n", ret);
        return -EBUSY;
    }
    kill_fasync(&fapp, SIGIO, POLLIN);
    printk("cdrv_demo write:%s\n",buf);
    return size;
}
static ssize_t drv_read(struct file *file,char __user *u_buf, size_t size, loff_t *offset)
{
    int ret = 0;
    if (size > DRV_BUF_SIZE) {
        return -EINVAL;
    }
    ret = copy_to_user(u_buf, buf, size);
    if (ret) {
        printk("copy to  user failed :%d\n", ret);
        return -EAGAIN;
    }
    printk("cdrv_demo read:%s\n",buf);
    return size;
}

static int drv_release(struct inode* node, struct file *filp)
{
    printk("cdrv_demo release\n");
    return 0;
}

static int drv_fasync(int fd, struct file* file, int on)
{
    return fasync_helper(fd, file, on, &fapp);
}
static struct file_operations file_ops = {
    .open = drv_open,
    .write = drv_write,
    .read  = drv_read,
    .fasync = drv_fasync,
    .release = drv_release,
};

#define  TOTAL_OF_DEV   1
static int __init drv_demo_init(void)
{
    int ret = 0;
    int i = 0;
    //1. 分配对象
    cdev = cdev_alloc();
    if (cdev == NULL) {
         ret = -ENOMEM;
         goto err1;
    }
    //2. 初始化
    cdev_init(cdev,&file_ops);
    //3.设备号请
    if (major > 0) {
        ret = register_chrdev_region(MKDEV(major, 0),1, DEV_NAME);
    } else {
        ret = alloc_chrdev_region(&devno, 0, TOTAL_OF_DEV, CLASS_NAME);
    }
    if(ret < 0) {
        goto err2;
    }

    //4. 注册
    ret = cdev_add(cdev, devno, TOTAL_OF_DEV);
    if (ret) {
        printk("char device register failed\n");
        goto err3;
    }
    cdev_cls = class_create(THIS_MODULE, CLASS_NAME);
    if(IS_ERR(cdev_cls)) {
        ret = PTR_ERR(cdev_cls);
        goto err4;
    }
    major = MAJOR(devno);
    for (i = 0;  i < TOTAL_OF_DEV; i++) {
        c_device = device_create(cdev_cls,NULL, MKDEV(major, i),NULL, DEV_NAME);  /* /proc/devices*/
        if(IS_ERR(c_device)) {
            ret = PTR_ERR(c_device);
            goto err5;
        }
    }
    printk("init done\n");
    return 0; //一定要有
err5:
    for (--i; i >= 0; i--) {
        device_destroy(cdev_cls,MKDEV(major,i));
    }
    class_destroy(cdev_cls);
err4:
    cdev_del(cdev);
err3:
    unregister_chrdev_region(MKDEV(major,0),TOTAL_OF_DEV);
err2:
    kfree(cdev);
err1:

    return ret;
}

static void __exit drv_demo_exit(void)
{
    int i = 0;
    for (i = 0; i < TOTAL_OF_DEV; i++) {
        device_destroy(cdev_cls, MKDEV(major,i));
    }
    class_destroy(cdev_cls);
    cdev_del(cdev);
    unregister_chrdev_region(devno, TOTAL_OF_DEV);
    kfree(cdev);
}

module_init(drv_demo_init)
module_exit(drv_demo_exit)

MODULE_LICENSE("GPL");
