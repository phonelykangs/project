#include<string>
#include<stack>

struct ListNode{
    int val;
    ListNode* next;
    ListNode() : val(0),next(nullptr) {}
    ListNode(int x) : val(x),next(nullptr) {}
    ListNode(int x,ListNode* next) : val(x),next(next) {}
};


class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {   // 19
        ListNode* fakeNode = new ListNode(0,head);
        ListNode* pre = fakeNode;
        ListNode* cur = head;
        while(n){
            head = head->next;
            --n;
        }
        while(head){
            head = head->next;
            pre = pre->next;
        }
        pre->next = pre->next->next;
        return fakeNode->next;
    }

    bool isValid(std::string s) {   // 20
        std::stack<char> stk;
        int length = s.size();
        for(int i = 0;i < length;++i){
            if(s[i] == '(' || s[i] == '{' || s[i] == '[')
                stk.push(s[i]);
            else{
                if(stk.empty())
                    return false;
                else if(s[i] == ')' && stk.top() == '(')
                    stk.pop();
                else if(s[i] == ']' && stk.top() == '[')
                    stk.pop();
                else if(s[i] == '}' && stk.top() == '{')
                    stk.pop();
                else
                    return false;
            }
        }
        return stk.empty();
    }
};