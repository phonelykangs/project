#include<stack>

class MinStack {  //  offer 30
public:
    MinStack() {}
    
    void push(int x) {
        stk.push(x);
        if(minstk.empty()){
            minstk.push(x);
        }
        else{
            minstk.push(std::min(x,minstk.top()));
        }
    }
    
    void pop() {
        stk.pop();
        minstk.pop();
    }
    
    int top() {
        return stk.top();
    }
    
    int min() {
        return minstk.top();
    }
private:
    std::stack<int> minstk;
    std::stack<int> stk;
};
