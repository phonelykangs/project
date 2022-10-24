/*
 * @lc app=leetcode.cn id=9 lang=cpp
 *
 * [9] 回文数
 */

// @lc code=start

#include<sstream>>
#include<string>

using namespace std;

class Solution {
public:
    bool isPalindrome(int x) {
        string s = "";
        stringstream ss;
        ss << x;
        ss >> s;
        if(s[0] < '0' || s[0] > '9'){
            return false;
        }
        int length = s.size();
        for(int i = 0;i <= length / 2;++i){
            if(s[i] != s[length-1-i]){
                return false;
            }
        }
        return true;
    }
};
// @lc code=end

