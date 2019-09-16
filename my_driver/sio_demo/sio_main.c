#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>

char buf[100] = "It is a test\n";
int fd;
void signal_handler(int num)
{
    if(num == SIGIO) {
        memset(buf, 0, sizeof(buf));
        read(fd, buf,sizeof(buf));
        printf("data:%s\n",buf);
    }
}
int main(void)
{

    int i = 0;
    if (0 > (fd = open("/dev/signal_io",O_RDWR))) {
        perror("open error\n");
        return 0;
    }
    signal(SIGIO, signal_handler);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | FASYNC);
    fcntl(fd, F_SETOWN, getpid());
    while (1) {
        sleep(10);
        write(fd, "it is test\n", 11);
    }
    close(fd);
    return 0;

}
