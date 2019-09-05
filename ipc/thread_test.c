#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void* func(void * param)
{
    pthread_t tid;
    tid = pthread_self();
     printf("this is a thread:%d\n",tid);
    sleep(5);
    pthread_exit((void*)8);
}
void* func_cancal(void* arg)
{
    pthread_t pid = (pt)

}
int main(void)
{
    int ret = 0;
    int i = 0;
    void *pstr;
    pthread_t tid[10];
    for (i= 0; i < 10; i++) {
        ret = pthread_create(&tid[i], NULL,func,NULL);
        if (ret != 0) {
            printf("pthread create:%s",strerror(ret));
        }
    }
    for (i = 0; i < 10; i++) {
       ret =  pthread_join(tid[i],&pstr);
        if (ret != 0) {
            printf("pthread jion:%s",strerror(ret));
        }
        printf("jion:%d\n",(int)pstr);
    }
    while(1) {
        sleep(2);
    }

}
