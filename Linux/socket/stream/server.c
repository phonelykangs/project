#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include"proto.h"
#include<arpa/inet.h>

int main(){
    int sd;
    struct sockaddr_in local_addr;
    socklen_t local_addr_len;

    sd = socket(AF_INET,SOCK_STREAM,0);
    if(sd < 0){
        perror("socket() ");
        exit(1);
    }

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi(RCV_PORT));
    inet_pton(AF_INET,"0.0.0.0",&local_addr.sin_addr);
    local_addr_len = sizeof(local_addr);

    if(bind(sd,(struct sockaddr *)&local_addr,local_addr_len) < 0){
        perror("bind() ");
        exit(1);
    }

    if(listen(sd,200) < 0){
        perror("listen() ");
        exit(1);
    }

    while(1){
        accept()
    }
}