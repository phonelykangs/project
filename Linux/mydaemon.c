#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<signal.h>
#include<syslog.h>

#define FILE_NAME "/tmp/out"

FILE *fp;

static int daemonize(){
    pid_t pid;
    int fd;
    pid = fork();
    if(pid < 0){
        return -1;
    }
    if(pid > 0){
        exit(0);
    }
    else{
        fd = open("/dev/null",O_RDWR);
        if(fd < 0){
            return -2;
        }
        if(setsid() < 0){
            return -3;
        }
        dup2(fd,0);
        dup2(fd,1);
        dup2(fd,2);
        if(fd > 2){
            close(fd);
        }

        chdir("/");
        umask(0);
        return 0;
    }
}

static void sighandle (int s){
    closelog();
    fclose(fp);
    exit(0);
}

int main(){
    
    int i = 0;
    openlog("mydaemon",LOG_PID,LOG_DAEMON);

    sigset_t sset;
    sigemptyset(&sset);
    sigaddset(&sset,SIGINT);
    sigaddset(&sset,SIGQUIT);
    sigaddset(&sset,SIGTERM);

    struct sigaction act;
    act.sa_handler = sighandle;
    act.sa_mask = sset;
    act.sa_flags = 0;
    act.sa_restorer = NULL;

    sigaction(SIGINT,&act,NULL);
    sigaction(SIGTERM,&act,NULL);
    sigaction(SIGQUIT,&act,NULL);

    if(daemonize()){
        syslog(LOG_ERR,"daemonize failed!");
        exit(1);
    }
    syslog(LOG_NOTICE,"daemonize success!");
    fp = fopen(FILE_NAME,"w");
    if(fp == NULL){
        syslog(LOG_ERR,"open file failed!");
        exit(1);
    }
    for(;;++i){
        fprintf(fp,"%d\n",i);
        fflush(fp);
        sleep(1);
    }
}