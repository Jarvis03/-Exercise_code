#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#define ADC_DEV_NAME    "plat_adc_name"
#define BUF_SIZE 128
#define ADC_ADDR_BASE 0XC005300

#define ADCCON_SHIFT  0
#define ADCDAT_SHIFT  1
#define ADCINTENB_SHIFT 2
#define ADCINTCLR_SHIFT 3
#define PRESCALERCON_SHIFT 4


struct adc_data {
    struct resource * res;
    unsigned int * adc_base;
    int irqno;
    wait_queue_head_t wq;
    int flag;
    int major;
    struct class *adc_cls;
    struct device *adc_dev;
};
struct adc_data *adc;
irqreturn_t adc_handler(int irqno,void *devid)
{
    adc->flag = 1;
    writel(1, adc->adc_base + ADCINTCLR_SHIFT);
    wake_up_interruptible(&adc->wq);
    return IRQ_HANDLED;
}
static int adc_open(struct inode* inode, struct file *filp)
{

    *(adc->adc_base + ADCCON_SHIFT) &= ~(0Xf << 10); /*dleay*/
    *(adc->adc_base + ADCCON_SHIFT) &= ~(0Xf << 10); /*dleay*/
    *(adc->adc_base + ADCCON_SHIFT) |= (0X0 << 10); /*DLEAY*/
    *(adc->adc_base + ADCCON_SHIFT) &= ~(0Xf << 6); /*asel*/
    *(adc->adc_base + ADCCON_SHIFT) |= ( 12  << 6); /*adc clk tot*/
    *(adc->adc_base + ADCCON_SHIFT) &= ~(0X7 << 3); /*asel*/
    *(adc->adc_base + ADCCON_SHIFT) &= ~(0X1 << 2); /*stby*/
    *(adc->adc_base + PRESCALERCON_SHIFT) &= ~(1 << 15); /*APEN*/
    *(adc->adc_base + PRESCALERCON_SHIFT) &= ~(0X1FF << 0); /*PRE*/
    *(adc->adc_base + PRESCALERCON_SHIFT) |= (199 << 0); /*PRE*/
    *(adc->adc_base + PRESCALERCON_SHIFT) |= (1 << 15); /*APEN*/
    *(adc->adc_base + ADCINTENB_SHIFT)   |= 0x01;
    return 0;
}
ssize_t adc_read(struct file *file, char __user* u_buf, size_t size,loff_t* loff)
{
    int data,ret;
    if(size > sizeof(data)) {
        return -EINVAL;
    }
    writel(readl(adc->adc_base + ADCCON_SHIFT)|0X01,adc->adc_base + ADCCON_SHIFT);
    ret = wait_event_interruptible(adc->wq, adc->flag);
    if (ret) {
        printk("wait failed\n");
        return ret;
    }
    data = readl(adc->adc_base + ADCDAT_SHIFT)& 0xfff;
    ret = copy_to_user(u_buf,&data,sizeof(data));
    if (ret ) {
        printk("copy to user failed\n");
        return ret;
    }
   adc->flag = 0;
    return sizeof(data);
}
ssize_t adc_write(struct file *file, char __user* u_buf, size_t size,loff_t*offs)
{

    return size;
}
int adc_close(struct inode *inode, struct file *file)
{
    writel(readl(adc->adc_base) | (1 << 2),adc->adc_base);
    return 0;
}

struct file_operations fops = {
    .open = adc_open,
    .read = adc_read,
    .write = adc_write,
};
int drv_probe(struct platform_device *pdev)
{
    int ret = 0;
    adc = kzalloc(sizeof(*adc),GFP_KERNEL);
    if (adc == NULL) {
        printk("kzalloc failed\n");
        ret = -ENOMEM;
        goto err1;
    }
    adc->res = platform_get_resource(pdev, IORESOURCE_MEM,0);
    if (adc->res == NULL) {
        printk("get resource memory failed\n");
        ret = -ENOMEM;
        goto err2;
    }
    adc->adc_base = ioremap(adc->res->start, (adc->res->end) - (adc->res->start));
    if(adc->adc_base == NULL) {
        printk("ioremap failed\n");
        ret = -EIO;
        goto err2;
    }
    adc->irqno = platform_get_irq(pdev,0);
    if (adc->irqno <  0) {
        printk("get irq failed\n");
        ret = adc->irqno;
        goto err3;
    }
    //register char device
    adc->major = register_chrdev(0,"platform_adc",&fops);
    if (adc->major < 0) {
        printk("register failed\n");
        ret = adc->major;
        goto err3;
    }
    ret = request_irq(adc->irqno,adc_handler,IRQF_DISABLED,"adc_irq",NULL);
    if (ret) {
        printk("register irq failed\n");
        goto err4;
    }
    adc->adc_cls = class_create(THIS_MODULE, ADC_DEV_NAME);
    if(IS_ERR(adc->adc_cls)) {
        printk("class create failed\n");
        ret = PTR_ERR(adc->adc_cls);
        goto err3;
    }
    adc->adc_dev = device_create(adc->adc_cls,NULL, MKDEV(adc->major,0), NULL,ADC_DEV_NAME);
    if(IS_ERR(adc->adc_dev)) {
        printk("class create failed\n");
        ret = PTR_ERR(adc->adc_dev);
        goto err5;
    }
    init_waitqueue_head(&adc->wq);
    return 0;
err5:
    class_destroy(adc->adc_cls);
err4:
    unregister_chrdev(MKDEV(adc->major,0),NULL);
err3:
    iounmap(adc->adc_base);
err2:
    kfree(adc);
err1:
    return ret;
}
int drv_remove(struct platform_device *dev)
{
    printk("it is remove\n");
    device_destroy(adc->adc_cls,MKDEV(adc->major,0));
    class_destroy(adc->adc_cls);
    unregister_chrdev(MKDEV(adc->major,0),ADC_DEV_NAME);
    free_irq(adc->irqno,NULL);
    iounmap(adc->adc_base);

    kfree(adc);
    return 0;
}
struct platform_driver pdrv = {
    .probe  = drv_probe ,
    .remove = drv_remove,
    .driver ={
        .name = "adc-dev",
        .owner = THIS_MODULE,
    },

};

static int __init pdrv_init(void)
{

    return platform_driver_register(&pdrv);
}

static void __exit pdrv_exit(void)
{
    platform_driver_unregister(&pdrv);
}

module_init(pdrv_init);
module_exit(pdrv_exit);
MODULE_LICENSE("GPL");
