//24BCS11540 (Krissh Chhabra)

#include <iostream>
#include <vector>


class Solution {
public:
    void reorderList(ListNode* head) {

        vector<int> original;
        vector<int> rev;

        ListNode* temp = head;

        while (temp != NULL)
        {
            original.push_back(temp->val);
            temp = temp->next;
        }

        for (int i = original.size() - 1; i >= 0; i--)
        {
            rev.push_back(original[i]);
        }

        int n = original.size();
        int i = 0, j = 0;

        temp = head;

        for (int k = 0; k < n && temp != NULL; k++)
        {
            if (k % 2 == 0)
            {
                temp->val = original[i++];
            }
            else
            {
                temp->val = rev[j++];
            }

            temp = temp->next;

            if (i > n-1-j) break; 
        }
    }
};

