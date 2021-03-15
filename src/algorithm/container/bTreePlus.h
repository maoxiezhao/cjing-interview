#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>

// B+ Tree
template<size_t T>
class BTreePlus
{
private:
    struct NodeElement
    {
        int mKeys[2 * T - 1];
        NodeElement* mChildren[2 * t];
        bool mIsLeaf = true;
        int mSize = 0;
    };
public:

};