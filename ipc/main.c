#include <stdio.h>
#include "process_demo.h"

int main (int argc, char** argv[])
{
    int  number = 0;
    printf("start up\n");
    printf("1.my_shell\n");
    printf("2.wait demo\n");
    printf("3.deamon demo\n");
    printf("select program:[number]\n");
    scanf("%d",&number);
    switch(number) {
        case 1:
            my_shell();
            break;
        case 2:
            wait_demo();
            break;
        case 3:
            deamon_proc();
            break;
        default:
            printf("No such option\n");
            break;
    }
    printf("end\n");
    return 0;
}
