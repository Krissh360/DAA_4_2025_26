#include <bits/stdc++.h>

class Solution {
public:
    int n;
    int ans = INT_MAX;

    void solve(vector<vector<int>>& cost, vector<bool>& visited, int pos, int count, int currCost) {
        if (count == n && cost[pos][0] != 0) {
            ans = min(ans, currCost + cost[pos][0]);
            return;
        }

        for (int i = 0; i < n; i++) {
            if (!visited[i] && cost[pos][i] != 0) {
                visited[i] = true;
                solve(cost, visited, i, count + 1, currCost + cost[pos][i]);
                visited[i] = false;
            }
        }
    }

    int totalCost(vector<vector<int>>& cost) {
        n = cost.size();
        vector<bool> visited(n, false);
        visited[0] = true;

        solve(cost, visited, 0, 1, 0);

        return ans;
    }
};