#include<string>
#include<vector>

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
        
    }
};