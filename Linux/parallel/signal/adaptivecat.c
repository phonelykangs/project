#include<stdlib.h>
#include<stdio.h>
#include<syslog.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<signal.h>
#include<sys/time.h>

#define     SPC         20
#define     MAX_TOKEN   10

static ssize_t writen(int fd,const void * buf, size_t count){
    ssize_t ret;
    ssize_t pos = 0;
    while(count > 0){
        ret = write(fd,buf+pos,count);
        if(ret < 0){
            if(errno == EINTR){
                continue;
            }
            syslog(LOG_ERR,"write() : %s",strerror(errno));
            return -1;
        }
        count -= ret;
        pos += ret;
    }
    return pos;
}

static volatile sig_atomic_t  token = 0;
static void signal_handle(int i){
    ++token;
    if(token > MAX_TOKEN){
        token = MAX_TOKEN;
    }
}

int main(int argc, char *argv[]){
    openlog("MyCat", LOG_PID | LOG_PERROR, LOG_USER);
    if(argc < 2){
        syslog(LOG_ERR,"Usage Error!");
        exit(1);
    }
    int src_fd;
    int dst_fd;
    char buf[SPC];
    ssize_t len;
    struct itimerval tv;

    do{
        src_fd = open(argv[1],O_RDONLY);
        if(src_fd < 0){
            if(errno == EINTR){
                continue;
            }
            syslog(LOG_ERR,"open() : %s",strerror(errno));
            exit(1);
        }
    }while(src_fd < 0);

    dst_fd = 1;
    tv.it_interval.tv_sec = 1;
    tv.it_interval.tv_usec = 0;
    tv.it_value.tv_sec = 5;
    tv.it_value.tv_usec = 0;

    signal(SIGALRM,signal_handle);
    setitimer(ITIMER_REAL,&tv,NULL);
    while(1){
        while(token <= 0){
            pause();
        }
        --token;

        while((len = read(src_fd,buf,SPC)) < 0){
            if(errno != EINTR && errno != EAGAIN){
                syslog(LOG_ERR,"read() : %s",strerror(errno));
                exit(1);
            }
            continue;
        }
        
        if(len == 0){
            break;
        }

        if(writen(dst_fd,buf,SPC) < 0){
            exit(1);
        }
    }
    closelog();
    exit(0);
}