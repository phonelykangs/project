/*
 * @lc app=leetcode.cn id=934 lang=cpp
 *
 * [934] 最短的桥
 */

// @lc code=start

#include<vector>
#include<queue>
#include<tuple>
#include<iostream>

using namespace std;

class Solution {
public:

    vector<vector<int>> directions = {
        {1,0},
        {-1,0},
        {0,1},
        {0,-1}
    };

    int shortestBridge(vector<vector<int>>& grid) {
        int rows = grid.size();
        int columns = grid[0].size();
        queue<pair<int,int>> vst;
        vector<pair<int,int>> island;
        for(int i = 0;i < rows;++i){
            for(int j = 0;j < columns;++j){
                if(grid[i][j] == 1){
                    vst.push(make_pair(i,j));
                    grid[i][j] = 2;
                    island.push_back(make_pair(i,j));
                    while(!vst.empty()){
                        int x = get<0>(vst.front());
                        int y = get<1>(vst.front());
                        vst.pop();
                        for(int m = 0;m < directions.size();++m){
                            int nx = x + directions[m][0];
                            int ny = y + directions[m][1];
                            if(nx < 0 || nx >= rows || ny < 0 || ny >= columns || grid[nx][ny] != 1){
                                continue;
                            }
                            grid[nx][ny] = 2;
                            island.push_back(make_pair(nx,ny));
                            vst.push(make_pair(nx,ny));
                        }
                    }
                }
                for(auto& i : island){
                    vst.push(i);
                }
                int step = 0;
                while(!vst.empty()){
                    int sz = vst.size();
                    for(int k = 0;k < sz;++k){
                        int x = get<0>(vst.front());
                        int y = get<1>(vst.front());
                        vst.pop();
                        for(int m = 0;m < directions.size();++m){
                            int nx = x + directions[m][0];
                            int ny = y + directions[m][1];
                            if(nx < 0 || nx >= rows || ny < 0 || ny >= columns || grid[nx][ny] == 2){
                                continue;
                            }
                            else if(grid[nx][ny] == 1){
                                return step;
                            }
                            else{
                                grid[nx][ny] = 2;
                            }
                            vst.push(make_pair(nx,ny));
                        }
                    }
                    ++step;
                }
            }
        }
        return 0;
    }
};
// @lc code=end