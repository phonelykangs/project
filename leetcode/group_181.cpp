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
};