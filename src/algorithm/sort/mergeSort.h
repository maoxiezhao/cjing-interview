#pragma once

#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

void Merge(std::vector<int>& array, int start, int mid, int end)
{
    std::vector<int> leftArray, rightArray;
    for(int i = start; i <= mid; i++) {
        leftArray.push_back(array[i]);
    }
    for(int i = mid + 1; i < end; i++) {
        rightArray.push_back(array[i]);
    }

    int i = 0, j = 0;
    int destIndex = start;
    int numLeft = leftArray.size();
    int numRight = rightArray.size();
    while(i < numLeft && j < numRight)
    {
        if (leftArray[i] <= rightArray[j])
        {
            array[destIndex] = leftArray[i];
            i++;
        }
        else 
        {
            array[destIndex] = rightArray[j];
            j++;
        }
        destIndex++;
    }

    // push remaining nums
    while (i < numLeft) {
        array[destIndex++] = leftArray[i++];
    }

    while (j < numRight) {
        array[destIndex++] = rightArray[j++];
    }
}

void MergeSort(std::vector<int>& array, int start, int end)
{
    if (start >= end) {
        return;
    }

    int mid = start + (start + end) / 2;
    MergeSort(array, start, mid);
    MergeSort(array, mid + 1, end);

    Merge(array, start, mid, end);
}

int Test()
{
    std::vector<int> array = {1, 3, 6, 2, 1};
    MergeSort(array, 0, array.size() - 1);
}