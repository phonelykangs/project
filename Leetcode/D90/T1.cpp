#include<string>
#include<vector>

using namespace std;

class Solution {
public:
    string oddString(vector<string>& words) {
        int rows = words.size();
        int columns = words[0].size()-1;
        vector<vector<int>> diff(rows,vector<int>(columns,0));
        for(int i = 0;i < rows;++i){
            for(int j = 0;j < columns;++j){
                diff[i][j] = words[i][j+1] - words[i][j];
            }
        }
        int i = 0;
        int idx = 0;
        for(;i < rows-1;++i){
            for(int j = 0;j < columns;++j){
                if(diff[i][j] != diff[i+1][j]){
                    idx = i;
                    i = rows;
                    break;
                }
            }
        }
        i = idx;
        if(i > 0){
            for(int j = 0;j < columns;++j){
                if(diff[i][j] != diff[i-1][j]){
                    return words[i];
                }
            }
            return words[i+1];
        }
        else if(i + 1 < rows - 1){
            for(int j = 0;j < columns;++j){
                if(diff[i + 1][j] != diff[i + 2][j]){
                    return words[i+1];
                }
            }
            return words[i];            
        }
        return words[i];
    }
};