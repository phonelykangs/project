#define     DEFAULT_IF_NAME         "eth0"
#define     DEFAULT_MEDIA_DIR       "~/media"

enum RUN_MODE{
    RUN_DAEMON = 1,
    RUN_FOREGROUND
};

struct server_config_st{
    char *mgroup;
    char *rcvport;
    char *media_dir;
    char run_mode;
    char *ifname;
};

extern struct server_config_st server_config;