/*
 * @lc app=leetcode.cn id=26 lang=c
 *
 * [26] 删除有序数组中的重复项
 */

// @lc code=start
#include<stdio.h>

int removeDuplicates(int* nums, int numsSize){
    int flag = -10000-1;
    int * pos = nums;
    for(int i = 0;i < numsSize;++i){
        if(nums[i] != flag){
            flag = nums[i];
            *pos = flag;
            ++pos;
        }
    }
    return pos - nums;
}
// @lc code=end

