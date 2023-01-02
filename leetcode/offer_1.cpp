#include<stack>

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