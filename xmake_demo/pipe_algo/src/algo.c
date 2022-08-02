#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
static pthread_t algo_thread;
#if 1
#define PIPE_NAME "/home/guo/code_test/myfifo"
static void* thread_entery(void* arg)
{
    int cnt = 0;
    int fd;
    char buf[150];
    fd = open(PIPE_NAME, O_WRONLY);     //打开fifo文件
    printf("algo start pipe\n");
    while(1) {
        cnt++;
        
		sprintf(buf,"count [%d]\n", cnt);
		printf("algo[%s]\n",buf);
        write( fd, buf, 11);
		sleep(1);
    };

}
#else
static void* thread_entery(void* arg)
{
    int cnt = 0;
    printf("algo start pipe\n");
    while(1) {
        cnt++;
        
		printf(buf,"count [%d]\n", cnt);
		sleep(1);
    };
}
#endif


int test_algo(int arg)
{
    int ret = pthread_create(&algo_thread, NULL, thread_entery, NULL);
    if (-1 == ret) {
        printf("algo thread create failed\n");
        return -1;
    }
}
