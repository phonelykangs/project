/*
 * @lc app=leetcode.cn id=907 lang=cpp
 *
 * [907] 子数组的最小值之和
 */

// @lc code=start
#include<vector>
#include<stack>
#include<iostream>

using namespace std;

#define     MOD     (1000000000+7)

class Solution {
public:
    int sumSubarrayMins(vector<int>& arr) {
        int length = arr.size();
        vector<int> left(length,0);
        vector<int> right(length,0);
        stack<int> minStack;
        long long ans = 0;
        for(int i = 0;i < length;++i){
            if(minStack.empty()){
                left[i] = -1;
                minStack.push(i);
                continue;
            }
            while(!minStack.empty() && arr[minStack.top()] >= arr[i]){
                minStack.pop();
            }
            if(minStack.empty()){
                left[i] = -1;
            }
            else{
                left[i] = minStack.top();
            }
            minStack.push(i);
        }
        while(!minStack.empty()){
            minStack.pop();
        }
        for(int i = length-1; i >= 0;--i){
            if(minStack.empty()){
                right[i] = length;
                minStack.push(i);
                continue;
            }
            while(!minStack.empty() && arr[minStack.top()] > arr[i]){
                minStack.pop();
            }
            if(minStack.empty()){
                right[i] = length;
            }
            else{
                right[i] = minStack.top();
            }
            minStack.push(i);
        }
        for(int i = 0;i < length;++i){
            //cout << ans << endl;
            ans += (long long)((right[i] - i) * (i - left[i])) * arr[i];
            ans %= MOD;
        }
        return ans;
    }
};
// @lc code=end

