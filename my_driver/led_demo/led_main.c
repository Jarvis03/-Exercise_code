#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
char buf[3] = {0, 0, 0};
int main(void)
{
    int fd = 0;
    int i = 0;
    if (0 > (fd = open("./drv1demo",O_RDWR))) {
        perror("open error\n");
        return 0;
    }
    while (1) {

       write(fd,buf, sizeof(buf));
       sleep(1);
       memset(buf, 0, sizeof(buf));
       buf[i] = buf[i] ? 0 : 1;
       i++;
       if (i >= 3) {
           i = 0;
       }

    }
    close(fd);
    return 0;

}
