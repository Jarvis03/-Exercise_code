
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
//#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define  IP_ADDR    "192.168.1.43"
int client(unsigned short port,const char *ipaddr)
{

    int sockfd;
    sockfd = socket(AF_INET,SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket create faild\n");
        exit(-1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    addr.sin_addr.s_addr = inet_addr(ipaddr);
    if(connect(sockfd,(const struct sockaddr*)&addr,sizeof(addr)) != 0) {
        perror("connect error\n");
        exit(-1);
    }
    char buf[20] = {"it is test"};
    int ret;
    while(1) {
        fgets(buf,20,stdin);
        buf[strlen(buf) - 1] = '\0';
        printf("send:%s\n",buf);
        send(sockfd, buf, strlen(buf),0);
        memset(buf,0 , 20);
        ret =  recv(sockfd,buf,20,0);
        if(ret > 0){
            printf("recv:%s\n",buf);
        }
    }
    return 0;
}
int main(int argc, char **argv)
{
    unsigned short port = 8888;
    if(argc == 3) {
        port = atoi(argv[2]);
        client(port,argv[1]);

    } else {
        printf("input: <IP> <port>\n");
    }
}

