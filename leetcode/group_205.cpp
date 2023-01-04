#include<string>
#include<sstream>

class Solution {
public:
    bool areNumbersAscending(std::string s) {
        int length = s.size();
        int pre = 0;
        std::string cur = "";
        std::stringstream ss;
        for(int i = 0;i < length;++i){
            if(s[i] <= '9' && s[i] >= '0'){
                cur += s[i];
            }
            else{
                if(cur != ""){
                    int icur = 0;
                    ss.clear();
                    ss.str("");
                    ss << cur;
                    ss >> icur;
                    if(icur <= pre){
                        return false;
                    }
                    else{
                        pre = icur;
                        cur = "";
                    }
                }
            }
        }
        if(cur != ""){
            int icur = 0;
            ss.clear();
            ss.str("");
            ss << cur;
            ss >> icur;
            if(icur <= pre){
                return false;
            }
        }
        return true;
    }
};