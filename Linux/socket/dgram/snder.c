#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include"proto.h"
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

#define  REMOTE_IP  "127.0.0.1"

int main(int argc,char *argv[]){
    if(argc < 2){
        fprintf(stderr,"Usage error ...\n");
        exit(1);
    }
    if(strlen(argv[1]) > NAME_MAX){
        fprintf(stderr,"Name is too long!\n");
        exit(1);
    }

    int socket_id;
    struct msg_st *pmsg;
    struct sockaddr_in remote_addr;
    size_t msg_size;

    socket_id = socket(AF_INET,SOCK_DGRAM,0);
    if(socket_id < 0){
        perror("socket() ");
        exit(0);
    }

    msg_size = sizeof(struct msg_st) + strlen(argv[1]);
    pmsg = malloc(msg_size);
    if(pmsg == NULL){
        perror("malloc() ");
        exit(1);
    }

    strcpy(pmsg->name,argv[1]);
    pmsg->math = htonl(rand() % 100);
    pmsg->chinese = htonl(rand() % 100);

    remote_addr.sin_family = AF_INET;
    inet_pton(AF_INET, REMOTE_IP ,&remote_addr.sin_addr);
    remote_addr.sin_port = htons(atoi(RCV_PORT));

    if(sendto(socket_id,pmsg,msg_size,0,(struct sockaddr *)&remote_addr,sizeof(remote_addr)) < 0){
        perror("sendto() ");
        exit(1);
    }
    
    puts("OK");
    close(socket_id);
    exit(0);
}