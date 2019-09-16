#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

struct resource * res;
int irqno;
int drv_probe(struct platform_device *pdev)
{
    res = platform_get_resource(pdev, IORESOURCE_MEM,0);
    if (res == NULL) {
        printk("get memory failed\n");
        return -ENOMEM;
    }
    irqno = platform_get_irq(pdev,0);
    if (irqno <  0) {
        printk("get irq failed\n");
        return irqno;
    }
    printk("addr :%x,irq:%d\n",res->start, irqno);
    return 0;
}
int drv_remove(struct platform_device *dev)
{
    printk("it is remove\n");
    return 0;
}
struct platform_driver pdrv = {
    .probe  = drv_probe ,
    .remove = drv_remove,
    .driver ={
        .name = "platform_dev",
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
