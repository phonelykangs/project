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
#include<sys/wait.h>

#define  BUFSIZE        1024
#define  IP_SIZE        40
#define  PROCESS_NUM    8

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
    free(ct);
}

void server_loop(int sd){
    char client_ip[IP_SIZE];
    int newsd;
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len;
    remote_addr_len = sizeof(remote_addr);
    while(1){
        newsd =  accept(sd,(struct sockaddr *)&remote_addr, &remote_addr_len);
        if(newsd < 0 && (newsd != EINTR || newsd != EAGAIN)){
            perror("accept() ");
            exit(1);
        }
        inet_ntop(AF_INET,&remote_addr.sin_addr,client_ip,IP_SIZE);
        printf("[%d] Client  addr : %s,\tport : %d \n",getpid(), client_ip,ntohs(remote_addr.sin_port));
        server_job(newsd);
        close(newsd);
    }
}

int main(){
    int sd;
    struct sockaddr_in local_addr;
    socklen_t local_addr_len;
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

    for(int i = 0;i < PROCESS_NUM;++i){
        pid = fork();
        if(pid < 0){
            perror("fork() ");
            exit(1);
        }
        if(pid == 0){
            server_loop(sd);
            exit(0);
        }
    }

    for(int i = 0;i < PROCESS_NUM;++i){
        wait(NULL);
    }
    
    close(sd);
    exit(0);
}