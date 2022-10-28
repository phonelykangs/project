/*
 * @lc app=leetcode.cn id=26 lang=c
 *
 * [26] 删除有序数组中的重复项
 */

// @lc code=start


int removeDuplicates(int* nums, int numsSize){
    int * pos = nums + 1;
    for(int i = 1;i < numsSize;++i){
        if(nums[i] != nums[i-1]){
            *pos++ = nums[i];
        }
    }
    return pos - nums;
}
// @lc code=end

