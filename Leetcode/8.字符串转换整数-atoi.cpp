/*
 * @lc app=leetcode.cn id=8 lang=cpp
 *
 * [8] 字符串转换整数 (atoi)
 */

// @lc code=start
#include<string>
#include<iostream>
#include<stdint.h>

using namespace std;
class Solution {
public:
    int myAtoi(string s) {
        int length = s.size();
        int start = 0;
        while(start < length && s[start] == ' '){
            ++start;
        }
        if(start == length){
            return 0;
        }
        int sign = 0;
        if(s[start] == '-'){
            sign = -1;
            ++start;
        }
        else if(s[start] == '+'){
            sign = 1;
            ++start;
        }
        else{
            sign = 1;
        }
        if(start == length){
            return 0;
        }
        while(start < length && s[start] == '0'){
            ++start;
        }
        if(start == length){
            return 0;
        }
        //string num = "";
        int64_t ret = 0;
        while(start < length && s[start] <= '9' && s[start] >= '0'){
            //num += s[start];
            ret = ret * 10 + (int64_t)(s[start] - '0') * sign;
            if(ret >= INT32_MAX){
                return INT32_MAX;
            }
            else if(ret <= INT32_MIN){
                return INT32_MIN;
            }
            ++start;
        }
        //cout << num << endl;
        return ret;
    }
};
// @lc code=end

