#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <dirent.h>
#include <arpa/inet.h>
#define  CMD_LIST      "list"
#define  CMD_DOWNLOAD "download"
#define  CMD_UPLOAD   "upload"
#define  CMD_QUIT     "quit"


#define TRANS_OVER   "transover"

int ftp_client(void)
{
    char buf[50];
    display_menu();
    fget(buf, 50, stdin);
    buf[strlen(buf) - 1] = '\0';


}

void display_menu(void)
{
    printf("--------------------\n");
    printf("select you opreation:\n");
    printf("1.list\n");
    printf("2.upload filename\n");
    printf("3.download filename\n");
    printf("4.quit\n");
    printf("--------------------\n");
}

