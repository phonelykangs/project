/* 
 *  print time according to shell input
 * 
 *  2022-10-25
 * 
 */



#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<assert.h>
#include<assert.h>
#include<unistd.h>

#define    MAX_TIME_SIZE     1024
#define    MAX_FMT_SIZE      1024

char * my_strncat(char *dest, const char *src, size_t n){
    assert(dest != NULL && src != NULL);
    char *ret = dest;
    while(*dest != '\0'){
        ++dest;
    }
    for(size_t i = 0;i < n && *src != '\0';++i){
        *dest = *src;
        ++dest;
        ++src;
    }
    *dest = '\0';
    return ret;
}

int main(int argc, char *argv[]){

    if(argc < 2){
        fprintf(stderr,"Usage error!\n");
        exit(1);
    }

    time_t t;
    struct tm *tm;
    char time_str[MAX_TIME_SIZE];
    char FMT[MAX_FMT_SIZE];
    FMT[0] = '\0';
    char ch;
    FILE *fp = stdout;

    tm = (struct tm *)malloc(sizeof(struct tm));
    t = time(NULL);
    tm = localtime_r(&t,tm);

    while(ch != -1){
        ch = getopt(argc,argv,"-YmMdH:Sw");
        switch (ch)
        {
            case 1:
                if(fp == stdout){
                    fp = fopen(argv[optind-1],"w");
                    if(fp == NULL){
                        perror("fopen() ");
                        fp = stdout;
                   }
                }
                break;
            case 'Y':
                my_strncat(FMT,"%Y ",MAX_FMT_SIZE-strlen(FMT));
                break;
            case 'm':
                my_strncat(FMT,"%m ",MAX_FMT_SIZE-strlen(FMT));
                break;   
            case 'M':
                my_strncat(FMT,"%M ",MAX_FMT_SIZE-strlen(FMT));
                break;
            case 'd':
                my_strncat(FMT,"%d ",MAX_FMT_SIZE-strlen(FMT));
                break;
            case 'H':
                if(strcmp(optarg,"1") == 0){
                    my_strncat(FMT,"%I ",MAX_FMT_SIZE-strlen(FMT));
                }
                else if(strcmp(optarg,"2") == 0){
                    my_strncat(FMT,"%H ",MAX_FMT_SIZE-strlen(FMT));
                }
                else{
                    fprintf(stderr, "invalid argument!\n");
                }
                break;
            case 'S':
                my_strncat(FMT,"%S ",MAX_FMT_SIZE-strlen(FMT));
                break;
            case 'w':
                my_strncat(FMT,"%w ",MAX_FMT_SIZE-strlen(FMT));
                break;
            default:
                break;
            }
    }
    printf("%s\n",FMT);
    strftime(time_str,MAX_TIME_SIZE,FMT,tm);
    my_strncat(time_str,"\n",MAX_FMT_SIZE-strlen(FMT));
    fprintf(fp, "%s", time_str);
    free(tm);
    if(fp != stdout){
        fclose(fp);
    }
    exit(0);
}