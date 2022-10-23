/*
 * @lc app=leetcode.cn id=901 lang=cpp
 *
 * [901] 股票价格跨度
 */

// @lc code=start
#include<stack>
#include<iostream>
#include<tuple>
using namespace std;
class StockSpanner {
public:
    stack<pair<int,int>> stk;
    StockSpanner() {

    }
    
    int next(int price) {
        int ret = 1;
        if(stk.empty() || get<0>(stk.top()) > price){
            stk.push(make_pair(price,ret));
            return ret;
        }
        while(!stk.empty() && get<0>(stk.top()) <= price){
            ret += get<1>(stk.top());
            stk.pop();
        }
        stk.push(make_pair(price,ret));
        return ret;
    }
};

/**
 * Your StockSpanner object will be instantiated and called as such:
 * StockSpanner* obj = new StockSpanner();
 * int param_1 = obj->next(price);
 */
// @lc code=end