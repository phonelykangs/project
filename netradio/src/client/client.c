
/**
 *   client  
 * 
 *   -M --mgroup   multi-group
 *   -P --Port     multi-group Port
 *   -p --player   music player
 *   -H --help     show help   
 * 
*/

#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include<unistd.h>
#include<getopt.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<net/if.h>
#include"client.h"
#include<proto.h>

size_t my_strlen(const char *src){
    assert(src != NULL);
    size_t count = 0;
    while(*src != '\0'){
        ++src;
        ++count;
    }
    return count;
}

char *my_strncpy(char *dest, size_t max_dsz, const char *src, size_t n){
    assert(dest != NULL && src != NULL);
    size_t n_copy = max_dsz-1 > n ? n : max_dsz-1;
    size_t i = 0;
    for(;i < n_copy && *src !='\0';++i){
        dest[i] = *src;
        src++;
    }
    for(;i < max_dsz;++i){
        dest[i] = '\0';
    }
    return dest;
}

void print_config(struct client_config_st *config){
    printf("mgroup addr: %s \n", config->mgroup);
    printf("Port: %s \n",config->rcv_port);
    printf("player cmd: %s\n",config->player_cmd);
}

void print_help(void){
    printf("-M --mgroup   multi-group\n");
    printf("-P --Port     multi-group Port\n");
    printf("-p --player   music player\n");
    printf("-H --help     show help\n");
}

struct client_config_st client_config;

int main(int argc,char *argv[]){

    my_strncpy(client_config.mgroup, MAX_MGROUP_SIZE, DEFAULT_MGROUP, sizeof(DEFAULT_MGROUP));
    my_strncpy(client_config.rcv_port, MAX_RCV_PORT_SIZE, DEFAULT_RCV_PORT, sizeof(DEFAULT_RCV_PORT));
    my_strncpy(client_config.player_cmd, MAX_PLAYER_CMD_SIZE, DEFAULT_PLAYER_CMD, sizeof(DEFAULT_PLAYER_CMD));

    int idx = 0;
    char ch;
    int sd;
    struct ip_mreqn soc_opt_req;
    struct sockaddr_in local_addr;
    int val;
    int pipefd[2];
    pid_t pid;
    
    struct option oparray[] = {
        {"mgroup",1,NULL,'M'},
        {"port",1,NULL,'P'},
        {"player",1,NULL,'p'},
        {"help",0,NULL,'H'},
        {NULL,0,NULL,0}
    };

    while (1){
        ch = getopt_long(argc,argv,"M:P:p:H",oparray,&idx);
        if(ch < 0){
            break;
        }
        switch (ch)
        {
        case 'M':
            my_strncpy(client_config.mgroup,MAX_MGROUP_SIZE,optarg,my_strlen(optarg));
            break;
        case 'P':
            my_strncpy(client_config.rcv_port,MAX_RCV_PORT_SIZE,optarg,my_strlen(optarg));
            break;
        case 'p':
            my_strncpy(client_config.player_cmd,MAX_PLAYER_CMD_SIZE,optarg,my_strlen(optarg));
            break;
        case 'H':
            print_help();
            exit(0);
            break;
        default:
            abort();
            break;
        }
    }

    print_config(&client_config);

    sd = socket(AF_INET,SOCK_DGRAM,0);      //socket
    if(sd < 0){
        perror("socket() ");
        exit(1);
    }
    
    inet_pton(AF_INET,client_config.mgroup,&soc_opt_req.imr_multiaddr);
    inet_pton(AF_INET,"0.0.0.0",&soc_opt_req.imr_address);
    soc_opt_req.imr_ifindex = if_nametoindex(NETWORK_INTERFACE);
    if(setsockopt(sd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&soc_opt_req,sizeof(soc_opt_req)) < 0){  //set multi-group option
        perror("setsocketopt(1) ");
        exit(1);
    }

    val = 1;
    if(setsockopt(sd,IPPROTO_IP,IP_MULTICAST_LOOP,&val,sizeof(val)) < 0){
        perror("setsocketopt(2) ");
        exit(1);
    }

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi(client_config.rcv_port));
    inet_pton(AF_INET,"0.0.0.0",&local_addr.sin_addr);
    if(bind(sd,(struct sockaddr *)&local_addr,sizeof(local_addr)) < 0){     //bind local ip and port
        perror("bind() ");
        exit(1);
    }

    if(pipe(pipefd) < 0){                            //pipe for process communication
        perror("pipe() ");
        exit(1);
    }

    pid = fork();
    if(pid < 0){
        perror("fork() ");  
        exit(1);
    }
    if(pid == 0){   
        close(sd);                                //child process 
        close(pipefd[1]); 
        dup2(pipefd[0],0);
        if(pipefd[0] > 0){
            close(pipefd[0]);
        }
        execl("/bin/bash","bash","-c",client_config.player_cmd,NULL);
        perror("execl() ");
        exit(1);
    }
    else{
        close(pipefd[0]);
    }


    exit(0); 
}