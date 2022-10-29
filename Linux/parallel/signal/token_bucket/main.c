#include<stdlib.h>
#include<stdio.h>
#include<token_bucket.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>

#define     DEFAULT_TOKEN       200
#define     DEFAULT_MAX_TOKEN   500
#define     BUFSIZE             1024

struct token_bucket_t * tb;


int main(int argc,char * argv[]){
    if(argc < 2){
        fprintf(stdout,"Usage error!\n");
        exit(1);
    }
    int srcfd;
    int dstfd;
    tb = init(DEFAULT_TOKEN,DEFAULT_MAX_TOKEN);
    int token = 0;
    char buf[BUFSIZE];
    ssize_t len;
    int pos;
    int ret;

    srcfd = open(argv[1],O_RDONLY);
    if(srcfd < 0){
        perror("open() ");
        exit(1);
    }
    while(1){
        token += token_featch(tb,DEFAULT_TOKEN);
        while((len = read(srcfd,buf,token)) < 0){ 
            if(errno == EINTR){
                continue;
            }
            perror("read() ");
            exit(1);
        }       
        token -= len;
        if(len == 0){
            if(token > 0)
                token_return(tb,token);
            break;
        }

        pos = 0;
        while (len > 0){
            if((ret = write(dstfd,buf+pos,len)) < 0){
                if(errno == EINTR){
                    continue;
                }
                perror("write() ");
                exit(1);
            }
            len -= ret;
            pos += ret;
        }
    }   
    destroy(tb);
    exit(1);
}