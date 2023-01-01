#include<vector>
#include<algorithm>

class Solution {
public:
    int minMovesToSeat(std::vector<int>& seats, std::vector<int>& students) {  //2037
        int res = 0;
        int length = seats.size();
        std::sort(seats.begin(),seats.end());
        std::sort(students.begin(),students.end());
        for(int i = 0;i < length;++i){
            res += abs(seats[i] - students[i]);
        }
        return res;
    }
};