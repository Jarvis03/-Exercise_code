#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include "server.h"
#define  OP_LOGIN  0
#define  OP_MENU   1


#define  CMD_MAX_SIZE  40

uint32_t  user_opcode = 0;
struct user user_info;
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

    int sock = 0;
    int ret = 0;

    char str_cmd[CMD_MAX_SIZE];
#if 0
    sock = socket(AF_INET, SOCK_STREAM, 0 );
    if (sock == -1) {
        perror("socket create failed!\n");
        return 0;
    }
    ret = connect(sock, SERVER_HOST_ADDR, sizeof(SERVER_HOST_ADDR));
    if (ret == -1) {
        perror("socket connect failed!\n");
        return 0;
    }
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
