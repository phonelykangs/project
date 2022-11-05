#include<sys/types.h>
#include<dirent.h>
#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<errno.h>

#define     DIR_NAME    "./"

int main(){
    DIR *dp;
    struct dirent *dirres;
    dp = opendir(DIR_NAME);
    if(dp == NULL){
        perror("opendir():");
        exit(1);
    }  

    while((dirres = readdir(dp)) != NULL){
        fprintf(stdout, "%s\n",dirres->d_name);
    }

    closedir(dp);
}