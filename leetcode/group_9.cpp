#include<vector>
#include<climits>

struct ListNode{
    int val;
    ListNode* next;
    ListNode() : val(0),next(nullptr){}
    ListNode(int x) : val(x), next(nullptr){}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* partition(ListNode* head, int x) {   // 86
        ListNode* minHead = new ListNode(-101);
        ListNode* maxHead = new ListNode(-101);
        ListNode* curMin = minHead;
        ListNode* curMax = maxHead;
        while(head){
            if(head->val < x){
                curMin->next = head;
                curMin = curMin->next;
            }
            else{
                curMax->next = head;
                curMax = curMax->next;
            }
            head = head->next;
        }
        curMin->next = maxHead->next;
        curMax->next = nullptr;
        return minHead->next;
    }

    void merge(std::vector<int>& nums1, int m, std::vector<int>& nums2, int n) {   // 88
        int length = nums1.size();
        int cur = length - 1;
        while(m > 0 || n > 0){
            int value1 = m > 0 ? nums1[m-1] : INT_MIN;
            int value2 = n > 0 ? nums2[n-1] : INT_MIN;
            if(value1 > value2){
                nums1[cur] = value1;
                --m;
            }
            else{
                nums1[cur] = value2;
                --n;
            }
            --cur;
        }
    }
};