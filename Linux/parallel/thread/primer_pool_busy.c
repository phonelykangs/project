#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<errno.h>

#define     THREAD_NUM      8

#define     START           30000000
#define     END             30000200

struct args_st{
    int num;
};

static int task = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void * func(void * p){

    int cur = 0;
    int mask;
    int idx = ((struct args_st *)p)->num;

    while(1){
        pthread_mutex_lock(&mutex);
        if(task != 0){
            if(task == -1){
                task = 0;
                pthread_mutex_unlock(&mutex);
                break;
            }
            else{
                cur = task;
                task = 0;
            }
        }
        pthread_mutex_unlock(&mutex);
        if(cur == 0){
            sched_yield();
            continue;
        }

        mask = 1;
        for(int i = 2;i <= cur / 2;++i){
            if(cur % i == 0){
                mask = 0;
                break;
            }
        }
        if(mask){
            fprintf(stdout,"[%d]:%d is a primer!\n",idx,cur);
        }
        cur = 0;
    }
    pthread_exit(p);
}

int main(){

    int err;
    struct args_st * args;
    void *ptr;
    pthread_t tid[THREAD_NUM];

    for(int i = 0;i < THREAD_NUM;++i){

        args = malloc(sizeof(struct args_st));
        if(args == NULL){
            perror("malloc() ");
            exit(1);
        }
        args->num = i;
        err = pthread_create(tid+i,NULL,func,args);
        if(err){
            fprintf(stderr,"pthread_create() : %s\n",strerror(errno));
            exit(1);
        }
    }
    for(int i = START;i <= END;++i){
        while(1){
            pthread_mutex_lock(&mutex);
            if(task != 0){
                pthread_mutex_unlock(&mutex);
                sched_yield();
                continue;
            }
            task = i;
            pthread_mutex_unlock(&mutex);
            break;
        }
    }

    for(int i = 0;i< THREAD_NUM;++i){
        while(1){
            pthread_mutex_lock(&mutex);
            if(task != 0){
                pthread_mutex_unlock(&mutex);
                sched_yield();
                continue;
            }
            task = -1;
            pthread_mutex_unlock(&mutex);
            break;
        }
    }

    pthread_mutex_lock(&mutex);
    while(task != 0){
        pthread_mutex_unlock(&mutex);
        sched_yield();
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_unlock(&mutex);

    for(int i = 0;i < THREAD_NUM;++i){
        pthread_join(tid[i],&ptr);
        free((struct args_st*)ptr);
    }

    pthread_mutex_destroy(&mutex);
    exit(0);
}