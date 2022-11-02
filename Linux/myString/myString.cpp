#include<iostream>
#include<myString.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<utils.h>
#include<assert.h>

#define     MAX(x,y)        ((x) < (y) ? (y) : (x))

std::ostream& operator<<(std::ostream& out, const myString& m_str){
    for(size_t i = 0;i < m_str.m_size;++i){
        out << (m_str.data)[i];
    }
    return out;
}

myString::myString():
    m_capcity(DEFAULT_BUFSIZE),
    m_size(0)
{
    data = (char *)malloc(DEFAULT_BUFSIZE);
    if(data == NULL){
        perror("malloc() ");
        exit(1);
    }
    data[0] = '\0';
}

myString::myString(const char *str){
    m_size = utils::myStrlen(str);
    m_capcity = MAX(2 * m_size, DEFAULT_BUFSIZE);

    data = (char *)malloc(m_capcity);
    if(data == NULL){
        perror("malloc() ");
        exit(1);
    }
    utils::myStrcpy(data,str,m_capcity);
}

myString::myString(const myString& m_str):
    m_capcity(m_str.m_capcity),
    m_size(m_str.m_size)
{
    data = (char *)malloc(m_capcity);
    if(data == NULL){
        perror("malloc() ");
        exit(1);
    }
    utils::myStrcpy(data,m_str.data,m_capcity);
}

size_t myString::getIndexOf(const char& ch,size_t pos){
    assert(pos < m_size);
    while(pos < m_size){
        if(data[pos] == ch)
            return pos;
        ++pos;
    }
    return -1;
}

myString::~myString(){
    free(data);
    m_capcity = 0;
    m_size = 0;
}

size_t myString::replaceCh(const char& dstch,const char& srcch){
    assert(dstch != '\0' && srcch != '\0');
    size_t count = 0;
    char *pos = data;
    while(*pos != '\0'){
        if(*pos == dstch){
            *pos = srcch;
            ++count;
        }
        ++pos;
    }
    return count;
}

myString& myString::operator=(const myString& src){
    if(src.m_capcity > this->m_capcity){
        this->data = (char *)realloc(this->data,src.m_capcity);
        if(this->data == NULL){
            perror("realloc() ");
            exit(1);
        }
    }
    m_size = src.m_size;
    m_capcity = src.m_capcity;
    utils::myStrcpy(data,src.data,m_capcity);
    return *this;
}

myString myString::operator+(const myString& src){
    myString temp(*this);
    temp.m_capcity += src.m_capcity;
    temp.m_size += src.m_size;
    temp.data = (char *)realloc(temp.data,temp.m_capcity);
    if(temp.data == NULL){
        perror("realloc()");
        exit(1);
    }
    utils::myStrcat(temp.data,src.data);
    return temp;
}

myString myString::sub_str(size_t start, size_t end){
    assert(start <= end && start >= 0 && end < this->m_size);
    size_t len = end - start + 1;
    char tmp[len+1];
    utils::myStrcpy(tmp,data+start,len);
    return myString(tmp);
}



