#include<string>
#include<vector>
#include<unordered_map>
#include<iostream>

class Solution {
public:
    char repeatedCharacter(std::string s) {  //  2351
        std::vector<int> count(26,0);
        int length = s.size();
        for(int i = 0;i < length;++i){
            int idx = s[i] - 'a';
            if(count[idx] > 0){
                return s[i];
            }
            ++count[idx];
        }
        return s[length-1];
    }

    char repeatedCharacter__1(std::string s) {  //  2351
        int length = s.size();
        int seen = 0;
        for(int i = 0;i < length;++i){
            int idx = s[i] - 'a';
            if(seen & (1 << idx)){
                return s[i];
            }
            seen |= (1 << idx);
        }
        return s[length-1];
    }

    int equalPairs(std::vector<std::vector<int>>& grid) {  //  2352
        int res = 0;
        int rows = grid.size();
        if(rows == 0){
            return 0;
        }
        int columns = grid[0].size();
        std::unordered_map<std::string,int> u_map;
        for(int i = 0;i < rows;++i){
            std::string cur = "";
            for(int j = 0;j < columns;++j){
                cur += std::to_string(grid[i][j]);
                cur += ",";
            }
            ++u_map[cur];
        }
        // for(auto iter = u_map.begin();iter != u_map.end();++iter){
        //     std::cout << iter->first << " " << iter->second << std::endl;
        // }
        for(int i = 0;i < columns;++i){
            std::string cur = "";
            for(int j = 0;j < rows;++j){
                cur += std::to_string(grid[j][i]);
                cur += ",";
            }
            res += u_map[cur];
        }
        return res;
    }
};