/**
 *  server
 *  -M  multigroup
 *  -P  Port
 *  -F  foreground
 *  -D  media directory
 *  -I  network interface
 *  -H  help
 * 
*/

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<server_config.h>
#include<proto.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

static void print_help(){
    printf("-M  multigroup!\n");
    printf("-P  Port!\n");
    printf("-F  foreground!\n");
    printf("-D  media directory!\n");
    printf("-I  network interface!\n");
}

static void print_config(struct server_config_st *pConfig){
    fprintf(stdout,"Mgroup Addr : %s\n",pConfig->mgroup);
    fprintf(stdout,"RCV Port : %s\n",pConfig->rcvport);
    fprintf(stdout,"Media Directory : %s\n",pConfig->media_dir);
    fprintf(stdout,"Network Interface %s\n",pConfig->ifname);
    fprintf(stdout,"Run Mode(1:Demon 2:Foreground) : %d\n",pConfig->run_mode);
}

static int daemonize(){
    pid_t pid;
    int fd;

    pid = fork();
    if(pid < 0){
        perror("fork() ");
        return -1;
    }
    if(pid > 0){
        exit(0);
    }
    else{
        fd = open("/dev/null",O_RDWR);
        if(fd < 0){
            perror("open() ");
            return -2;
        }
        if(setsid() < 0){
            perror("setsid() ");
            return -3;
        }
        dup2(fd,0);
        dup2(fd,1);
        dup2(fd,2);
        if(fd > 2){
            close(fd);
        }
        chdir("/");
        umask(0);
        return 0;
    }
}

struct server_config_st server_config = {
    .rcvport = DEFAULT_RCV_PORT,
    .mgroup = DEFAULT_MGROUP,
    .media_dir = DEFAULT_MEDIA_DIR,
    .ifname = DEFAULT_IF_NAME,
    .run_mode = RUN_DAEMON
};
 
int main(int argc, char *argv[]){
    int ch;
    
    while(1){
        ch = getopt(argc, argv, "M:P:FD:I:H");
        if(ch == -1){
            break;
        }
        switch (ch)
        {
        case 'M':
            server_config.mgroup = optarg;
            break;
        case 'P':
            server_config.rcvport = optarg;
            break;
        case 'F':
            server_config.run_mode = RUN_FOREGROUND;
            break;
        case 'D':
            server_config.media_dir = optarg;
            break;
        case 'I':
            server_config.ifname = optarg;
            break;
        case 'H':
            print_help();
            exit(0);
        default:
            fprintf(stderr,"Invalid Arguement!\n");
            abort();
            break;
        }
    }
    print_config(&server_config);

    if(server_config.run_mode == RUN_DAEMON){
        if(daemonize()){
            exit(1);
        }
    }
    else if(server_config.run_mode == RUN_FOREGROUND){

    }
    else{
        fprintf(stderr,"Invalid Vlaue!\n");
        exit(1);
    }
}