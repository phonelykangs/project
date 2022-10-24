/*
 * @lc app=leetcode.cn id=2 lang=cpp
 *
 * [2] 两数相加
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
 struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode * fake_node = new ListNode();
        int ad = 0;
        ListNode * node = fake_node;
        while(l1 || l2){
            int i1 = l1 == nullptr ? 0 : l1->val;
            int i2 = l2 == nullptr ? 0 : l2->val; 
            if(l2){
                l2 = l2->next;
            }
            if(l1){
                l1 = l1->next;
            }
            int cur = ad + i1 + i2;
            ad = cur / 10;
            cur %= 10;
            ListNode * nodecur = new ListNode(cur);
            node->next = nodecur;
            node = nodecur;
        }
        if(ad){
            ListNode * nodecur = new ListNode(ad);
            node->next = nodecur;
            node = nodecur;
        }
        return fake_node->next;
    }
};
// @lc code=end

