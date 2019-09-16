#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/slab.h>
#define  DRV_BUF_SIZE 128
#define  ADC_ADDR_BASE    0XC0053000

#define  ADCCON_SHIFT    0
#define  ADCDAT_SHIFT    1
#define  ADCINTENB_SHIFT 2
#define  ADCINTCLR_SHIFT 3
#define  PRESCALERCON_SHIFT 4

wait_queue_head_t adc_rq;
int flag = 0;
int major = 0;
static char  buf[DRV_BUF_SIZE] = {0};
unsigned int * adc_base = NULL;

irqreturn_t adc_handle(int irqno, void *devid)
{
    flag = 1;
    writel(1, adc_base + ADCINTCLR_SHIFT);
    wake_up_interruptible(&adc_rq);
    return IRQ_HANDLED;
}
static int drv_open(struct inode *node, struct file *filp)
{
    int ret;
    adc_base = ioremap(ADC_ADDR_BASE, 20);
    *(adc_base + ADCCON_SHIFT) &= ~(0Xf << 10); /*dleay*/
    *(adc_base + ADCCON_SHIFT) &= ~(0Xf << 10); /*dleay*/
    *(adc_base + ADCCON_SHIFT) |= (0X0 << 10); /*DLEAY*/
    *(adc_base + ADCCON_SHIFT) &= ~(0Xf << 6); /*asel*/
    *(adc_base + ADCCON_SHIFT) |= ( 12  << 6); /*adc clk tot*/
    *(adc_base + ADCCON_SHIFT) &= ~(0X7 << 3); /*asel*/
    *(adc_base + ADCCON_SHIFT) &= ~(0X1 << 2); /*stby*/
    *(adc_base + PRESCALERCON_SHIFT) &= ~(1 << 15); /*APEN*/
    *(adc_base + PRESCALERCON_SHIFT) &= ~(0X1FF << 0); /*PRE*/
    *(adc_base + PRESCALERCON_SHIFT) |= (199 << 0); /*PRE*/
    *(adc_base + PRESCALERCON_SHIFT) |= (1 << 15); /*APEN*/
    *(adc_base + ADCCON_SHIFT) |= (0X1 << 0); /*aden*/
    printk("adccon:%x,dat:%x,pre:%x\n",*(adc_base + ADCCON_SHIFT),*(adc_base + ADCDAT_SHIFT),*(adc_base + PRESCALERCON_SHIFT));
    while(*(adc_base + ADCCON_SHIFT) & 0X01) {};
    ret = request_irq(73, adc_handle,IRQF_DISABLED,"adc_demo", NULL);
    if (ret < 0) {
        printk("interrupt error\n");
        iounmap(adc_base);
    }
    printk("init done\n");
    return 0;
}

static ssize_t drv_write(struct file *file, const char __user *u_buf,size_t size, loff_t *offset)
{
    int ret = 0;
    printk("drv_demo write:%d %d %d \n", buf[0],buf[1],buf[2]);
    return size;
}
static ssize_t drv_read(struct file *file,char __user *u_buf, size_t size, loff_t *offset)
{

    int ret = 0;
    int adc;

    if (size > DRV_BUF_SIZE) {
        return -EINVAL;
    }
    *(adc_base + ADCCON_SHIFT) |= (0X1 << 0); /*aden*/
    ret =*(adc_base + ADCCON_SHIFT);

    wait_event(adc_rq, flag);
    flag = 0;
    adc = readl(adc_base + ADCDAT_SHIFT );
    ret = copy_to_user(u_buf, &adc, sizeof(adc));
    if (ret) {
        printk("copy to  user failed :%d\n", ret);
    }

   // printk("drv_demo read\n");
    return size;
}

static int drv_release(struct inode* node, struct file *filp)
{
    iounmap(adc_base);
    printk("drv_demo release\n");
    return 0;
}
static struct file_operations file_ops = {
    .open = drv_open,
    .write = drv_write,
    .read  = drv_read,
    .release = drv_release,
};
struct class *adc_cls;
struct device *device;
dev_t devno;
struct cdev *adc_dev;
static int __init adc_drv_init(void)
{
    int ret = 0;
    adc_dev = cdev_alloc();
    if (adc_dev == NULL) {
        ret = -ENOMEM;
        goto err1;
    }
    cdev_init(adc_dev,&file_ops);
    ret = alloc_chrdev_region(&devno,0, 1, "adc_dev");
    if (ret < 0) {
        goto err2;
    }
    ret = cdev_add(adc_dev, devno,1);
    if(ret ) {
        goto err3;
    }
    adc_cls = class_create(THIS_MODULE, "ADC_CLS");
    if (IS_ERR(adc_cls)) {
        ret = PTR_ERR(adc_cls);
        goto err4;
    }
    device  = device_create(adc_cls, NULL, devno, NULL,"adc_dev");

    if (IS_ERR(device)) {
        ret = PTR_ERR(device);
        goto err5;
    }
    init_waitqueue_head(&adc_rq);
    return 0;
err5:
    class_destroy(adc_cls);
err4:
    cdev_del(adc_dev);
err3:
    unregister_chrdev_region(devno,1);
err2:
    kfree(adc_dev);
err1:
   return ret;

}

static void __exit adc_drv_exit(void)
{
    device_destroy(adc_cls, devno);
    class_destroy(adc_cls);
    cdev_del(adc_dev);
    unregister_chrdev_region(devno,1);
    kfree(adc_dev);
    free_irq(73,NULL);
}

module_init(adc_drv_init)
module_exit(adc_drv_exit)

MODULE_LICENSE("GPL");
