/*
 * @lc app=leetcode.cn id=934 lang=cpp
 *
 * [934] 最短的桥
 */

// @lc code=start
#include<vector>
#include<iostream>

using namespace std;

class Solution {
public:

    void dfs(vector<vector<int>>& nums, int i, int j, int rows, int columns){
        if(i < 0 || i >= rows || j < 0 || j >= columns || nums[i][j] != 1){
            return ;
        }
        nums[i][j] = 2;
        dfs(nums,i+1,j,rows,columns);
        dfs(nums,i-1,j,rows,columns);
        dfs(nums,i,j+1,rows,columns);
        dfs(nums,i,j-1,rows,columns);
    }

    int shortestBridge(vector<vector<int>>& grid) {
        int rows = grid.size();
        int columns = grid[0].size();
        int ans = 10000;
        for(int i = 0;i < rows;++i){
            for(int j = 0;j < columns;++j){
                if(grid[i][j] == 1){
                    dfs(grid,i,j,rows,columns);
                    i = rows;
                    j = columns;
                }
            }
        }
        for(int i = 0;i < rows;++i){
            for(int j = 0;j < columns;++j){
                if(grid[i][j] == 2){
                    for(int m = 0;m < rows;++m){
                        for(int n = 0;n < columns;++n){
                            if(grid[m][n] == 1){
                                ans = min(ans,abs(m-i)+abs(n-j)-1);
                            }
                        }
                    }
                }
            }
        }
        return ans;
    }
};
// @lc code=end

