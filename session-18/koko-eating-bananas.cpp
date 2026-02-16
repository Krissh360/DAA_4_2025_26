// 24BCS11540 (Krissh Chhabra)

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:

    bool check(vector<int> &piles, int mid, int h)
    {
        long long sum = 0;

        for (int i = 0; i < piles.size(); i++)
        {
            sum += (piles[i] + mid - 1) / mid;
        }

        return sum <= h;
    }

    int minEatingSpeed(vector<int>& piles, int h) {

        int n = piles.size();
        int mx = piles[0];

        for (int i = 1; i < n; i++)
        {
            if (piles[i] > mx)  mx = piles[i];
        }

        int low = 1, high = mx;
        int ans = mx;

        while (low <= high)
        {
            int mid = low + (high - low) / 2;

            if (check(piles, mid, h))
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


int main()
{
    Solution sol;
    vector<int> piles = {30,11,23,4,20};
    int h = 5;

    cout << sol.minEatingSpeed(piles, h) << endl;

    return 0;
}