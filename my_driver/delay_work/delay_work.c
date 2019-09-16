#include <linux/workqueue.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/init.h>
#define WORK_ON  1
struct work_struct work;
struct delayed_work delay_work;
struct timer_list  mytimer;
void work_timer_func(struct work_struct *work)
{
   printk("hello_word:%d\n",work->data);

 // schedule_delayed_work(&delay_work,  HZ * 2);
}

void timer_func(unsigned long data)
{
    if (data == 0) {
        schedule_work(&work);
    } else {
       schedule_delayed_work(&delay_work,  HZ);
    }
    mod_timer(&mytimer, jiffies + HZ);
}

static int __init work_timer_init(void)
{
    INIT_DELAYED_WORK(&delay_work, work_timer_func);

#if WORK_ON
    INIT_WORK(&work, work_timer_func);
    mytimer.expires = jiffies + HZ;
    mytimer.function = timer_func;
    mytimer.data = 20;
    init_timer(&mytimer);
    add_timer(&mytimer);
#endif

    return 0;
}

static void __exit work_timer_exit(void)
{
#if WORK_ON
    del_timer(&mytimer);
    cancel_work_sync(&work);
#endif
    cancel_delayed_work_sync(&delay_work);
}

module_init(work_timer_init);
module_exit(work_timer_exit);
MODULE_LICENSE("GPL");
