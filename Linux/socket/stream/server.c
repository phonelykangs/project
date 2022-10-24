#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include"proto.h"
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include<time.h>
#include<unistd.h>

#define  BUFSIZE   1024
#define  IP_SIZE   40

static void server_job(int sd){
    char str[BUFSIZ];
    int len;
    time_t t = time(NULL);
    struct tm *ct = (struct tm *)malloc(sizeof(struct tm));
    ct = localtime_r(&t, ct);
    len = sprintf(str,TIME_FMT,t,ct->tm_year+1900,ct->tm_mon+1);
    if(send(sd,str,len,0) < 0){
        perror("send() ");
        exit(1);
    }
}

int main(){
    int sd;
    struct sockaddr_in local_addr, remote_addr;
    socklen_t local_addr_len, remote_addr_len;
    int newsd;
    char client_ip[IP_SIZE];
    pid_t pid;

    sd = socket(AF_INET,SOCK_STREAM,0);
    if(sd < 0){
        perror("socket() ");
        exit(1);
    }

    int val = 1;
    if(setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)) < 0){
        perror("setsockopt() ");
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

    remote_addr_len = sizeof(remote_addr);
    while(1){
        newsd =  accept(sd,(struct sockaddr *)&remote_addr, &remote_addr_len);
        if(newsd < 0 && (newsd != EINTR || newsd != EAGAIN)){
            perror("accept() ");
            exit(1);
        }
        pid = fork();
        if(pid < 0){
            perror("fork() ");
            exit(1);
        }
        if(pid == 0){
            close(sd);
            inet_ntop(AF_INET,&remote_addr.sin_addr,client_ip,IP_SIZE);
            printf("Client  addr : %s,\tport : %d \n",client_ip,ntohs(remote_addr.sin_port));
            server_job(newsd);
            close(newsd);
            exit(0);
        }
        close(newsd);
    }
    close(sd);
    exit(0);
}