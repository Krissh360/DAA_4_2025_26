#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
  public:
  
    bool check(vector<int> &stalls, int k, int dist)
    {
        int count = 1;
        int first = stalls[0];
        
        for (int i = 1; i < stalls.size(); i++)
        {
            if (stalls[i] - first >= dist)
            {
                first = stalls[i];
                count++;
            }
        }
        
        return (count >= k);
    }
  
    int aggressiveCows(vector<int> &stalls, int k) {
        
        sort(stalls.begin(), stalls.end());
        int ans = 0;
        int low = 1, high = stalls.back() - stalls[0];
        
        while (low <= high)
        {
            int mid = low + (high - low) / 2;
            
            if (check(stalls, k, mid))
            {
                ans = mid;
                low = mid + 1;
            }
            
            else
            {
                high = mid - 1;
            }
        }
        
        return ans;
    }
};

int main()
{
    Solution sol;
    vector<int> stalls = {1, 2, 4, 8, 9};
    int k = 3;
    
    cout << sol.aggressiveCows(stalls, k) << endl;
    
    return 0;
}