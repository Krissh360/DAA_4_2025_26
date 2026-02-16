// 24BCS11540 (Krissh Chhabra)

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    
    bool check(vector<int>& arr, int k, long long mid)
    {
        long long sum = 0;
        int painters = 1;
        
    
    for (int i = 0; i < arr.size(); i++) {
            
            if (arr[i] > mid) return false;
            
            if (sum + arr[i] <= mid)
            {
                sum += arr[i];
            }
            else
            {
                painters++;
                sum = arr[i];
            }
        }
        
        return painters <= k;
    }
    
    int minTime(vector<int>& arr, int k)
    {
        long long low = *max_element(arr.begin(), arr.end());
        long long high = 0;
        
        for (int i = 0; i < arr.size(); i++) high += arr[i];
        
        long long ans = high;
        
        while (low <= high)
        {
            long long mid = low + (high - low) / 2;
            
            if (check(arr, k, mid))
            {
                ans = mid;
                high = mid - 1;
            }
            else
            {
                low = mid + 1;
            }
        }
        
        return ans;
    }
};

int main() {
    Solution solution;
    vector<int> arr = {5, 10, 30, 20, 15};
    int k = 3;
    
    int result = solution.minTime(arr, k);
    cout << "Minimum time to paint all boards: " << result << endl;
    
    return 0;
}