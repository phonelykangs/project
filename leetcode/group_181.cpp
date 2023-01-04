#include<vector>
#include<queue>

class Solution {
public:
    int getNumberOfBacklogOrders(std::vector<std::vector<int>>& orders) {  //  1801
        int length = orders.size();
        int MOD = 1000000007;
        int res = 0;
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int> >, std::less<std::pair<int, int> > > buyOrder;
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int> >, std::greater<std::pair<int, int> > > sellOrder;
        for(int i = 0;i < length;++i){
            int price = orders[i][0];
            int amount = orders[i][1];
            int orderType = orders[i][2];
            if(orderType == 0){
                while(amount > 0 && !sellOrder.empty() && price >= sellOrder.top().first){
                    auto sellAmount = sellOrder.top();
                    sellOrder.pop();
                    int minAmount = std::min(sellAmount.second,amount);
                    sellAmount.second -= minAmount;
                    amount -= minAmount;
                    if(sellAmount.second > 0){
                        sellOrder.push(sellAmount);
                    }
                }
                if(amount > 0){
                    buyOrder.push(std::make_pair(price,amount));
                }
            }
            else if(orderType == 1){
                while(amount > 0 && !buyOrder.empty() && price <= buyOrder.top().first){
                    auto buyAmount = buyOrder.top();
                    buyOrder.pop();
                    int minAmount = std::min(buyAmount.second,amount);
                    buyAmount.second -= minAmount;
                    amount -= minAmount;
                    if(buyAmount.second > 0){
                        buyOrder.push(buyAmount);
                    }
                }
                if(amount > 0){
                    sellOrder.push(std::make_pair(price,amount));
                }
            }
        }
        while(!buyOrder.empty()){
            res += (buyOrder.top().second);
            res %= MOD;
            buyOrder.pop();
        }
        while(!sellOrder.empty()){
            res += (sellOrder.top().second);
            res %= MOD;
            sellOrder.pop();
        }
        return res;
    }

    int maxValue(int n, int index, int maxSum) {  // 1802
        int left = 1;
        int right = maxSum;
        while(left < right){
            int mid = left + (right - left) / 2 + 1;
            if(valid(mid,index,n,maxSum)){
                left = mid;
            }
            else{
                right = mid - 1;
            }
        }
        return left;
    }

    bool valid(int mid, int index, int n ,int maxSum){
        int left = index;
        int right = n - index - 1;
        return mid + calculate(mid, left) + calculate(mid, right) <= maxSum;
    }

    long calculate(int big, int length){
        if(length+1 <= big){
            return (long)length * (long)(big-length+big-1) / 2;
        }
        else{
            return (long)(big-1) * (long)big / 2 + (long)(length+1-big);
        }
    }
};