#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/unistd.h>

static int err;
static struct stat statres;
static uint64_t mydu(const char *path){
    err = stat(path,&statres);
    if(err < 0){
        perror("stat():");
        exit(1);
    }
    if(S_ISDIR(statres.st_size)){
        
    }
    else{
        return statres.st_blocks;
    }
}

int main(int argc,char ** argv){
    if(argc < 2){
        perror("Usage error!\n");
        exit(1);
    }

    fprintf(stdout,"%lu\n",mydu(argv[1]));

    exit(0);
}