#pragma once

#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

int Partition(std::vector<int>& array, int start, int end)
{
    int pivot = array[end];
    int i = start - 1;
    
    // partition all nums which less then pivot  
    for(int j = start; j < end; j++)
    {
        if (array[j] < pivot)
        {
            i++;
            std::swap(array[i], array[j]);
        }
    }

    // swap i+1 pivot
    std::swap(array[i + 1], array[end]);
    return i + 1;
}

void QuickSort(std::vector<int>& array, int start, int end) 
{
    int partiton = Partition(array, start, end);
    QuickSort(array, start, partiton - 1);
    QuickSort(array, partiton = 1， end);
}

int Test()
{
    std::vector<int> array = {1, 3, 6, 2, 1};
    QuickSort(array, 0, array.size() - 1);
}