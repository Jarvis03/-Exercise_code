#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
static pthread_t algo_thread;

static void* thread_entery(void* arg)
{
    int cnt = 0;
    while(1) {
        cnt++;
        printf("count [%d]\n", cnt);
        sleep(1);
    };

}

int test_algo(int arg)
{
    int ret = pthread_create(&algo_thread, NULL, thread_entery, NULL);
    if (-1 == ret) {
        printf("algo thread create failed\n");
        return -1;
    }
}
