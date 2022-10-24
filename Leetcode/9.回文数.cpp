/*
 * @lc app=leetcode.cn id=9 lang=cpp
 *
 * [9] 回文数
 */

// @lc code=start
#include<iostream>
class Solution {
public:
    bool isPalindrome(int x) {
        if(x < 0 || (x % 10 == 0 && x != 0)){
            return false;
        }
        int revnum = 0;
        int temp = x;
        while(x > revnum){
            revnum = revnum * 10 + (x % 10);  
            x /= 10;
        }
        std::cout << revnum << std::endl;
        return x == revnum || x == revnum / 10;
    }
};
// @lc code=end

