#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<unistd.h>
#include"proto.h"

#define iP_SIZE  32

int main(int argc,char **argv){
    int socket_id;
    struct sockaddr_in local_addr, remote_addr;
    struct msg_st *pmsg;
    socklen_t remote_addr_len;
    char ipstr[iP_SIZE];
    size_t msg_size;

    socket_id = socket(AF_INET,SOCK_DGRAM,0);
    if(socket_id < 0){
        perror("socket() ");
        exit(0);
    }

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi(RCV_PORT));
    inet_pton(AF_INET,"0.0.0.0",&local_addr.sin_addr);

    if(bind(socket_id,(struct sockaddr *)&local_addr,sizeof(local_addr)) < 0){
        perror("bind() ");
        exit(0);
    }

    msg_size = sizeof(struct msg_st) + NAME_MAX - 1;
    pmsg = malloc(msg_size);
    if(pmsg == NULL){
        perror("malloc() ");
        exit(1);
    }

    remote_addr_len = sizeof(remote_addr);
    while (1){
        recvfrom(socket_id,pmsg,msg_size,0,(struct sockaddr *)&remote_addr,&remote_addr_len);
        inet_ntop(AF_INET,(void *)&remote_addr.sin_addr,ipstr,iP_SIZE);
        printf("remote addr %s, remote port %d\n",ipstr,ntohs(remote_addr.sin_port));
        printf("Name = %s\n",pmsg->name);
        printf("math = %d\n",ntohl(pmsg->math));
        printf("chinese = %d\n",ntohl(pmsg->chinese));
    }
    close(socket_id);
    exit(0);
}
