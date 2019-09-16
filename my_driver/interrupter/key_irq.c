#include <linux/timer.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#define  GPIO(m,n) (m * 32 + n)

#define GPIOB8  GPIO(1,8)
#define GPIOB16 GPIO(1,16)

int gpiono[] = {GPIOB8, GPIOB16};

struct timer_list key_timer;
char *irq_name[] = {"interrupt-b8","interrupt-b16"};
irqreturn_t key_irq_handle(int num, void *dev)
{
    // 启动定时器
    mod_timer(&key_timer, jiffies + 10);
    return IRQ_HANDLED;
}
void key_timer_func(unsigned long data)
{
    int status_b8 = gpio_get_value(GPIOB8);
    int status_b16 = gpio_get_value(GPIOB16);
    if (status_b8 == 0) {
        printk("left button down....\n");
    }
    if (status_b16 == 0) {
        printk("right button down ----\n");
    }

}
static int __init timer_init(void)
{
    int i , ret;
    key_timer.expires = jiffies + 10;
    key_timer.function = key_timer_func;
    key_timer.data = 0;
    init_timer(&key_timer);
    add_timer(&key_timer);

    for (i = 0; i < ARRAY_SIZE(gpiono); i++) {
        ret = request_irq(gpio_to_irq(gpiono[i]),key_irq_handle,IRQF_TRIGGER_FALLING,irq_name[i], NULL);
        if (ret) {
            printk("gpio request %d error", gpiono[i]);
            return ret;
        }

     }
    return 0;
}

static void __exit timer_exit(void)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(gpiono); i++) {
        free_irq(gpio_to_irq(gpiono[i]),NULL);
    }
    del_timer(&key_timer);
}

module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");
