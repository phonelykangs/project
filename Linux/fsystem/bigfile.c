#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#define     FILE_NAME       "bigfile.t"

int main(){
    int fd;
    fd = open(FILE_NAME,O_CREAT | O_TRUNC | O_RDWR, 0700);
    if(fd < 0){
        perror("open() ");
        exit(1);
    }
    lseek(fd,5LL * 1024LL * 1024LL * 1024LL,SEEK_SET);
    write(fd,"1",1);
    close(fd);
    exit(0);
}