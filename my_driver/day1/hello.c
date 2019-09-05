#include <linux/init.h>
#include <linux/module.h>
//#include <linux/kernel.h>
int a=2;
char b=3;
char *ptr = NULL;
module_param(a, int,   0664);
module_param(b, byte, 0775);
module_param(ptr,charp, 0775);
MODULE_PARM_DESC(a,"it is color");
/*
 * __init 将代码放到 init.text 段中
 */
static int __init hello_init(void)
{
    printk("val a = %d\n",a);
    printk("val b = %d\n",b);
    printk("val c = %s\n",ptr);
    printk(KERN_ERR "hello init\n");
    return 0;
}
static void __exit hello_exit(void)
{

    printk(KERN_ERR "hello exit\n");

}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
