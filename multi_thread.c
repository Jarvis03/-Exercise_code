#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
void* print_time(void * arg)
{
    FILE *fp = NULL;
    time_t now;
    struct tm *mytm;
    pthread_detach(pthread_self());
    fp = fopen ("time.log","a+");
    if (fp == NULL) {
        return NULL;
    }
    while(1){

        now = time(NULL);
        mytm = localtime(&now);
        fprintf(fp,"%d-%d-%d %d:%d:%d\n",mytm->tm_year+1900,mytm->tm_mon+1,\
			  mytm->tm_mday,mytm->tm_hour,mytm->tm_min,mytm->tm_sec);
        fflush(fp);
        sleep(1);
    }


}

void* terminal_input(void *arg)
{
    int fd,len;
    char buf[100];
    FILE *fp;

    pthread_detach(pthread_self());
    printf("input content to record\n");
    fp = fopen ("terminal.log","a+");
    if (fp == NULL) {
        return NULL;
    }


    while(1) {
        len = read(STDIN_FILENO,buf,100);
        if (len > 0) {
            printf("continue...\n");
           fwrite(buf, 1, len, fp);
           fflush(fp);
        }
        sleep(1);
    }

}

int create_thread(void)
{
    pthread_t tid_time, tid_input;
    int ret;
    if (0 !=pthread_create(&tid_time, NULL, print_time, NULL)){
        printf("print time create failed:\n");
        return -1;
    }
    ret = pthread_create(&tid_time, NULL, terminal_input, NULL);
    if(ret != 0) {
        printf("print time create failed:%s\n",strerror(ret));
        return -1;
    }
    sleep(60);
    printf("end thread\n");
    pthread_cancel(tid_time);
    pthread_cancel(tid_input);
    return 0;

}
int main(void)
{
    printf("start\n");
    create_thread();
    return 0;

}
