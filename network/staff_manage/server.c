#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
int main(void)
{
    int sockfd;
    int sock_client;
    int ret = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
        perror("socket failed\n");
        return -1;
    }
    struct sockaddr_in sockaddr;
    sockaddr.sin_addr.s_addr = inet_addr(SERVER_HOST_ADDR);
    sockaddr.sin_port = htons(8886);
    sockaddr.sin_family = AF_INET;
    if(0 > bind(sockfd,(struct sockaddr*)&sockaddr, sizeof(sockaddr))){

        perror("bind error\n");
        return -1;
    }
    if(0 > listen(sockfd, 20)) {
        perror("listen error\n");
        return -1;
    }
    fd_set select_fd, select_temp;
    int max_fd = sockfd;
    FD_ZERO(&select_fd);
    FD_SET(sockfd, &select_fd);
    struct sockaddr_in client;
    char buf[100];
    int i = 0;
    printf("ready...\n");
    while(1) {
        select_temp = select_fd;

        ret = select(max_fd + 1, &select_fd, NULL, NULL, NULL);
        printf("event start\n");
        if (ret == -1) {
            perror("select _error\n");
        } else if (ret == 0) {
            printf("data invalid\n");
        } else {
            if (FD_ISSET(sockfd, &select_fd)){
                sock_client = accept(sockfd,NULL, 0);
                max_fd = sock_client;
                FD_SET(sock_client, &select_fd);
                printf("client fd:%d sock:%d\n", sock_client, sockfd);
            }
            for(i = sockfd + 1; i < max_fd + 1; i++) {

                if(FD_ISSET(i, &select_fd)){
                    memset(buf, 0, 100);
                    read(i, buf, 100);
                    printf("recv:%s\n",buf);
                }
            }

        }
    }

}
