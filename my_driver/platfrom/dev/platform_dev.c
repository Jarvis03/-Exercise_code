#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

struct resource  res[] = {
    [0] = {
        .start = 0xc001a000,
        .end   = 0xc001a000 + 19,
        .flags  = IORESOURCE_MEM,
    },
    [1] = {
        .start =73,
        .end   = 73,
        .flags  = IORESOURCE_IRQ,

    }
};
void pdev_release(struct device *dev)
{
    printk("it is release\n");
}
struct platform_device pdev = {
    .name = "platform_dev",
    .id   = -1,
    .dev ={
        .release = pdev_release,
    },
    .resource = res,
    .num_resources = ARRAY_SIZE(res),
};

static int __init pdev_init(void)
{

    return platform_device_register(&pdev);
}

static void __exit pdev_exit(void)
{
    platform_device_unregister(&pdev);
}

module_init(pdev_init);
module_exit(pdev_exit);
MODULE_LICENSE("GPL");
