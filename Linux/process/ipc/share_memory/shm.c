#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

#define MEMORY_SIZE  64

int main(){

    __pid_t pid;
    int shmid;
    char *ptr;

    shmid = shmget(IPC_PRIVATE,MEMORY_SIZE,0600);
    if(shmid < 0){
        perror("shmget() ");
        exit(1);
    }

    pid = fork();
    if(pid < 0){
        perror("fork() ");
        exit(1);
    }
    if(pid == 0){
        ptr = shmat(shmid,NULL,0);
        if(ptr == (void *)-1){
            perror("shmat() ");
            exit(1);
        }
        strcpy(ptr,"Hello");
        shmdt(ptr);
        exit(0);
    }
    else{
        wait(NULL);
        ptr = shmat(shmid,NULL,0);
        if(ptr == (void *)-1){
            perror("shmat() ");
            exit(1);
        }     
        puts(ptr);
        shmdt(ptr);
        shmctl(shmid,IPC_RMID,0);
    }
    exit(0);
}