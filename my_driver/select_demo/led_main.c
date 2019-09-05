#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>
char buf[128] = {0, 0, 0};
int main(void)
{
    int fd1, fd2, ret;
    int i = 0;
    fd_set fset;

    if (0 > (fd1 = open("/dev/led1_0",O_RDWR))) {
        perror("open error\n");
        return 0;
    }
    if (0 > (fd2 = open("/dev/led_0",O_RDWR))) {
        perror("open error\n");
        return 0;
    }
    while (1) {
        FD_ZERO(&fset);
        FD_SET(fd1, &fset);
        FD_SET(fd2, &fset);
        select(fd2 +1,&fset,NULL,NULL,NULL);
        for(i = 3; i < fd2 + 1; i++) {
            if(FD_ISSET(i, &fset)) {
                read(i ,buf,sizeof(buf));
                printf("buf:%s",buf);
            }
        }

    }
    close(fd1);
    return 0;

}
