#include<stdlib.h>
#include<stdio.h> 
#include"mytbf.h"
#include<pthread.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

#define     MIN(x,y)        ((x) > (y) ? (y) : (x))

struct mytbf_st{
    int cps;
    int token;
    int pos;
    int token_max;
    pthread_mutex_t st_mutex;
    pthread_cond_t st_cond;
};

static struct mytbf_st *tb_list[TOKEN_BUCKET_NUM];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t tid;
static pthread_once_t once = PTHREAD_ONCE_INIT;

static int getFreePos_unlocked(){
    int pos;
    for(pos = 0;pos < TOKEN_BUCKET_NUM;++pos){
        if(tb_list[pos] == NULL)
            return pos;
    }
    return -1;
}

static void unload_module(void){
    int pos;
    pthread_cancel(tid);
    pthread_join(tid,NULL);
    for(pos = 0;pos < TOKEN_BUCKET_NUM;++pos){
        if(tb_list[pos] != NULL){
            tbf_destroy(tb_list[pos]);
        }
    }
    pthread_mutex_destroy(&mutex);
    exit(0);
}

static void *load_module(void *p){
    int pos;
    while(1){
        pthread_mutex_lock(&mutex);
        for(pos = 0;pos < TOKEN_BUCKET_NUM;++pos){
            if(tb_list[pos] != NULL){
                pthread_mutex_lock(&(tb_list[pos]->st_mutex));
                tb_list[pos]->token += tb_list[pos]->cps;
                if(tb_list[pos]->token > tb_list[pos]->token_max){
                    tb_list[pos]->token = tb_list[pos]->token_max;
                }
                pthread_mutex_unlock(&(tb_list[pos]->st_mutex));
                pthread_cond_broadcast(&tb_list[pos]->st_cond);
            }
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}

static void init(void){
    int err;
    err = pthread_create(&tid,NULL,load_module,NULL);
    if(err){
        fprintf(stderr,"pthread_create() : %s\n",strerror(errno));
        exit(1);
    }
    atexit(unload_module);
}

mytbf_t *tbf_init(int cps,int token_max){
    struct mytbf_st *me;
    int pos;

    pthread_once(&once,init);

    me = malloc(sizeof(struct mytbf_st));
    if(me == NULL){
        perror("malloc() ");
        return NULL;
    }
    me->cps = cps;
    me->token = 0;
    me->token_max = token_max;
    pthread_mutex_init(&me->st_mutex,NULL);
    pthread_cond_init(&me->st_cond,NULL);

    pthread_mutex_lock(&mutex);
    pos = getFreePos_unlocked();
    if(pos == -1){
        pthread_mutex_unlock(&mutex);
        free(me);
        return NULL;
    }
    me->pos = pos;
    tb_list[pos] = me;
    pthread_mutex_unlock(&mutex);
    return me;
}

int tbf_featch(mytbf_t *ptr,int count){
    if(count <= 0)
        return -EINVAL;
    struct mytbf_st *me = ptr;
    int n;

    pthread_mutex_lock(&me->st_mutex);
    while(me->token <= 0){
        pthread_cond_wait(&me->st_cond,&me->st_mutex);
    }
    n = MIN(count,me->token);
    me->token -= n;
    pthread_mutex_unlock(&me->st_mutex);

    return n;
}

int tbf_return(mytbf_t *ptr,int count){
    if(count <= 0)
        return -EINVAL;
    struct mytbf_st *me = ptr;

    pthread_mutex_lock(&me->st_mutex);
    me->token += count;
    if(me->token > me->token_max)
        me->token = me->token_max;
    pthread_mutex_unlock(&me->st_mutex);
    pthread_cond_broadcast(&me->st_cond);

    return count;
}

void tbf_destroy(mytbf_t *ptr){
    struct mytbf_st *me = ptr;
    pthread_mutex_lock(&mutex);
    tb_list[me->pos] = NULL;
    pthread_mutex_unlock(&mutex);
    pthread_cond_destroy(&me->st_cond);
    pthread_mutex_destroy(&me->st_mutex);
    free(me);
}
