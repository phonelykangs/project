#ifndef MYTBF_H__
#define MYTBF_H__

#define     TB_SZ       1024

typedef     void    mytbf_t;

mytbf_t *init(int CPS,int tb_max);

int fetch(mytbf_t *tb, int sz);

int rev(mytbf_t *tb,int sz);

void destroy(mytbf_t *tb);

#endif