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
#include<syslog.h>
#include<string.h>
#include<errno.h>
#include<signal.h>
#include<sys/socket.h>
#include<errno.h>
#include<string.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<net/if.h>
#include<server.h>

struct server_config_st server_config = {
    .rcvport = DEFAULT_RCV_PORT,
    .mgroup = DEFAULT_MGROUP,
    .media_dir = DEFAULT_MEDIA_DIR,
    .ifname = DEFAULT_IF_NAME,
    .run_mode = RUN_DAEMON
};

static struct sigaction act,quit_old_act,term_old_act,intr_old_act;

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
        syslog(LOG_ERR,"fork() : %s",strerror(errno));
        return -1;
    }
    if(pid > 0){
        exit(0);
    }
    else{
        fd = open("/dev/null",O_RDWR);
        if(fd < 0){
            syslog(LOG_ERR,"open() : %s",strerror(errno));
            return -2;
        }
        if(setsid() < 0){
            syslog(LOG_ERR,"setsid() : %s",strerror(errno));
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

static void undaemonize(int s){

    sigaction(SIGINT,&intr_old_act,NULL);
    sigaction(SIGQUIT,&quit_old_act,NULL);
    sigaction(SIGTERM,&term_old_act,NULL);

    closelog();
    exit(0);
}

static int socket_int(void){
    int sd;
    struct ip_mreqn iprq;
    struct sockaddr_in local_addr;
    socklen_t local_addr_len;

    sd = socket(AF_INET,SOCK_DGRAM,0);
    if(sd < 0){
        syslog(LOG_ERR,"socket():%s",strerror(errno));
        return -1;
    }

    inet_pton(AF_INET,server_config.mgroup,&iprq.imr_multiaddr.s_addr);
    inet_pton(AF_INET,"0.0.0.0",&iprq.imr_address.s_addr);
    iprq.imr_ifindex = if_nametoindex(server_config.ifname);
    if(setsockopt(sd,IPPROTO_IP,IP_MULTICAST_IF,&iprq,sizeof(iprq)) < 0){
        syslog("setsocketopt(IP_MULTICAST_IF): %s",strerror(errno));
        return -2;
    }

    local_addr.sin_family = AF_INET;
    inet_pton(AF_INET,"0.0.0.0",&local_addr.sin_addr);
    local_addr.sin_port = htons(atoi(DEFAULT_SERVER_PORT));
    local_addr_len = sizeof(local_addr);
    if(bind(sd,(struct sockaddr *)&local_addr,local_addr_len) < 0){
        syslog(LOG_ERR,"bind(): %s",strerror(errno));
        return -3;
    }
    return sd;
}
 
int main(int argc, char *argv[]){
    int ch;
    int sd;

    act.sa_sigaction = undaemonize;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGINT);
    sigaddset(&act.sa_mask,SIGTERM);
    sigaddset(&act.sa_mask,SIGQUIT);
    act.sa_flags = 0;
    act.sa_restorer = NULL;

    sigaction(SIGINT,&act,&intr_old_act);
    sigaction(SIGQUIT,&act,&quit_old_act);
    sigaction(SIGTERM,&act,&term_old_act);

    openlog("netradio",LOG_PID | LOG_PERROR,LOG_DAEMON);
    
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
            syslog(LOG_ERR,"Invalid Arguement!");
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
        /* forground do sth! */
    }
    else{
        syslog(LOG_ERR,"EINVAL run_mode error");
        exit(1);
    }

    sd = socket_int();
    if(sd < 0){
        undaemonize(SIGQUIT);
    }

    
}