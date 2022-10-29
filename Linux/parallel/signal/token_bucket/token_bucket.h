#define     TOKEN_BUCKET_NUM        1024

typedef void token_bucket_t;

token_bucket_t *init(int cps,int token_max);

int token_featch(token_bucket_t * ptr,int count);

int token_return(token_bucket_t * ptr,int count);

int destroy(token_bucket_t * ptr);
