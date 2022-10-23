#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<errno.h>

#define FILE_NAME       "temp"
#define PROCESS_NUM     20
#define BUFSIZE         32

static int semid;

void P(void){
    struct sembuf semst;
    semst.sem_num = 0;
    semst.sem_op = -1;
    semst.sem_flg = 0;

    while(semop(semid,&semst,1) < 0){
        if(errno != EINTR || errno != EAGAIN){
            perror("semop() ");
            exit(1);
        }
    }
    return;
}

void V(void){
    struct sembuf semst;
    semst.sem_num = 0;
    semst.sem_op = +1;
    semst.sem_flg = 0;
    if(semop(semid,&semst,1) < 0){
        perror("semop() ");
        exit(1);
    }
    return ;
}

void add_func(void){
    FILE * fp;
    int num;
    char buf[BUFSIZ];
    fp = fopen(FILE_NAME,"r+");
    if(fp == NULL){
        perror("fopen() ");
        exit(1);
    }
    P();
    //printf("++++");
    fgets(buf,BUFSIZ,fp);
    num = atoi(buf);
    ++num;
    fseek(fp,0,SEEK_SET);
    fprintf(fp,"%d",num);
    fflush(fp);
    V();
    fclose(fp);
    return ;
}

int main(){
    int i;
    __pid_t pid;
    key_t key;
    key = ftok(FILE_NAME,'a');
    if(key < 0){
        perror("ftok() ");
        exit(1);
    }
    semid = semget(key,1,IPC_CREAT | 0600);
    if(semid < 0){
        perror("semget() ");
        exit(1);
    }

    if(semctl(semid,0,SETVAL,1) < 0){
        perror("semctl() ");
        exit(1);
    }

    for(i = 0;i < PROCESS_NUM;++i){
        pid = fork();
        if(pid < 0){
            perror("fork() ");
            exit(1);
        }
        if(pid == 0){
            add_func();
            exit(0);
        }
        else{

        }
    }

    for(i = 0;i < PROCESS_NUM;++i){
        wait(NULL);
    }
    semctl(semid,0,IPC_RMID);
    exit(0);
}