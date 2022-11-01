#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

#define     THREAD_NUM      4

static pthread_mutex_t mutex_arr[THREAD_NUM];

struct args_st{
    int num;
};

static void * print(void * p){
    int num = ((struct args_st *)p)->num;
    while(1){
        pthread_mutex_lock(mutex_arr+num);
        fprintf(stdout,"%c",'a'+num);
        pthread_mutex_unlock(mutex_arr+((num+1)%THREAD_NUM));
    }
    pthread_exit(p);
}

int main(){

    int err;
    struct args_st * args;
    void * ptr;
    pthread_t tid[THREAD_NUM];

    for(int i = 0;i < THREAD_NUM;++i){
        pthread_mutex_init(mutex_arr+i,NULL);
        pthread_mutex_lock(mutex_arr+i);
        
        args = (struct args_st *)malloc(sizeof(struct args_st));
        if(args == NULL){
            perror("malloc() ");
            exit(1);
        }
        args->num = i;
        err = pthread_create(tid+i,NULL,print,(void *)args);
        if(err){
            fprintf(stderr,"pthread_create() : %s\n",strerror(errno));
            exit(1);
        }
    }
    pthread_mutex_unlock(mutex_arr);
    alarm(1);

    for(int i = 0;i < THREAD_NUM;++i){
        pthread_join(tid[i],&ptr);
        free((struct args_st *)ptr);
        pthread_mutex_destroy(mutex_arr+i);
    }
    exit(0);
}