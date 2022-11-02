#include"utils.h"
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>

size_t utils::myStrlen(const char *str){
    size_t pos = 0;
    while(str[pos] != '\0'){
        ++pos;
    }
    return pos;
}

char *utils::myStrcat(char *dst,const char *src){
    assert(dst != NULL && src != NULL);
    char *ret = dst;

    while(*dst != '\0'){
        ++dst;
    }
    while(*src != '\0'){
        *dst++ = *src++;
    }
    *dst = '\0';
    return ret;
}

char *utils::myStrcpy(char *dst,const char *src,size_t len){
    assert(dst != NULL && src != NULL);
    size_t i = 0;
    for(;i < len && src[i] != '\0';++i){
        dst[i] = src[i];
    }
    for(;i < len;++i){
        dst[i] = '\0';
    }
    return dst;
}
    



