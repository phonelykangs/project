#pragma once

#include<iostream>

#define     DEFAULT_BUFSIZE           512

class myString{
    friend std::ostream& operator<<(std::ostream& out, const myString& m_str);
public:
    myString();                      
    myString(const char *str);
    myString(const myString& m_str);

    ~myString();

    size_t getIndexOf(const char& ch,size_t pos);
    size_t replaceCh(const char& dstch,const char& srcch);
    myString sub_str(size_t start, size_t end);

    myString& operator=(const myString& src);
    myString operator+(const myString& src);

    size_t size() const{return m_size;}
    bool empty(){return m_size == 0;}
    size_t capcity() const{return m_capcity;}
private:
    char *data;
    size_t m_capcity;
    size_t m_size;
};