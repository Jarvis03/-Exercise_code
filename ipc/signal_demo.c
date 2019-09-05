#include <unistd.h>
#include <stdio.h>
#include <signal.h>
typedef void(*sighandler_t)(int arg);
void signal_proc(void);
sighandler_t hand_ret;

void signal_proc(void);

int main(void)
{

    alarm(3);
    /*pause();*/
    signal_proc();
#if __USR_GNU
    printf("is gnu\n");
#endif
    while(1) {
        sleep(2);
        printf("wake up\n");
    }
    return 0;
}


void handler(int arg)
{

    if(arg == SIGINT) {

        printf("it  is sigint\n");


        signal(SIGINT, hand_ret);
        printf("set sigint default\n");
    } else if (arg == SIGQUIT) {
        printf("it is quit\n");
    } else if (arg == SIGALRM) {
        printf("it is alarm\n");
    }
}


void signal_proc(void)
{

   hand_ret = signal(SIGINT, handler);
    signal(SIGQUIT,handler);
    signal(SIGALRM,handler);
}
