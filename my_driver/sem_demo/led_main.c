#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include "led_ctl.h"
char buf[3] = {0, 0, 0};
int main(void)
{
    int fd = 0;
    int i = 0;
    if (0 > (fd = open("/dev/led_ctl",O_RDWR))) {
        perror("open error\n");
        return 0;
    }
    while (1) {

       ioctl(fd, LED_RED_OFF);
       ioctl(fd, LED_RED_ON);
       ioctl(fd, ACCESS_R);
       ioctl(fd, ACCESS_W);
       sleep(1);


    }
    close(fd);
    return 0;

}
