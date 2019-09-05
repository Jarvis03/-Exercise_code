
#include<stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
typedef struct {
    long type;
    char data[30];
} msg_t;
msg_t g_msgdata;
int main(void)
{
    int ikey;
    int msgid;
    ikey = ftok(".",33);
    if (ikey == -1) {
        perror("ERROR\n");
        return -1;
    }

    msgid = msgget(ikey,IPC_CREAT|0666);
    while(1) {
       msgrcv(msgid,&g_msgdata, sizeof(g_msgdata) - 4, 0,0);
       printf("%s\n",g_msgdata.data);
       sleep(1);

    }
    printf("done\n");
}
