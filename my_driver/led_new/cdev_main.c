#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
char buf[100] = "It is a test\n";
int main(void)
{
    int fd = 0;
    int i = 0;
    if (0 > (fd = open("/dev/cdev0",O_RDWR))) {
        perror("open error\n");
        return 0;
    }
    while (1) {
       strcpy(buf,"you are a little boy\n");
       write(fd,buf, sizeof(buf));
       sleep(1);
       memset(buf, 0, sizeof(buf));
       read(fd, buf,sizeof(buf));
       printf("read:%s",buf);
    }
    close(fd);
    return 0;

}
