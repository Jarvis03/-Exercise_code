#include "test_pipe.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
static pthread_t algo_thread;
time_t timep;
struct tm *p;
#define DEBUG 1
#define print_log(fmt, args...) { \
    do \
    { \
        if (DEBUG == 1 ) \
        { \
            time(&timep); \
            p=localtime(&timep); \
            fprintf(stdout, "[%4d-%02d-%02d %02d:%02d:%02d] FILE: %s, LINE: %d, FUNC: %s.     ", \
            (1900+p->tm_year), \
            (1+p->tm_mon), \
            p->tm_mday,  \
            p->tm_hour, \
            p->tm_min,  \
            p->tm_sec, \
            __FILE__, \
            __LINE__,  \
            __FUNCTION__); \
           fprintf(stdout, fmt"\n", ##args); \
        } \
    } \
    while(0); \
}
#if  !TEST_PIPE
static void* thread_entery(void* arg)
{
    int cnt = 0;
    FILE *fp=NULL; 
	
	char buff[128]={0};   
	memset(buff,0,sizeof(buff)); 
	fp=popen("./pipe_algo","r");//将命令ls-l 同过管道读到fp 
    if (fp == NULL) {
        printf("popen error");
    } else {
        printf("popen ok \n");
    }
    while(1) {
        cnt++;
        print_log("pipe count [%d]\n", cnt);
        if (fgets(buff,127,fp) == NULL){
            print_log("fread error\n");
        }//将fp的数据流读到buff中
        else 
        {
            print_log("========\n{%s}\n", buff);
        }
        
        // if(fputs(buff, stdout) == EOF) {
        //     printf("fout error\n");
        // } 
        // sleep(1);
    };
    print_log("pipe exit\n");
    pclose(fp); 
}
#else 
#define PIPE_NAME "/home/guo/code_test/myfifo"
static void* thread_entery(void* arg)
{
    int fd;
    char buf[150];
    print_log("start pipe 111\n");
    if(access(PIPE_NAME,F_OK) != 0){//如果文件存在
        int err = mkfifo(PIPE_NAME,0777);
        if(err != 0){
				printf("Create fifo failed");
            return -1;
        }
    }
	system("./pipe_algo");
    fd = open(PIPE_NAME, O_RDONLY);     //打开fifo文件
    print_log("start pipe\n");
	while(1) {
	read(fd, buf, 11);                   //写入数据
    print_log("read result is [%s].\n",buf);
	}
	close(fd);                           //关闭fifo文件 
    unlink("./myfifo"); 
}
#endif
int test_pipe(int arg)
{
    int ret = pthread_create(&algo_thread, NULL, thread_entery, NULL);
    if (-1 == ret) {
        printf("pipe thread create failed\n");
        return -1;
    }
    printf("creat pipe done\n");
}
