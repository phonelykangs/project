#include<stack>
#include<vector>

struct ListNode{
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
};


class Solution {
public:
    std::vector<int> reversePrint(ListNode* head) {  //offer 06
        std::stack<int> stk;
        while (head){
            stk.push(head->val);
            head = head->next;
        }
        std::vector<int> ret;
        while(!stk.empty()){
            ret.push_back(stk.top());
            stk.pop();
        }
        return ret;
    }
};

class CQueue {  //  offer 09
public:
    CQueue() {}
    
    void appendTail(int value) {
        tail.push(value);
    }
    
    int deleteHead() {
        int ret = 0;
        if(head.empty()){
            if(tail.empty()){
                ret = -1;
            }
            else{
                while(!tail.empty()){
                    head.push(tail.top());
                    tail.pop();
                }
                ret = head.top();
                head.pop();
            }
        }
        else{
            ret = head.top();
            head.pop();
        }
        return ret;
    }
private:
    std::stack<int> head;
    std::stack<int> tail;
};