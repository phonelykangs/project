/*
 * @lc app=leetcode.cn id=415 lang=c
 *
 * [415] 字符串相加
 */

// @lc code=start
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#define     MAX(x,y)     ((x) > (y) ? (x) : (y))

char * addStrings(char * num1, char * num2){
    uint32_t ns1 = strlen(num1);
    uint32_t ns2 = strlen(num2);
    uint32_t nsret = MAX(ns1,ns2) + 2;
    char * ret = malloc(nsret);
    memset(ret,'0',nsret);
    ret[nsret-1] = '\0';
    uint32_t idx = nsret-2;
    int i = ns1 - 1;
    int j = ns2 - 1;
    int add = 0;
    while(i >= 0 || j >= 0){
        int i1 = i < 0 ? 0 : num1[i--] - '0';
        int i2 = j < 0 ? 0 : num2[j--] - '0';
        int sum = i1 + i2 + add;
        add = sum / 10;
        sum %= 10;
        ret[idx--] = sum + '0';
    }
    if(add){
        ret[idx--] = add + '0';
    }
    while(*ret == '0')
        ++ret;
    if(*ret == '\0')
        return "0";
    return ret;
}
// @lc code=end

