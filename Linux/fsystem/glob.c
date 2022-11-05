#include<stdlib.h>
#include<stdio.h>
#include<glob.h>

#define     PATTERN     "/etc/a*"

int main(int argc,char **argv){

    int err;
    glob_t globres;
    err = glob(PATTERN,0,NULL,&globres);
    if(err < 0){
        perror("glob() ");
        exit(1);
    }
    for(int i = 0;i < globres.gl_pathc;++i){
        fprintf(stderr,"%s\n",globres.gl_pathv[i]);
    }

    exit(0);
}