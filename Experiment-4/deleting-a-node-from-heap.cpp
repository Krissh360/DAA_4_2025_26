//24BCS11540 (Krissh Chhabra)

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void heapifyDown(vector<int>& heap, int n, int i)
{
    int largest = i;

    while (true)
    {
        int left = 2*i + 1;
        int right = 2*i + 2;

        if (left < n && heap[left] > heap[largest])
            largest = left;
        if (right < n && heap[right] > heap[largest])
            largest = right;
        if (largest == i) break;

        swap(heap[i], heap[largest]);
        i = largest;
    }
}

void heapifyUp(vector<int>& heap, int i)
{
    while (i > 0)
    {
        int parent = (i - 1) / 2;
        if (heap[parent] >= heap[i]) break;
        swap(heap[parent], heap[i]);
        i = parent;
    }
}

void deleteAtIndex(vector<int>& heap, int& n, int index)
{
    if (index >= n) return;

    heap[index] = heap[n - 1];
    n--;

    heapifyDown(heap, n, index);
    heapifyUp(heap, index);
}

int main()
{
    vector<int> heap = {50, 30, 40, 10, 5, 20, 30};
    int n = heap.size();

    deleteAtIndex(heap, n, 2); 

    for (int i = 0; i < n; i++)
        cout << heap[i] << " ";
}
