#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/unistd.h>
#include<string.h>
#include<glob.h>
#include<stdbool.h>

#define     PATH_SIZE       1024

static bool noloop(char *path){
    size_t sz = strlen(path);
    int i = sz-1;
    for(;i >= 0;--i){
        if(path[i] == '/')
            break;
    }
    if(i == 0)
        return false;
    char *pos = &path[i];
    if(strcmp(".",pos+1) == 0 || strcmp("..",pos+1) == 0){
        return true;
    }
    return false;
}

static int err;
static struct stat statres;
static char nxpath[PATH_SIZE];

static uint64_t mydu(const char *path){
    uint64_t ret = 0;
    glob_t globres;
    err = lstat(path,&statres);
    if(err < 0){
        perror("stat():");
        exit(1);
    }
    if(S_ISDIR(statres.st_mode)){
        strncpy(nxpath,path,PATH_SIZE);
        strncat(nxpath,"/*",PATH_SIZE-strlen(path));
        glob(nxpath,0,NULL,&globres);

        strncpy(nxpath,path,PATH_SIZE);
        strncat(nxpath,"/.*",PATH_SIZE-strlen(path));
        glob(nxpath,GLOB_APPEND,NULL,&globres);

        ret += statres.st_blocks;
        for(int i = 0;i < globres.gl_pathc;++i){
            if(noloop(globres.gl_pathv[i])){
                continue;
            }
            ret += mydu(globres.gl_pathv[i]);
        }
        globfree(&globres);
    }
    else{
        return statres.st_blocks;
    }
    return ret;
}

int main(int argc,char **argv){
    if(argc < 2){
        perror("Usage error!\n");
        exit(1);
    }
    fprintf(stdout,"%lu\n",mydu(argv[1]) / 2);
    exit(0);
}