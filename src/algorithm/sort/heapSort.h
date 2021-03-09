#pragma once

#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

void SetupHeap(vector<int>& arr, int i)
{
    if (i >= arr.size()) {
        return;
    }

    // find which child is largest
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int largest = i;

    if (left < arr.size() && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < arr.size() && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) 
    {
        std::swap(arr[i], arr[largest]);
        SetupHeap(arr, largest);
    }
}

void HeapSort(std::vector<int>& arr, std::vector<int>& out)
{
    // setup max heap
    for(int i = arr.size() / 2 - 1; i >= 0; i--) {
        SetupHeap(arr, i);
    }

    out.reserve(arr.size());

    for(int i = arr.size() - 1; i >= 0; i--)
    {
        std::swap(arr[0], arr[i]);
        out.push_back(arr[i]);
        arr.pop_back();

        SetupHeap(arr, 0);
    }
}

void Test()
{
    std::vector<int> out;
    std::vector<int> array = {1, 3, 6, 2, 1};
    HeapSort(array, out);
}