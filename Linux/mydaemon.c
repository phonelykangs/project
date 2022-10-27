#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define FILE_NAME "/tmp/out"

static int daemonize(){
    pid_t pid;
    int fd;
    pid = fork();
    if(pid < 0){
        perror("fork() ");
        return -1;
    }
    if(pid > 0){
        exit(0);
    }
    else{
        fd = open("/dev/null",O_RDWR);
        if(fd < 0){
            perror("open()");
            return -1;
        }
        if(setsid() < 0){
            perror("setsid() ");
            return -1;
        }
        dup2(fd,0);
        dup2(fd,1);
        dup2(fd,2);
        if(fd > 2){
            close(fd);
        }

        chdir("/");
        return 0;
    }
}

int main(){
    if(daemonize()){
        exit(1);
    }
    FILE *fp;
    int i = 0;
    
    fp = fopen(FILE_NAME,"w");
    if(fp == NULL){
        exit(1);
    }
    
    for(;;++i){
        fprintf(fp,"%d\n",i);
        fflush(fp);
        sleep(1);
    }

    fclose(fp);
    exit(1);
}