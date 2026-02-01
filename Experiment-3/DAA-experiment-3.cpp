//UID - 24BCS11540 (Krissh Chhabra)

#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin >> n;

    vector<char> records(n);
    for (int i = 0; i < n; i++) cin >> records[i];

    unordered_map<int, int> firstIdx;
    int sum = 0;
    int maxLen = 0;

    firstIdx[0] = -1;

    for (int i = 0; i < n; i++)
    {
        if (records[i] == 'P') sum += 1;
        else sum -= 1;

        if (firstIdx.find(sum) != firstIdx.end())
        {
            int len = i - firstIdx[sum];
            maxLen = max(maxLen, len);
        } 
        else
        {firstIdx[sum] = i;}
    }

    cout << maxLen << endl;
    return 0;
}

/*
output for the input:
5
P
P
A
A
P
4
*/