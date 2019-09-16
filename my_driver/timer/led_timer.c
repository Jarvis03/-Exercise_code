#include <linux/timer.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>

#define  GPIO(m,n) (m * 32 + n)

#define GPIOA28 GPIO(0,28)
#define GPIOB12 GPIO(1,12)
#define GPIOE13 GPIO(4,13)

int gpiono[] = {GPIOA28, GPIOB12,GPIOE13};
struct timer_list led_timer;
void led_timer_func(unsigned long data)
{
    int red_status = gpio_get_value(GPIOA28);
    red_status = red_status ? 0 : 1;
    gpio_set_value(GPIOA28, red_status);
    mod_timer(&led_timer, jiffies + HZ);

}
static int __init timer_init(void)
{
    int i , ret;
    for (i = 0; i < ARRAY_SIZE(gpiono); i++) {
        gpio_free(gpiono[i]);
    }
    for (i = 0; i < ARRAY_SIZE(gpiono); i++) {
        ret = gpio_request(gpiono[i], NULL);
        if (ret) {
            printk("gpio request %d error", gpiono[i]);
            return ret;
        }
        gpio_direction_output(gpiono[i],0);
    }
    led_timer.expires = jiffies + HZ;
    led_timer.function = led_timer_func;
    led_timer.data = 0;
    init_timer(&led_timer);
    add_timer(&led_timer);
    return 0;
}

static void __exit timer_exit(void)
{
    int i;
    del_timer(&led_timer);
    for (i = 0; i < ARRAY_SIZE(gpiono); i++) {
        gpio_free(gpiono[i]);
    }
}

module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");
