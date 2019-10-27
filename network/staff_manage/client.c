#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include "server.h"
#define  OP_LOGIN  0
#define  OP_MENU   1


#define  CMD_MAX_SIZE  40

uint32_t  user_opcode = 0;
struct user user_info;
int sock;

int sock_read(char *buf, uint32_t len)
{

    return recv(sock, buf, len, 0);
}
int sock_write(char *buf, uint32_t len)
{

    return send(sock, buf, len, 0);
}
uint32_t get_opcode(void)
{
    char temp[20];
    printf("you choice:");
     fgets(temp, 20, stdin);
    return atoi(temp);

}
void ui_login(void)
{
    char temp[40];
    int ret = 1;
     printf("log in\nplease enter you name:");
     fgets(temp, 40, stdin);
     memcpy(user_info.name, temp, strlen(temp));
     printf("enter you password:");

     fgets(temp, 40, stdin);
     memcpy(user_info.name, temp, strlen(temp));
     printf("login....\n");
     sock_write("login scuess\n", 13);
     if (ret == 1) {
        user_opcode = OP_MENU;
     } else {
        user_opcode = OP_LOGIN;
     }
}
void ui_display_menu(void)
{
    printf("1. modify info\n");
    printf("2. display information\n");
    printf("3. quit\n");
    user_opcode = get_opcode();

}

int  main(void)
{


    int ret = 0;

    char str_cmd[CMD_MAX_SIZE];
#if 1
    sock = socket(AF_INET, SOCK_STREAM, 0 );
    if (sock == -1) {
        perror("socket create failed!\n");
        return -1;
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_HOST_ADDR);
    server.sin_port = htons(8886);
    ret = connect(sock, (struct sockaddr*)&server, sizeof(server));
    if (ret == -1) {
        perror("socket connect failed!\n");
        return -1;
    }
    printf("conect sucess\n");
#endif
    while(1) {
        switch(user_opcode) {
            case OP_LOGIN:
                ui_login();
                break;
            case OP_MENU:
                ui_display_menu();
                break;
            default:
                break;
        }
    }
    return 0;
}
