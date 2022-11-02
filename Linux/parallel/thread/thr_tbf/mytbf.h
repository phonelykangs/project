#define     TOKEN_BUCKET_NUM       1024
#define     DEFAULT_CPS            20
#define     DEFAULT_TOKEN_MAX      200

typedef     void    mytbf_t;

mytbf_t *tbf_init(int cps,int token_max);

int tbf_featch(mytbf_t *ptr,int count);

int tbf_return(mytbf_t *ptr,int count);

void tbf_destroy(mytbf_t *ptr);
