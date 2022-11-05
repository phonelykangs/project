#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>

static off_t flen(const char *path){
    int err;
    struct stat st;
    err = stat(path,&st);
    if(err < 0){
        perror("stat() ");
        exit(1);
    }
    return st.st_size;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr,"Usage error!\n");
        exit(1);   
    }
    fprintf(stdout,"%ld\n",flen(argv[1]));
    exit(1);
}