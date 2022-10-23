#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){
    int pipefd[2];
    pid_t pid;
    char buf[BUFSIZ];
    ssize_t len;
    if(pipe(pipefd) < 0){
        perror("pipe(): ");
        exit(1);
    }
    pid = fork();
    if(pid < 0){
        perror("fork(): ");
        exit(1);
    }
    if(pid == 0){
        close(pipefd[1]);
        len = read(pipefd[0],buf,BUFSIZ);
        if(len < 0){
            perror("read(): ");
            exit(1);
        }
        write(1,buf,len);
        close(pipefd[0]);
        exit(0);
    }
    else{
        close(pipefd[0]);
        write(pipefd[1],"Hello!\n",7);
        close(pipefd[1]);
        wait(NULL);
    }
    exit(0);
}