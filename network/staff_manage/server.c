#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include "server.h"

#if 1



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
    int epfd, epnfds,i;
    struct epoll_event event;
    struct epoll_event events[30];
    epfd = epoll_create(10);
    event.data.fd = sockfd;
    event.events = EPOLLIN | EPOLLET;
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event)== -1){
        perror("epoll ctl:");
        exit(1);
    }
    char buf[100];
    while(1) {
        epnfds = epoll_wait(epfd, events, 20, -1);
        if(epnfds == -1){
            perror("epoll wait");
            exit(-1);
        }
        for (i = 0; i < epnfds; ++i){
            if(events[i].data.fd == sockfd) {
                sock_client = accept(sockfd, NULL, 0);
                if (sock_client == -1) {
                    perror("accpt");
                    exit(-1);
                }
                event.data.fd = sock_client;
                event.events = EPOLLIN | EPOLLET;

                if(epoll_ctl(epfd, EPOLL_CTL_ADD, sock_client,&event) == -1){
                    perror("add client:");
                    exit(-1);
                }


            } else {
                memset(buf, 0, 100);
                if (recv(events[i].data.fd, buf,100,0) <= 0) {
                    printf("sock close:%d\n", events[i].data.fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, sock_client,&event);
                }
                else {
                    printf("buf:%s\n", buf);
                }
            }
        }
    }
}
#else
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
        printf("maxfd:%d\n",max_fd);
        ret = select(max_fd + 1, &select_temp, NULL, NULL, NULL);
        printf("event start\n");
        if (ret == -1) {
            perror("select _error\n");
            return -1;
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

                if(FD_ISSET(i, &select_temp)){
                    memset(buf, 0, 100);
                    printf("ready recv:%d \n",i);
                    if(read(i, buf, 100) <= 0) {
                        close(i);
                        FD_CLR(i, &select_fd);
                        printf("close sock:%d\n",i);
                    } else {
                       printf("recv:%s\n",buf);
                    }
                }
            }

        }
    }

}
#endif
