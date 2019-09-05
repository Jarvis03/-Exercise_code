#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#define INIT_MONEY  5000
sem_t  my_sem;
int card_account = INIT_MONEY;
int pass_book    = INIT_MONEY;
char buf[30];
void* product (void * arg)
{

    while(1) {
        fgets(buf, 20, stdin);
        sem_post(&my_sem);
        sleep(1);

    }
}


void * customer (void * arg)
{
    while (1) {

        sem_wait(&my_sem);
        printf("%s\n",buf);
        sleep(1);
    }
}


int create_thread(void)
{
    pthread_t tid_time, tid_input;
    sem_init(&my_sem, 0, 0);
    int ret, i;
        if (0 !=pthread_create(&tid_time, NULL, product, NULL)){
            printf("print time create failed:\n");
            return -1;
        }
        if (0 !=pthread_create(&tid_input, NULL, customer, NULL)){
            printf("print time create failed:\n");
            return -1;
        }
    /*sleep(60);*/
    /*printf("end thread\n");*/
    /*pthread_cancel(tid_time);*/
    /*pthread_cancel(tid_input);*/
    return 0;

}
int main(void)
{
    printf("start\n");
    create_thread();
    for(;;) {

        sleep(2);
    }
    return 0;

}
