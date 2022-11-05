#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdint.h>

static uint8_t fkind(mode_t * st_mode){
    if(S_ISREG(*st_mode))
        return '-';
    else if(S_ISFIFO(*st_mode))
        return 'p';
    else if(S_ISBLK(*st_mode))
        return 'b';
    else if(S_ISCHR(*st_mode))
        return 'c';
    else if(S_ISDIR(*st_mode))
        return 'd';
    else if(S_ISLNK(*st_mode))
        return 'l';
    else if(S_ISSOCK(*st_mode))
        return 's';
    else  return '?';
}

static char * ftype(mode_t *st_mode){
    switch (S_IFMT & *st_mode)
    {
    case S_IFBLK:
        return "block device\n";
        break;
    case S_IFCHR:
        return "character device\n";
    case S_IFDIR:
        return "directory\n";
    case S_IFIFO:
        return "FIFO\n";
    case S_IFLNK:
        return "symlink\n";
    case S_IFREG:
        return "regular\n";
    case S_IFSOCK:
        return "socket\n";
    default:
        break;
    }
}

int main(int argc,char *argv[]){
    if(argc < 2){
        fprintf(stderr,"Usage error!\n");
        exit(1);
    }
    struct stat statres;
    if(stat(argv[1],&statres) < 0){
        perror("stat() ");
        exit(1);
    }
    fprintf(stdout,"%c\n",fkind(&statres.st_mode));
    fprintf(stdout,"%s",ftype(&statres.st_mode));
    exit(0);
}