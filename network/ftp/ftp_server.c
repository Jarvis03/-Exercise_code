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

#define  CMD_TYPE_LIST      0x1
#define  CMD_TYPE_DOWNLOAD  0x2
#define  CMD_TYPE_UPLOAD    0x3
#define  CMD_TYPE_QUIT      0x4

#define  IP_ADDR "192.168.199.224"

#define  ERR_LOG(arg)  do { perror(arg); return (-1); } while(0)

#define  BUF_MAXSIZE    1024

typedef struct {
    int type;
    char param[20];
}cmd_type_t;

int file_list(int sockfd);
int cmd_type_get(char *buf);
int cmd_get(char *buffer, int len, cmd_type_t * cmd);
int ftp_server(void)
{
    int sockfd;
    int clientfd, len;
    char buf[BUF_MAXSIZE];
    struct sockaddr_in server_addr;
    unsigned short port = 8888;
    cmd_type_t cmd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        ERR_LOG("socket failed\n");
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(IP_ADDR);
    if(0 > bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
        ERR_LOG("bind failed\n");
    }
    if ((clientfd = accept(sockfd, NULL, 0)) < 0) {
        ERR_LOG("accept failed\n");
    }
    while(1) {
        len = recv(clientfd, buf, sizeof(buf), 0);
        if (len < 0) {
            ERR_LOG("recv failed\n");
        }
        if (len == 0) {
            printf("client close\n");
            break;
        }
        cmd_get(buf, len, &cmd);
        switch(cmd.type) {
            case CMD_TYPE_LIST:
            break;
            case CMD_TYPE_DOWNLOAD:
            break;
            case CMD_TYPE_UPLOAD:
            break;
            case CMD_TYPE_QUIT:
            break;
            default:
            break;
        }
    }
    close(sockfd);
    close(clientfd);


}

int file_list(int sockfd)
{
    char buf[30];
    char sock_buf[500];
    DIR *dir;
    struct dirent *entry;
    int i = 0;
    if (getcwd(buf, 30) == NULL) {
        ERR_LOG("get current path error\n");
    }
    dir = opendir(buf);
    if(dir == NULL) {
        ERR_LOG("opendir error\n");
    }
    while((entry = readdir(dir)) != NULL) {
       memcpy(&buf[i], entry->d_name, strlen(entry->d_name) + 1);
       i += strlen(entry->d_name) + 1;
    }
    send(sockfd, buf,i,0);
    send(sockfd, TRANS_OVER, strlen(TRANS_OVER),0);
    return 0;

}
int cmd_get(char *buffer, int len, cmd_type_t * cmd)
{
    char buf[20];

    int i = 0;
    int  index = 0;
    for (i = 0; i < len; i++){
        if(buf[i] = ' ') {
            index = i + 1;
            /* 多个空格会bug  */
        }
    }
    i = 0;
    if (index != 0){
        memcpy(cmd->param, &buf[index], len - index);
        memcpy(buf, buffer, index);
        buf[index] = '\0';
    } else {
        memcpy(buf, buffer,len);
    }
    cmd->type = cmd_type_get(buf);
    return 0;
}

int cmd_type_get(char *buf)
{
    if(0 == strcmp(buf, CMD_LIST)) {
        return CMD_TYPE_LIST;
    }
    if (0 == strcmp(buf, CMD_DOWNLOAD)) {
        return CMD_TYPE_DOWNLOAD;
    }
    if (0 == strcmp(buf, CMD_UPLOAD)) {
        return CMD_TYPE_UPLOAD;
    }
    if (0 == strcmp(buf, CMD_QUIT)) {
        return CMD_TYPE_QUIT;
    }
    return 0;
}
int main (void)
{

    return 0;
}





/* end of file */
