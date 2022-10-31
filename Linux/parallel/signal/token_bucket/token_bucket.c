#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<token_bucket.h>
#include<signal.h>
#include<unistd.h>
#include<stdbool.h>
#include<sys/time.h>

#define     MIN(x,y)    ((x) < (y) ? (x) : (y))

struct token_bucket_st{
    int pos;
    int token;
    int cps;
    int token_max;
};

typedef void(*sighandler_t)(int);

static struct token_bucket_st * bucket_list[TOKEN_BUCKET_NUM];
static bool inited = false;
static struct itimerval itv, otv;
static struct sigaction act,oldact;

static int getFreePos(){
    int pos = 0;
    for(;pos < TOKEN_BUCKET_NUM;++pos){
        if(bucket_list[pos] == NULL)
            return pos;
    }
    return -1;
}

void signal_handle(int i,siginfo_t * siginfo, void *unused){
    if(siginfo->si_code != SI_KERNEL){
        return ;
    }
    struct token_bucket_st * me;
    int pos = 0;
    for(;pos < TOKEN_BUCKET_NUM;++pos){
        if(bucket_list[pos] != NULL){
            me = bucket_list[pos];
            me->token += me->cps;
            if(me->token > me->token_max)
                me->token = me->token_max;
        }
    }
}

static void unload_module(){
    setitimer(ITIMER_REAL,&otv,NULL);
    if(sigaction(SIGALRM,&oldact,NULL) < 0){
        perror("sigaction() ");
        exit(1);
    }
    for(int i = 0;i < TOKEN_BUCKET_NUM;++i){
        free(bucket_list[i]);
    }
}

static int load_module(){
    itv.it_value.tv_sec = 2;
    itv.it_value.tv_usec = 0;
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;

    act.sa_sigaction = signal_handle;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;

    if(sigaction(SIGALRM,&act,&oldact) < 0){
        perror("sigaction() ");
        exit(1);
    }
    
    if(setitimer(ITIMER_REAL,&itv,&otv) < 0){
        perror("setitimer() ");
        exit(1);
    }
    atexit(unload_module);
    return 0;
}

token_bucket_t *init(int cps,int token_max){
    struct token_bucket_st * me;
    if(!inited){
        load_module();
        inited = true;
    }
    int pos = getFreePos();
    if(pos < 0)
        return NULL;
    me = (struct token_bucket_st *)malloc(sizeof(struct token_bucket_st));
    if(me == NULL){
        return NULL;
    }
    me->cps = cps;
    me->token_max = token_max;
    me->token = 0;
    me->pos = pos;
    bucket_list[pos] = me;
    return me;
}

int token_featch(token_bucket_t * ptr,int count){
    struct token_bucket_st * me = ptr;

    if(count <= 0)
        return -EINVAL;
    while(me->token <= 0)
        pause();
    
    int n = MIN(count,me->token);
    me->token -= n;
    return n;
}

int token_return(token_bucket_t * ptr,int count){
    struct token_bucket_st * me = ptr;

    if(count <= 0){
        return -EINVAL;
    }
    me->token += count;
    if(me->token > me->token_max){
        me->token = me->token_max;
    }
    return count;
}

int destroy(token_bucket_t * ptr){
    struct token_bucket_st * me = ptr;
    bucket_list[me->pos] = NULL;
    free(me);
    return 0;
}

