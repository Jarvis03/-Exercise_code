#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <linux/in.h>
int server_build(void)
{
    int sockfd;
    int ret;
    struct sockaddr_in addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd) {
        perror("socket faild\n");
        exit(-1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr  = inet_addr("192.168.1.43");
    ret = bind(sockfd, (struct sockaddr*)&addr,sizeof(struct sockaddr));
    if (-1 == ret ) {
        perror("bind error\n");
        exit(-1);
    }
    if(listen(sockfd,8) < 0){
        perror("bind failed");
        exit(-1);
    }
    printf("listen ok\n");
    return 0;

}
int main ()
{
    server_build();

}
