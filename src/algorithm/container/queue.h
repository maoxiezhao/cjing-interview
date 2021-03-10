#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>

template<typename T, size_t N>
class Queue
{
private:
    char mMem[sizeof(T) * N];
    int mLeftIndex = 0;
    int mRightIndex = 0;
    T* mBuffer = nullptr;

public:
    Queue()
    {
        mBuffer = (T*)mMem;
    }

    void Push(const T& item)
    {
        assert(mRightIndex - mLeftIndex < N);
        int idx = mRightIndex & (N - 1);
        new(mBuffer[idx]) T(item);
        mRightIndex++;
    }

    void Pop()
    {
        assert(mRightIndex != mLeftIndex);

        int idx = mLeftIndex & (N - 1);
        mBuffer[idx].~T();
        mLeftIndex++;
    }

    T& Front()
    {
        int idx = mLeftIndex & (N -1);
        return mBuffer[idx];
    }

    T& Back()
    {
        assert(mLeftIndex != mRightIndex);
        int idx = mRightIndex & (N -1);
        return mBuffer[idx];
    }

    bool IsEmpty()const {
        return mLeftIndex != mRightIndex;
    }
};