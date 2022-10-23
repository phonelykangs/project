#include<vector>
#include<string>
#include<iostream>

using namespace std;

int gcd(int n, int m) {
    return m ? gcd(m, n % m) : n;
}

class Solution {
public:
    int subarrayGCD(vector<int>& nums, int k) {
        int length = nums.size();
        int start = 0;
        int end = 0;
        int ans = 0;
        while (start <= end && start < length) {
            if (end < length && nums[end] % k == 0) {
                int cur = nums[start];
                for (int i = start; i <= end; ++i) {
                    cur = gcd(cur, nums[i]);
                }
                if (cur == k) {
                    ++ans;
                }
                ++end;
            }
            else{
                end = start;
                ++start;
                ++end;
            }
        }
        return ans;
    }
};
