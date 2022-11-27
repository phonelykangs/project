/*
 * @lc app=leetcode.cn id=21 lang=cpp
 *
 * [21] 合并两个有序链表
 */

// @lc code=start
#include<climits>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode* fakeNode = new ListNode(0);
        ListNode* cur = fakeNode;
        while(list1 || list2){
            int val1 = list1 == nullptr ? 101 : list1->val;
            int val2 = list2 == nullptr ? 101 : list2->val;
            if(val1 >= val2){
                cur->next = list2;
                list2 = list2->next;
            }
            else{
                cur->next = list1;
                list1 = list1->next;
            }
            cur = cur->next;
        }
        return fakeNode->next;
    }
};
// @lc code=end

