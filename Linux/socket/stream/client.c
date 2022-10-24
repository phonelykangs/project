#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include"proto.h"
#include<arpa/inet.h>

#define  BUF_SIZE 1024

int main(int argc,char *argv[]){
    if(argc < 2){
        fprintf(stderr,"Usage error!\n");
        exit(1);
    }

    int sd;
    FILE * fp;
    struct sockaddr_in remote_addr;
    char buf[BUF_SIZE];

    sd = socket(AF_INET,SOCK_STREAM,0);
    if(sd < 0){
        perror("socket() ");
        exit(1);
    }

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(atoi(RCV_PORT));
    inet_pton(AF_INET,argv[1],&remote_addr.sin_addr);

    if(connect(sd,(struct sockaddr *)&remote_addr,sizeof(remote_addr)) < 0){
        perror("connect() ");
        exit(1);
    }

    fp = fdopen(sd,"r+");
    if(fgets(buf, BUF_SIZE, fp) == NULL){
        fprintf(stderr,"fgets() \n");
        exit(1);
    }
    printf("%s\n",buf);
    fclose(fp);
    exit(0);
}