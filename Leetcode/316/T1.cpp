#include<vector>
#include<string>
#include<iostream>

using namespace std;

class Solution {
public:
    bool haveConflict(vector<string>& event1, vector<string>& event2) {
        event1[0].erase(event1[0].begin() + 2);
        event1[1].erase(event1[1].begin() + 2);
        event2[0].erase(event2[0].begin() + 2);
        event2[1].erase(event2[1].begin() + 2);
        if ((stoi(event1[0]) <= stoi(event2[1]) && stoi(event1[1]) >= stoi(event2[0])) || (stoi(event2[0]) <= stoi(event1[1]) && stoi(event2[1]) >= stoi(event1[0]))){
            return true;
        }
        return false;
    }
};
