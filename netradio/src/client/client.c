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
#include<errno.h>
#include"client.h"
#include<proto.h>

static size_t my_strlen(const char *src){
    assert(src != NULL);
    size_t count = 0;
    while(*src != '\0'){
        ++src;
        ++count;
    }
    return count;
}

static char *my_strncpy(char *dest, size_t max_dsz, const char *src, size_t n){
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

static void print_config(struct client_config_st *config){
    printf("mgroup addr: %s \n", config->mgroup);
    printf("Port: %s \n",config->rcv_port);
    printf("player cmd: %s\n",config->player_cmd);
}

static void print_help(void){
    printf("-M --mgroup   multi-group\n");
    printf("-P --Port     multi-group Port\n");
    printf("-p --player   music player\n");
    printf("-H --help     show help\n");
}

static ssize_t writen(int fd, const void *buf,size_t count){
    ssize_t ret = 0;
    ssize_t pos = 0;
    while(count > 0){
        ret = write(fd,buf+pos,count);
        if(ret < 0){
            if(errno == EINTR){
                continue;
            }
            perror("write() ");
            return -1;
        }
        count -= ret;
        pos += ret;
    }
    return pos;
}

struct client_config_st client_config;

int main(int argc,char *argv[]){
    
    /**
     *  init client config 
    */

    my_strncpy(client_config.mgroup, MAX_MGROUP_SIZE, DEFAULT_MGROUP, sizeof(DEFAULT_MGROUP));
    my_strncpy(client_config.rcv_port, MAX_RCV_PORT_SIZE, DEFAULT_RCV_PORT, sizeof(DEFAULT_RCV_PORT));
    my_strncpy(client_config.player_cmd, MAX_PLAYER_CMD_SIZE, DEFAULT_PLAYER_CMD, sizeof(DEFAULT_PLAYER_CMD));

    /**
     *  variables define
    */

    int idx = 0;
    char ch;
    int sd;
    struct ip_mreqn soc_opt_req;
    struct sockaddr_in local_addr, server_addr, remote_addr;
    socklen_t server_addr_len, remote_addr_len;
    int val;
    int pipefd[2];
    pid_t pid;
    struct msg_list_st *msg_list;   
    ssize_t len; 
    struct msg_listdesc_st *msg_listdesc;
    char server_IP[SERVER_IP_SIZE_MAX];
    int choosen_id;
    int scanf_ret;
    struct msg_channle_st *msg_channle;
    
    struct option oparray[] = {
        {"mgroup",1,NULL,'M'},
        {"port",1,NULL,'P'},
        {"player",1,NULL,'p'},
        {"help",0,NULL,'H'},
        {NULL,0,NULL,0}
    };

    /**
     *  process shell cmd input
    */

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

    /**
     *  init socket
    */

    sd = socket(AF_INET,SOCK_DGRAM,0);      
    if(sd < 0){
        perror("socket() ");
        exit(1);
    }

    /**
     *  set socket attributes
    */
    
    inet_pton(AF_INET,client_config.mgroup,&soc_opt_req.imr_multiaddr);
    inet_pton(AF_INET,"0.0.0.0",&soc_opt_req.imr_address);
    soc_opt_req.imr_ifindex = if_nametoindex(NETWORK_INTERFACE);
    if(setsockopt(sd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&soc_opt_req,sizeof(soc_opt_req)) < 0){
        perror("setsocketopt(1) ");
        exit(1);
    }

    val = 1;
    if(setsockopt(sd,IPPROTO_IP,IP_MULTICAST_LOOP,&val,sizeof(val)) < 0){
        perror("setsocketopt(2) ");
        exit(1);
    }

    /**
     *  bind local ip and port
    */

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi(client_config.rcv_port));
    inet_pton(AF_INET,"0.0.0.0",&local_addr.sin_addr);
    if(bind(sd,(struct sockaddr *)&local_addr,sizeof(local_addr)) < 0){ 
        perror("bind() ");
        exit(1);
    }

    /**
     *  pipe for process communication between son and father process
    */

    if(pipe(pipefd) < 0){                            
        perror("pipe() ");
        exit(1);
    }

    pid = fork();
    if(pid < 0){
        perror("fork() ");  
        exit(1);
    }
    if(pid == 0){   
        close(sd);                              
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

        /**
         *  Recv channel list
        */

        msg_list = (struct msg_list_st *)malloc(MSG_LIST_MAX);
        if(msg_list == NULL){
            perror("malloc() ");
            exit(1);
        }
        while(1){
            server_addr_len = sizeof(server_addr);
            len = recvfrom(sd, msg_list, MSG_LIST_MAX, 0, (struct sockaddr *)&server_addr, &server_addr_len);
            if(len < 0 && errno != EAGAIN && errno != EINTR){
                perror("recvfrom() ");
                free(msg_list);
                exit(1);
            }
            if(len < sizeof(struct msg_list_st)){
                continue;
            }
            if(msg_list->chnid != CHANNLE_LIST_ID){
                continue;
            }
            inet_ntop(AF_INET,&server_addr.sin_addr,server_IP,SERVER_IP_SIZE_MAX);
            printf("server addr: %s, port: %d\n",server_IP,ntohs(server_addr.sin_port));
            break;
        }

        /**
         *  print channel list and choose channel
        */

        msg_listdesc = msg_list->list_description;
        for(;(char*)msg_listdesc < (((char*)msg_list) + len);msg_listdesc = (struct msg_listdesc_st *)(ntohs(msg_listdesc->length) + (char*)msg_listdesc)){
            printf("channel %d : %s\n",msg_listdesc->chnid,msg_listdesc->description);
        }
        free(msg_list);
        while(1){
            scanf_ret = scanf("%d",&choosen_id);
            if(scanf_ret != 1){
                exit(1);
            }
        }

        /**
         *  Rcv channel udp packet and write into pipe
        */

       remote_addr_len = sizeof(remote_addr);
       msg_channle = (struct msg_channle_st *)malloc(sizeof(MSG_CHANNLE_MAX));
       if(msg_channle == NULL){
        perror("malloc() ");
        exit(1);
       }
       while(1){
            len = recvfrom(sd,msg_channle,MSG_CHANNLE_MAX,0,(struct sockaddr *)&remote_addr,&remote_addr_len);
            if(len < 0 && errno != EAGAIN && errno != EINTR){
                    perror("recvfrom() ");
                    exit(1);
            }  
            if(len < sizeof(struct msg_channle_st)){
                fprintf(stderr,"udp packet Ignore!\n");
                continue;
            }
            if(remote_addr.sin_addr.s_addr != server_addr.sin_addr.s_addr || remote_addr.sin_port != server_addr.sin_port){
                fprintf(stderr,"Addr not match!\n");
                continue;
            }       
            if(msg_channle->chnid == choosen_id){
                if(writen(pipefd[1],msg_channle->data,len-sizeof(chnid_t)) < 0){
                    exit(1);
                }
            }   
       }
       free(msg_channle);
    }
    close(sd);
    exit(0); 
}