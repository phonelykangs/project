#pragma once

#define     MAX_RCV_PORT_SIZE       20
#define     MAX_MGROUP_SIZE         40
#define     MAX_PLAYER_CMD_SIZE     100
#define     DEFAULT_PLAYER_CMD      "/usr/bin/mpg123 - > /dev/null"
#define     NETWORK_INTERFACE       "eth0"
#define     SERVER_IP_SIZE_MAX      50    


struct client_config_st{
    char rcv_port[MAX_RCV_PORT_SIZE];
    char mgroup[MAX_MGROUP_SIZE];
    char player_cmd[MAX_PLAYER_CMD_SIZE];
};

extern struct client_config_st client_config;