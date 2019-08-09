#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
//#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define  IP_ADDR    "192.168.1.43"
int server_build(void)
{
    int sockfd, socknew;
    sockfd = socket(AF_INET,SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket create faild\n");
        exit(-1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(8885);
    addr.sin_addr.s_addr = inet_addr(IP_ADDR);
    if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr) ) == -1) {
        perror("bind error\n");
        exit(-1);
    }
    if (listen(sockfd, 8) == -1) {
        perror("listen failed\n");
        exit(-1);
    }
    socknew = accept(sockfd, NULL, 0);
    if(socknew < 0) {
        perror("accept failed\n");
        exit(-1);
    }
    char buf[20];
    int len;
    printf("sock is ready\n");
    while(1) {
        memset(buf,0,20);
        printf("clear:%s\n",buf);
        len = recv(socknew, buf,20, 0);
        if (len > 0) {
            printf("len:%d:%s\n",len,buf);
            if(strcmp(buf,"nihao") == 0) {
                printf("send :\n");
                send(socknew,"nihao!!!", 9,0);
            }
        } else if (len == 0) {
            printf("close and reconnect\n");
            socknew = accept(sockfd, NULL, 0);
            printf("reconnecting\n");
        } else {
            perror("recv error\n");
            break;
        }
    }
    return 0;
}

int main(void)
{

    server_build();
}
