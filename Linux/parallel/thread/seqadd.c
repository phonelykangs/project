#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<errno.h>

#define     THREAD_NUM      20
#define     TARGET_FILE     "/tmp/out"
#define     BUFFER_SIZE     1024

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void * addfunc(void){
    FILE * fp;
    char LINEBUF[BUFFER_SIZE];

    fp = fopen(TARGET_FILE,"r+");
    if(fp == NULL){
        perror("fopen() : ");
        exit(1);
    }

    pthread_mutex_lock(&mutex);
    if(fgets(LINEBUF,BUFFER_SIZE,fp) < 0){
        perror("getline() ");
        exit(1);
    }

    fseek(fp,0,SEEK_SET);
    fprintf(fp,"%d\n",atoi(LINEBUF)+1);
    pthread_mutex_unlock(&mutex);

    fclose(fp);
    pthread_exit(NULL);
}

int main(){
    pthread_t tid[THREAD_NUM];
    int err;
    
    for(int i = 0;i < THREAD_NUM;++i){
        err = pthread_create(tid+i,NULL,addfunc,NULL);
        if(err){
            fprintf(stderr,"pthread_create() : %s\n",strerror(errno));
            exit(1);
        }
    }

    for(int i = 0;i < THREAD_NUM;++i){
        pthread_join(tid[i],NULL);
    }

    pthread_mutex_destroy(&mutex);
    exit(0);
}