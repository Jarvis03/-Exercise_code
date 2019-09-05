#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <asm/io.h>

#define  DRV_BUF_SIZE 128
#define  ADC_ADDR_BASE    0XC0053000

#define  ADCCON_SHIFT    0
#define  ADCDAT_SHIFT    1
#define  ADCINTENB_SHIFT 2
#define  ADCINTCLR_SHIFT 3
#define  PRESCALERCON_SHIFT 4


int major = 0;
static char  buf[DRV_BUF_SIZE] = {0};
unsigned int * adc_base = NULL;


static int drv_open(struct inode *node, struct file *filp)
{
    adc_base = ioremap(ADC_ADDR_BASE, 20);
    *(adc_base + ADCCON_SHIFT) &= ~(0Xf << 10); /*dleay*/
    *(adc_base + ADCCON_SHIFT) |= (0X2 << 10); /*DLEAY*/
    *(adc_base + ADCCON_SHIFT) &= ~(0Xf << 6); /*asel*/
    *(adc_base + ADCCON_SHIFT) |= ( 12  << 6); /*adc clk tot*/
    *(adc_base + ADCCON_SHIFT) &= ~(0X7 << 3); /*asel*/
    *(adc_base + ADCCON_SHIFT) &= ~(0X1 << 2); /*stby*/
    *(adc_base + PRESCALERCON_SHIFT) &= ~(1 << 15); /*APEN*/
    *(adc_base + PRESCALERCON_SHIFT) &= ~(0X1FF << 0); /*PRE*/
    *(adc_base + PRESCALERCON_SHIFT) |= (100 << 0); /*PRE*/
    *(adc_base + PRESCALERCON_SHIFT) |= (1 << 15); /*APEN*/
    *(adc_base + ADCCON_SHIFT) |= (0X1 << 0); /*aden*/
    printk("adccon:%x,dat:%x,pre:%x\n",*(adc_base + ADCCON_SHIFT),*(adc_base + ADCDAT_SHIFT),*(adc_base + PRESCALERCON_SHIFT));
    while(*(adc_base + ADCCON_SHIFT) & 0X01) {};
    printk("init done\n");
    return 0;
}

static ssize_t drv_write(struct file *file, const char __user *u_buf,size_t size, loff_t *offset)
{
    int ret = 0;
    printk("drv_demo write:%d %d %d \n", buf[0],buf[1],buf[2]);
    return 0;
}
static ssize_t drv_read(struct file *file,char __user *u_buf, size_t size, loff_t *offset)
{

    int ret = 0;
    volatile unsigned int adc;
    if (size > DRV_BUF_SIZE) {
        return -EINVAL;
    }
    adc = *((volatile unsigned int*)adc_base + ADCDAT_SHIFT);
    printk("raw_adc:%x\n",adc);
    adc = *(adc_base + ADCDAT_SHIFT) & 0XFFFF;
    buf[0] = adc & 0xff;
    buf[1] = adc >> 8 & 0xff;
    buf[2] = adc >> 16 & 0xff;
    ret = copy_to_user(u_buf, buf, size);
    if (ret) {
        printk("copy to  user failed :%d\n", ret);
    }

    printk("drv_demo read\n");
    return 0;
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
static int __init adc_drv_init(void)
{
    major = register_chrdev(major,"adc_demo",&file_ops);
    if(major < 0) {
        printk("register_chrdev error\n");
        return major;
    }
    return 0;
}

static void __exit adc_drv_exit(void)
{
    unregister_chrdev(major,"drv_demo");
}

module_init(adc_drv_init)
module_exit(adc_drv_exit)

MODULE_LICENSE("GPL");
