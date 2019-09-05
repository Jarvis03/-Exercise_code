#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <asm/io.h>

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

int major = 0;
static char buf[DRV_BUF_SIZE] = {0};
unsigned int * red_base = NULL;
unsigned int * blue_base = NULL;
unsigned int * green_base = NULL;

spinlock_t  led_lock;
int flag = 0;

static int drv_open(struct inode *node, struct file *filp)
{
    spin_lock(&led_lock);
    if (flag) {
        spin_unlock(&led_lock);

        return -1;

    }
    spin_unlock(&led_lock);
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
    printk("open drv-demo\n");
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
        printk("copy from user failed :%d\n", ret);
    }
    *(red_base + GPIO_OUT_SHITF) &= ~(1 << RED_GPIO_SHIFT);
    *(red_base + GPIO_OUT_SHITF) |= (buf[0] << RED_GPIO_SHIFT);
    *(blue_base + GPIO_OUT_SHITF) &= ~(1 << BLUE_GPIO_SHIFT);
    *(blue_base + GPIO_OUT_SHITF) |= (buf[1] << BLUE_GPIO_SHIFT);
    *(green_base + GPIO_OUT_SHITF) &= ~(1 << GREEN_GPIO_SHIFT);
    *(green_base + GPIO_OUT_SHITF) |= (buf[2] << GREEN_GPIO_SHIFT);

    printk("drv_demo write:%d %d %d \n", buf[0],buf[1],buf[2]);
    return 0;
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
    }

    printk("drv_demo read\n");
    return 0;
}

static int drv_release(struct inode* node, struct file *filp)
{
    iounmap(red_base);
    iounmap(blue_base);
    iounmap(green_base);
    printk("drv_demo release\n");
    flag = 0;
    return 0;
}
static struct file_operations file_ops = {
    .open = drv_open,
    .write = drv_write,
    .read  = drv_read,
    .release = drv_release,
};
static int __init drv_demo_init(void)
{
    major = register_chrdev(major,"drv_demo",&file_ops);
    if(major < 0) {
        printk("register_chrdev error\n");
        return major;
    }

    spin_lock_init(&led_lock);
    return 0;
}

static void __exit drv_demo_exit(void)
{
    unregister_chrdev(major,"drv_demo");
}

module_init(drv_demo_init)
module_exit(drv_demo_exit)

MODULE_LICENSE("GPL");
