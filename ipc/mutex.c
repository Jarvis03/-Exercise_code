#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define INIT_MONEY  5000
pthread_mutex_t  my_lock;
int card_account = INIT_MONEY;
int pass_book    = INIT_MONEY;

void* take_money (void * arg)
{
    printf("thread in\n");
    while(1) {
        pthread_mutex_lock(&my_lock);
        card_account -= 2;
        usleep(100000);
        pass_book    -= 2;
        pthread_mutex_unlock(&my_lock);
        if (card_account != pass_book) {
            printf("card:%d,passbook:%d\n",card_account, pass_book);
        }
        sleep(1);

    }
}





int create_thread(void)
{
    pthread_t tid_time, tid_input;
    pthread_mutex_init(&my_lock, NULL);
    int ret, i;
    for (i = 0; i < 4; i++) {
        if (0 !=pthread_create(&tid_time, NULL, take_money, NULL)){
            printf("print time create failed:\n");
            return -1;
        }
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
