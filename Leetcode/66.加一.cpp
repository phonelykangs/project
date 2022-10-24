/*
 * @lc app=leetcode.cn id=66 lang=cpp
 *
 * [66] 加一
 */

// @lc code=start
#include<vector>
using namespace std;

class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        int length = digits.size();
        int ad = 0;
        digits[length-1] += 1;
        for(int i = length-1;i >= 0;--i){
            digits[i] = ad + digits[i];
            ad = digits[i] / 10;
            digits[i] %= 10;
        }
        if(ad){
            digits.insert(digits.begin(),1);
        }
        return digits;
    }
};
// @lc code=end

