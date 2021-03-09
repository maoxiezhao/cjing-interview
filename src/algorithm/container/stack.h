#pragma once

#include <iostream>
#include <math.h>
#include <vector>

// A type-unsafe stack for storing different types of data.
class Stack
{
private:
    char* mStackData = nullptr;
    char* mStackTop = nullptr;
    char* mStackEnd = nullptr;
    size_t mInitialCapacity = 0;

    void Reserve(size_t capactity)
    {
        size_t size = GetSize();
        mStackData = (char*)realloc(mStackData, capactity);
        mStackTop = mStackData + size;
        mStackEnd = mStackData + capactity;
    }

    void Expend(size_t size)
    {
        size_t newCapacity = 0;
        if (mStackData == nullptr) {
            newCapacity = mInitialCapacity;
        }
        else {
            newCapacity = GetCapacity() * 2;
        }

        if (newCapacity < size) {
            newCapacity = size;
        }

        Reserve(newCapacity);
    }

public:
    Stack(size_t capacity) :
        mInitialCapacity(capacity) 
    {}

    ~Stack()
    {
        Free();
    }

    void Free()
    {
        free(mStackData);
        mStackData = nullptr;
        mStackTop  = nullptr;
        mStackEnd  = nullptr;
    }

    template<typename T>
    T* Push(int count)
    {
        if (sizeof(T) * count > (mStackEnd - mStackTop)) {
            Expend(sizeof(T) * count);
        }

        T* ret = reinterpret_cast<T*>(mStackTop);
        mStackTop += sizeof(T) * count;
        return ret;
    }

    template<typename T>
    void Pop(int count)
    {
        mStackTop -= count * sizeof(T);
    }

    bool IsEmpty()const{
        return GetSize() <= 0;
    }
    size_t GetSize()const {
        return static_cast<size_t>(mStackTop - mStackData);
    }
    size_t GetCapacity()const {
        return static_cast<size_t>(mStackEnd - mStackData);
    }

};