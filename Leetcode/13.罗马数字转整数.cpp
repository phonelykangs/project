/*
 * @lc app=leetcode.cn id=13 lang=cpp
 *
 * [13] 罗马数字转整数
 */

// @lc code=start
#include<string>

using namespace std;
class Solution {
public:
    int ctoi(char c){
        int ret = 0;
        switch (c)
        {
        case 'I':
            ret = 1;
            break;
        case 'V':
            ret = 5;
            break;
        case 'X':
            ret = 10;
            break;
        case 'L':
            ret = 50;
            break;
        case 'C':
            ret = 100;
            break;
        case 'D':
            ret = 500;
            break;
        case 'M':
            ret = 1000;
            break;
        default:
            break;
        }
        return ret;
    }
    
    int romanToInt(string s) {
        int length = s.size();
        int ans = 0;
        ans += ctoi(s[length-1]);
        for(int i = length-2;i >= 0;--i){
            if(ctoi(s[i]) < ctoi(s[i+1])){
                ans -= ctoi(s[i]);
            }
            else{
                ans += ctoi(s[i]);
            }
        }
        return ans;
    }
};
// @lc code=end

