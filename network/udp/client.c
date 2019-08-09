#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
int client_build(unsigned int port, const char * addr)
{
    char msg[20] = "it is test";
    int sockfd;
    struct sockaddr_in  serveraddr;
    socklen_t sock_len = sizeof(struct sockaddr);
    sockfd = socket(AF_INET,  SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket error\n");
        exit(-1);
    }
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port   = htons(port);
    serveraddr.sin_addr.s_addr = inet_addr(addr);
    while(1) {
        sendto(sockfd,msg, strlen(msg),0,(struct sockaddr*)&serveraddr, sock_len);
        sleep(1);
    }


}


int main (int argc , char **argv)
{
    if (argc == 3) {
        client_build(atoi(argv[2]), argv[1]);
    } else {
        printf("input fomat: <ip> <port>\n");
    }
}

