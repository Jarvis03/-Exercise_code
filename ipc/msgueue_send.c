#include<stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
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
    g_msgdata.type = 1;
    strcpy(g_msgdata.data,"it is you\n");
    msgsnd(msgid,&g_msgdata, sizeof (g_msgdata)-sizeof(long),0);
}
