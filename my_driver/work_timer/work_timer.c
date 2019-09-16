#include <linux/workqueue.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/init.h>
struct work_struct work;
struct timer_list  mytimer;
void work_timer_func(struct work_struct *work)
{
   printk("hello_word:%d\n",jiffies & 0xffffffff);
}

void timer_func(unsigned long data)
{
    schedule_work(&work);
    mod_timer(&mytimer, jiffies + HZ);
}

static int __init work_timer_init(void)
{
    INIT_WORK(&work, work_timer_func);

    mytimer.expires = jiffies + HZ;
    mytimer.function = timer_func;
    mytimer.data = 0;
    init_timer(&mytimer);
    add_timer(&mytimer);
    return 0;
}

static void __exit work_timer_exit(void)
{
    del_timer(&mytimer);
    cancel_work_sync(&work);
}

module_init(work_timer_init);
module_exit(work_timer_exit);
MODULE_LICENSE("GPL");
