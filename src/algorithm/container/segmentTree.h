#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>

// https://www.hackerearth.com/practice/data-structures/advanced-data-structures/segment-trees/tutorial/
// https://blog.csdn.net/zearot/article/details/48299459

// 线段树本职上是一个二叉数，只不过一个节点包括了一个线段范围[0, N] 同时左节点则包括了[0, N/2],
// 右节点包括了(N/2, N],知道每个节点的范围为1
class SegmentTree
{
private:
    struct Node
    {
        int mLeft = 0;
        int mIsLeaf = true;
        int mValue = 0; // 父节点会算子节点的和
    };
    Node* mNodeList = nullptr;
    int*  mLazyList = nullptr;  // 当更新某一个范围的节点数据时，仅仅只更新满足范围的父节点，并打上lazy标记
                                // 同时在查询或者更新时再下推lazy标记，可以减少时间复杂度
    int mSize = 0;
 
private:
    inline int LeftChild(int root) const { return root << 1; }
    inline int RightChild(int root) const { return root << 1 + 1; }
    inline void PushUp(int root)
    {
        mNodeList[root].mValue = mNodeList[root << 1].mValue + mNodeList[root << 1 + 1].mValue;
    }

    inline void PushDown(int root, int leftChildrenCount, int rightChildrenCount)
    {
        // 下推lazy标记,同时更新children节点Sum值
        if (mLazyList[root] > 0)
        {
            // update lazy list
            mLazyList[LeftChild(root)] += mLazyList[root];
            mLazyList[RightChild(root)] += mLazyList[root];

            // update children values
            mNodeList[LeftChild(root)].mValue += mLazyList[root] * leftChildrenCount;
            mNodeList[RightChild(root)].mValue += mLazyList[root] * rightChildrenCount;

            mLazyList[root] = 0;
        }
    }

    void BuildImpl(int root, int left, int right)
    {
        if (left > right || root >= mSize) {
            return;
        }
        if (left == right)
        {
            mNodeList[root].mLeft = left;
            mNodeList[root].mIsLeaf = true;
            mNodeList[root].mValue = 1; // Value[left]
        }
        else
        {
            int mid = (left + right) / 2;
            BuildImpl(root * 2, left, mid);
            BuildImpl(root * 2 + 1, mid + 1, right);
            PushUp(root);
        }
    }

public:
    // 点修改
    void Update(int root, int key, int value, int left, int right)
    {
        if (left == right) 
        {
            mNodeList[root].mValue += value;
            return;
        }

        int mid = (left + right) / 2;
        if (key < mid) {
            Update(root * 2, key, value, left, mid);
        }
        else {
            Update(root * 2 + 1, key, value, mid + 1, right);
        }
        PushUp(root);
    }

    // 区间修改
    void RangeUpdate(int root, int rangeLeft, int rangeRight, int value, int left, int right)
    {
        if (rangeLeft <= left && rangeRight <= right)
        {
            mNodeList[root].mValue += value * (right - left + 1);
            mLazyList[root] += value; //增加Lazy标记，表示本区间的Sum正确，子区间的Sum仍需要根据Lazy的值来调整
            return;
        }
        int mid = (left + right) / 2;
        // 下推标记，保证value正确更新
        PushDown(root, mid - left + 1, right - mid);
        if (rangeLeft <= mid) {
            RangeUpdate(root * 2, rangeLeft, rangeRight, value, left, mid);
        }
        if (rangeRight > mid){
            RangeUpdate(root * 2 + 1, rangeLeft, rangeRight, value, mid + 1, right);
        }
        PushUp(root);
    }

    int Query(int root, int rangeLeft, int rangeRight, int left, int right)
    {
        if (rangeLeft <= left && rangeRight <= right) {
            return mNodeList[root].mValue;
        }

        int mid = (left + right) / 2;
        // 下推标记，保证value正确更新
        PushDown(root, mid - left + 1, right - mid);

        int ret = 0;
        if (rangeLeft <= mid) {
            ret += Query(root * 2, rangeLeft, rangeRight, left, mid);
        }
        if (rangeRight > mid){
            ret += Query(root * 2 + 1, rangeLeft, rangeRight, mid + 1, right);
        }
        return ret;
    }
};