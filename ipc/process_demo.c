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

void process_create (void)
{
    int pid;
    int var = 90;
    pid = fork();
    if (pid < 0) {
        perror("process create failed\n");
    } else if (pid > 0) {

        printf("I am parent!\n");
        printf(" chilld is %d!\n",pid);
    } else {
        var++;
        printf("I am child!\n");
    }
    printf("var is %d \n", var);

}

void wait_demo(void)
{
    int pid;
    int var = 90;
    int status;
    pid = fork();
    if (pid < 0) {
        perror("process create failed\n");
    } else if (pid ==  0) {

        printf("I am child!\n");
        sleep(4);
        exit(2);
    } else {
        var++;
        printf("I am parent!\n");
        wait(&status);
        printf("status:%d\n",status);
    }
    printf("var is %d \n", var);

}

int my_system(const char *p)
{
    int status;
    int pid = fork();
    if (pid < 0) {
        perror("fork failed\n");
    } else if (pid == 0) {
        //exec
        execl("/bin/sh","-c",p,NULL);
        _exit(2);
    } else {
        // ?
        wait(&status);
        printf("cmd finished\n");
    }
    return 0;
}

void my_shell(void)
{
    char buf[500]; //store input string
    int len = 0;
    char *parry[10];
    int  param_num = 0;
    int  i = 0, pid = 0;
    while(1){
        printf("#");
        fflush(stdout);
        sleep(1);
        len = read(STDIN_FILENO,buf,500);
        if (len > 0) {
            parry[param_num++] =&(buf[0]);
            for (i = 0; i < len - 1; i++) {
                if( isspace(buf[i])) {
                    buf[i] = '\0';
                    while(!isprint(buf[++i]));
                    parry[param_num++] = &(buf[i]);
#if DEBUG
                    printf("%d:%s\n",param_num-1,parry[param_num - 1]);
#endif
                }

            }
            buf[i] = '\0'; //string end
            parry[param_num] = NULL;
            pid = fork();
            if (pid < 0) {

                perror("fork proc error\n");
            } else if (pid == 0) {
#if DEBUG
                printf("param:%s,%s\n",parry[0],parry[1]);
#endif
                if(-1 ==  execvp(parry[0],parry)) {
                    printf("exec:%d\n",errno);
                }
                exit(2);
            } else {
                wait(NULL);
#if DEBUG
                printf("done\n");
#endif
            }
            param_num = 0;
        }
    }

}

int deamon_proc(void)
{
    int pid = 0;
    int i;
    FILE *fp = NULL;
    time_t now;
    struct tm *mytm;
    pid = fork();
    if (pid < 0) {
        perror("FORK ERROR");
        return -1;

    } else if (pid > 0) {
        return 0;
    }
    setsid();
    chdir("./");
    umask(0);
    //printf("close file\n");
    for(i = 0; i < getdtablesize();i++) {
        close(i);
    }
    fp = fopen("time_record.txt","a+");
    if (fp == NULL) {
        return -1;
    }
    while (1) {
        now = time(NULL);
        mytm = localtime(&now);
        fprintf(fp,"%d-%d-%d %d:%d:%d\n",mytm->tm_year+1900,mytm->tm_mon+1,\
			  mytm->tm_mday,mytm->tm_hour,mytm->tm_min,mytm->tm_sec);
        fflush(fp);
        sleep(1);
    }

}

