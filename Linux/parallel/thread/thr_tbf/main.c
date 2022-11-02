#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"mytbf.h"
#include<unistd.h>
#include<errno.h>

#define     BUFFER_SIZE     1024

int main(int argc,char *argv[]){
    if(argc < 2){
        fprintf(stderr,"Usage error!\n");
        exit(1);
    }
    
    int src_fd;
    int dst_fd;
    ssize_t len;
    int token = 0;
    struct mytbf_t *tb;
    char linebuf[BUFFER_SIZE];
    int pos;
    int ret;

    src_fd = open(argv[1],O_RDONLY);
    if(src_fd < 0){
        perror("open() ");
        exit(1);
    }

    tb = tbf_init(DEFAULT_CPS,DEFAULT_TOKEN_MAX);
    if(tb == NULL){
        exit(1);
    }
    while(1){
        token += tbf_featch(tb,DEFAULT_CPS);
        while((len = read(src_fd,linebuf,token)) < 0){
            if(errno == EINTR)
                continue;
            perror("read():");
            exit(1);
        }
        if(len == 0){
            if(token > 0)  
                return token;
            break;
        }
        token -= len;
        
        pos = 0;
        dst_fd = 0;
        while(len > 0){
            ret = write(dst_fd,linebuf+pos,len);
            if(ret < 0){
                if(ret == EINTR)
                    continue;
                perror("write() ");
                exit(1);
            }
            len -= ret;
            pos += ret;
        }
    }
    tbf_destroy(tb);
    exit(1);
}