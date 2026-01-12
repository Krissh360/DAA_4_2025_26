// UID: 24BCS11540

#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

void complexRec(int n) {

    int opcount = 0, depth = 0;

   if (n <= 2) {
       return;
   }


   int p = n;
   while (p > 0) {
       vector<int> temp(n);
       for (int i = 0; i < n; i++) {
           temp[i] = i ^ p;;
       }
       p >>= 1;
   }


   vector<int> small(n);
   for (int i = 0; i < n; i++) {
       small[i] = i * i;
       opcount++;
   }


   if (n % 3 == 0) {
       reverse(small.begin(), small.end());
   } else {
       reverse(small.begin(), small.end());
   }


   complexRec(n / 2);
   complexRec(n / 2);
   complexRec(n / 2);

   depth++;

   cout << "total operation count: " << opcount << "  total depth: " << depth << endl;
}


int main()
{

    complexRec(4);
    auto start = high_resolution_clock::now();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    cout << "time taken by program:" << duration.count();
}

/*
output: total operation count: 16  total depth: 1
time taken by program:0
*/ 