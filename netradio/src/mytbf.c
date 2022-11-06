#include<stdlib.h>
#include<stdio.h>
#include"mytbf.h"
#include<errno.h>
#include<pthread.h>

#define     MIN(x,y)    ((x) < (y) ? (x) : (y))

struct mytbf_st{
    int cps;
    int token;
    int token_max;
    int pos;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
};

static struct mytbf_st *tb_list[TB_SZ];
static pthread_t tid;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_once_t once = PTHREAD_ONCE_INIT;

static int getPos_unlocked(){
    int pos = 0;
    for(;pos < TB_SZ;++pos){
        if(tb_list[pos] == NULL)
            return pos; 
    }
    return -1;
}

static void unload_module(){
    pthread_cancel(tid);
    pthread_join(tid,NULL);
    int pos = 0;
    for(;pos < TB_SZ;++pos){
        if(tb_list[pos] != NULL){
            pthread_mutex_lock(&tb_list[pos]->mutex);
            destroy(tb_list[pos]);
        }
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    exit(0);
}

static void *loop(void *p){
    int pos = 0;
    while(1){
        pthread_mutex_lock(&mutex);
        for(;pos < TB_SZ;++pos){
            if(tb_list[pos] != NULL){
                pthread_mutex_lock(&tb_list[pos]->mutex);
                tb_list[pos]->token += tb_list[pos]->cps;
                if(tb_list[pos]->token > tb_list[pos]->token_max)
                    tb_list[pos]->token = tb_list[pos]->token_max;
                pthread_mutex_unlock(&tb_list[pos]->mutex);
                pthread_cond_broadcast(&tb_list[pos]->cond);
            }
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}

static void load_module(){
    int err;
    err = pthread_create(&tid,NULL,loop,NULL);
    if(err < 0){
        perror("pthread_create() ");
        exit(1);
    }
    atexit(unload_module);
}

mytbf_t *init(int cps,int tb_max){
    int pos;
    struct mytbf_st *me;

    pthread_once(&once,load_module);

    me = (struct mytbf_t *)malloc(sizeof(struct mytbf_st));
    if(me == NULL){
        perror("malloc(): ");
        return NULL;
    }

    pthread_mutex_lock(&mutex);
    pos = getPos_unlocked();
    if(pos < 0){
        pthread_mutex_unlock(&mutex);
        free(me);
        return NULL;
    }
    me->pos = pos;
    tb_list[pos] = me;
    pthread_mutex_unlock(&mutex);

    me->cps = cps;
    me->token = 0;
    me->token_max = tb_max;
    pthread_mutex_init(&me->mutex,NULL);
    pthread_cond_init(&me->cond,NULL);

    return me;
}

int fetch(mytbf_t *tb, int sz){
    struct mytbf_st *me = tb;
    int ret = 0;
    if(sz <= 0)
        return -EINVAL;
    pthread_mutex_lock(&me->mutex);
    while(me->token <= 0){
        pthread_cond_wait(&me->cond,&me->mutex);
    }
    ret = MIN(sz,me->token);
    me->token -= ret;
    pthread_mutex_unlock(&me->mutex);
    return ret;
}

int rev(mytbf_t *tb,int sz){
    struct mytbf_st *me = tb;
    if(sz <= 0)
        return -EINVAL;
    pthread_mutex_lock(&me->mutex);
    me->token += sz;
    if(me->token > me->token_max)
        me->token = me->token_max;
    pthread_mutex_unlock(&me->mutex);
    pthread_cond_broadcast(&me->cond);
    return sz;
}

void destroy(mytbf_t *tb){
    struct mytbf_st *me = tb;
    pthread_mutex_lock(&mutex);
    tb_list[me->pos] = NULL;
    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&me->mutex);
    pthread_cond_destroy(&me->cond);
    free(me);
}
