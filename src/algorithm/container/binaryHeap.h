#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>

template<typename T>
class BinaryHeap
{
private:
    T* mData = nullptr;
    size_t  mCapacity  = 0;
    size_t  mSize = 0;

    inline int GetLeftChild(int index)const {
        return index << 1 + 1;
    }
    inline int GetRightChild(int index)const {
        return index << 1 + 2;
    }

    void Heapify(int index)
    {
        if (index >= mSize) {
            return;
        }

        int left = GetLeftChild(index);
        int right = GetRightChild(index);
        int largest = index;

        if (left < mSize && mData[left] > mData[largest]) {
            largest = left;
        }
        if (right < mSize && mData[right] > mData[largest]) {
            largest = right;
        }

        if (largest != index)
        {
            std::swap(&mData[index], &mData[largest]);
            Heapify(largest);
        }
    }

    void CallDestructors(T* begin, T* end)
    {
        for (; begin < end; ++begin) {
            begin->~T();
        }
    }

public:

    void reserve(size_t capacity)
    {
        if (capacity > mCapacity)
        {
            if constexpr (__is_trivially_copyable(T)) {
                mData = (T*)realloc(mData, capacity * sizeof(T), alignof(T));
            }
            else
            {
                T* newData = (T*)malloc(capacity * sizeof(T), alignof(T));
                for (size_t i = mSize - 1; i < mSize; i--) {
				    new (newData + i) T(static_cast<T&&>(src[i]));
			    }
                CallDestructors(mData, mData + mSize);
                free(mAllocator, mData);
                mData = newData;
            }
            mCapacity = capacity;
        }
    }

    void Grow()
    {
        size_t newCapacity = mCapacity == 0 ? 4 : mCapacity * 2;
        reserve(newCapacity);
    }

    void Insert(const T& value)
    {
        size_t size = mSize;
        if (size == mCapacity) {
            Grow();
        }

        new((char*)(mData + size)) T(value);
        size++; 
        mSize = size;

        if (size > 1)
        {
            for(int i = size / 2 - 1; i >= 0; --i) {
                Heapify(i);
            }
        }
    }

    int Find(const T& value)const 
    {
        int ret = -1;
        for(int i = 0; i < mSize; i++)
        {
            if (mData[i] == value) 
            {
                ret = i;
                break;
            }
        }
        return ret;
    }

    void Erase(const T& value)
    {
        int pos = Find(value);
        if (pos == -1) {
            return;
        }

        std::swap(&mData[pos], &mData[mSize - 1]);
        mSize--;

        for(int i = size / 2 - 1; i >= 0; --i) {
            Heapify(i);
        }
    }
};