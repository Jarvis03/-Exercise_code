
#include <stdio.h>
#include <unistd.h>
#include "process_demo.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
void pipe_demo (void)
{
    int pid;
    int var = 90;
    char buf[30];
    int pfd[2];
    int ret;
    ret =  pipe(pfd);
    if (ret <  0) {
        perror("pipe failed\n");
    }
    pid = fork();
    if (pid < 0) {
        perror("process create failed\n");
    } else if (pid ==  0) {

        printf("I am child!\n");
        while(1) {

            ret = read(pfd[0], buf, 20);
            printf("read:%d\n",ret);
            if (ret > 0) {
                printf("%s\n",buf);
            }
        }
    } else {

        printf("I am parent!\n");
        while(1) {
            strcpy(buf,"you are");
            write(pfd[1], buf, strlen(buf));
            printf("write done\n");
            sleep(1);
        }
    }
    printf("var is %d \n", var);

}
int main(void)
{
    pipe_demo();
}
