#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <asm/io.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/sched.h>

#define  DRV_BUF_SIZE 128
#define  RED_GPIOA_ADDR_BASE    0XC001A000
#define  BLUE_GPIOB_ADDR_BASE   0XC001B000
#define  GREEN_GPIOC_ADDR_BASE  0XC001E000
#define  GPIO_OUT_SHITF    0
#define  GPIO_OUTEN_SHITF  1
#define  GPIO_ALTFN0_SHIFT 8
#define  GPIO_ALTFN1_SHIFT 9

#define  RED_GPIO_SHIFT    28
#define  BLUE_GPIO_SHIFT   12
#define  GREEN_GPIO_SHIFT  13


#define  RED_ALTFN1_SHIFT    24
#define  BLUE_ALTFN0_SHIFT   24
#define  GREEN_ALTFN0_SHIFT  26

#define CODE_ON_BOARD   0
#define  CLASS_NAME    "class_led_drv"
#define  DEV_NAME      "led_dev_"

struct class   *led_cls;
struct device  *led_dev;
struct cdev    *led_cdev;
dev_t led_dev_id;

int major = 0;
wait_queue_head_t wq;
int condition = 0;
static char buf[DRV_BUF_SIZE] = {0};

#if CODE_ON_BOARD
unsigned int * red_base = NULL;
unsigned int * blue_base = NULL;
unsigned int * green_base = NULL;
#endif

static int drv_open(struct inode *node, struct file *filp)
{
#if CODE_ON_BOARD
    red_base = ioremap(RED_GPIOA_ADDR_BASE, 40);
    blue_base = ioremap(BLUE_GPIOB_ADDR_BASE, 40);
    green_base = ioremap(GREEN_GPIOC_ADDR_BASE, 40);
    *(red_base + GPIO_OUT_SHITF) &= ~(1 << RED_GPIO_SHIFT);
    *(red_base + GPIO_OUTEN_SHITF) |= (1 << RED_GPIO_SHIFT);
    *(red_base + GPIO_ALTFN1_SHIFT) &= ~(0X3 << RED_ALTFN1_SHIFT);
    *(red_base + GPIO_ALTFN1_SHIFT) |= (0X0 << RED_ALTFN1_SHIFT);

    *(blue_base + GPIO_OUT_SHITF) &= ~(1 << BLUE_GPIO_SHIFT);
    *(blue_base + GPIO_OUTEN_SHITF) |= (1 << BLUE_GPIO_SHIFT);
    *(blue_base + GPIO_ALTFN0_SHIFT) &= ~(0X3 << BLUE_ALTFN0_SHIFT);
    *(blue_base + GPIO_ALTFN0_SHIFT) |= (0X1 << BLUE_ALTFN0_SHIFT);

    *(green_base + GPIO_OUT_SHITF) &= ~(1 << GREEN_GPIO_SHIFT);
    *(green_base + GPIO_OUTEN_SHITF) |= (1 << GREEN_GPIO_SHIFT);
    *(green_base + GPIO_ALTFN0_SHIFT) &= ~(0X3 << GREEN_ALTFN0_SHIFT);
    *(green_base + GPIO_ALTFN0_SHIFT) |= (0X0 << GREEN_ALTFN0_SHIFT);
#endif
    printk("open drv-demo\n");
    return 0;
}

static ssize_t drv_write(struct file *file, const char __user *u_buf,size_t size, loff_t *offset)
{
    int ret = 0;
    if (size > DRV_BUF_SIZE) {
        return -EINVAL;
    }
    condition = 1;
    wake_up_interruptible(&wq);
#if CODE_ON_BOARD
    ret = copy_from_user(buf, u_buf, size);
    if (ret) {
        printk("copy from user failed :%d\n", ret);
    }
    *(red_base + GPIO_OUT_SHITF) &= ~(1 << RED_GPIO_SHIFT);
    *(red_base + GPIO_OUT_SHITF) |= (buf[0] << RED_GPIO_SHIFT);
    *(blue_base + GPIO_OUT_SHITF) &= ~(1 << BLUE_GPIO_SHIFT);
    *(blue_base + GPIO_OUT_SHITF) |= (buf[1] << BLUE_GPIO_SHIFT);
    *(green_base + GPIO_OUT_SHITF) &= ~(1 << GREEN_GPIO_SHIFT);
    *(green_base + GPIO_OUT_SHITF) |= (buf[2] << GREEN_GPIO_SHIFT);
#endif
    printk("drv_demo write:%d %d %d \n", buf[0],buf[1],buf[2]);
    return 0;
}
static ssize_t drv_read(struct file *file,char __user *u_buf, size_t size, loff_t *offset)
{
    int ret = 0;
    if (size > DRV_BUF_SIZE) {
        return -EINVAL;
    }
    wait_event(wq, condition);
    condition = 0;
    ret = copy_to_user(u_buf, buf, size);
    if (ret) {
        printk("copy to  user failed :%d\n", ret);
    }

    printk("drv_demo read:%s\n",buf);
    return 0;
}

static int drv_release(struct inode* node, struct file *filp)
{
#if CODE_ON_BOARD
    iounmap(red_base);
    iounmap(blue_base);
    iounmap(green_base);
#endif
    printk("drv_demo release\n");
    return 0;
}

static unsigned int drv_poll(struct file *file, struct poll_table_struct *wait)
{
    unsigned int mask = 0;
    poll_wait(file, &wq, wait);
    if (condition) {

        mask |= POLL_IN;
    }

    return mask;


}
static struct file_operations file_ops = {
    .open = drv_open,
    .write = drv_write,
    .read  = drv_read,
    .release = drv_release,
    .poll = drv_poll,
};


#define  TOTAL_OF_DEV   3
static int __init drv_demo_init(void)
{
//    major = register_chrdev(major,"drv_demo",&file_ops);
    int ret = 0;
    int i = 0;
    init_waitqueue_head(&wq);
    led_cdev = cdev_alloc();
    if (led_cdev == NULL) {
        ret = -ENOMEM;
        printk("cdev_alloc failed:%x\n",led_cdev);
        goto err1;
    }
    cdev_init(led_cdev,&file_ops);
        ret = alloc_chrdev_region(&led_dev_id, 0, TOTAL_OF_DEV, CLASS_NAME);
        if(ret < 0) {
            goto err2;
        }
    ret = cdev_add(led_cdev, led_dev_id, TOTAL_OF_DEV);
    led_cls = class_create(THIS_MODULE, CLASS_NAME);
    if(IS_ERR(led_cls)) {
        ret = -1;
        goto err3;
    }
    major = MAJOR(led_dev_id);
    for (i = 0;  i < TOTAL_OF_DEV; i++) {
        led_dev = device_create(led_cls,NULL, MKDEV(major, i),NULL, "led_%d",i);  /* /proc/devices*/
        if(IS_ERR(led_cls)) {
            ret = -1;
            goto err4;
        }
    }
    if(IS_ERR(led_dev)) {
        ret = PTR_ERR(led_dev);
        goto err5;
    }
    printk("init done\n");
    return 0;
err5:

    printk("init error5\n");
err4:
    printk("init error4\n");
err3:
    printk("init error3\n");
err2:
  //  kfree(led_cdev);
    printk("init error2\n");
err1:
    printk("init error\n");
    return ret;
}

static void __exit drv_demo_exit(void)
{
    int i = 0;
    for (i = 0; i < TOTAL_OF_DEV; i++) {
        device_destroy(led_cls, MKDEV(major,i));
    }
    class_destroy(led_cls);
    cdev_del(led_cdev);
    unregister_chrdev_region(led_dev_id, TOTAL_OF_DEV);
    kfree(led_cdev);
  //  unregister_chrdev(major,"drv_demo");
}

module_init(drv_demo_init)
module_exit(drv_demo_exit)

MODULE_LICENSE("GPL");
