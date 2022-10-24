/*
 * @lc app=leetcode.cn id=915 lang=cpp
 *
 * [915] 分割数组
 */

// @lc code=start
#include<vector>
#include<limits.h>
#include<iostream>

using namespace std;

class Solution {
public:
    int partitionDisjoint(vector<int>& nums) {
        int length = nums.size();
        vector<int> Min(length,0);
        vector<int> Max(length,0);
        int cur_max = INT_MIN;
        for(int i = 0;i < length;++i){
            cur_max = max(cur_max,nums[i]);
            Max[i] = cur_max;
        }
        int cur_min = INT_MAX;
        for(int i = length-1;i >= 0;--i){
            cur_min = min(cur_min,nums[i]);
            Min[i] = cur_min;
        }

        int i = 0;
        for(;i < length-1;++i){
            if(Max[i] <= Min[i+1]){
                break;
            }
        }
        return ++i;
    }
};
// @lc code=end

