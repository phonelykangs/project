#include<vector>
#include<string>
using namespace std;

class Solution {
public:
    vector<string> twoEditWords(vector<string>& queries, vector<string>& dictionary) {
        int q = queries.size();
        int d = dictionary.size();
        int length = queries[0].size();
        vector<string> ret;
        for(int i = 0;i < q;++i){
            for(int j = 0;j < d;++j){
                int count = 0;
                for(int k = 0;k < length;++k){
                    if(queries[i][k] != dictionary[j][k]){
                        ++count;
                    }
                    if(count > 2){
                        break;
                    }
                }
                if(count <= 2){
                    ret.push_back(queries[i]);
                    break;
                }
            }
        }
        return ret;
    }
};