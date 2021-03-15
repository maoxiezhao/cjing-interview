#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>

// 1. For each node x, the keys are stored in increasing order.
// 2. In each node, there is a boolean value x.leaf which is true if x is a leaf.
// 3. If n is the order of the tree, each internal node can contain at most n - 1 keys along with a pointer to each child.
// 4. Each node except root can have at most n children and at least n/2 children.
// 5. All leaves have the same depth (i.e. height-h of the tree).
// 6. The root has at least 2 children and contains a minimum of 1 key.
// 7. If n ≥ 1, then for any n-key B-tree of height h and minimum degree t ≥ 2, h ≥ logt (n+1)/2.

template<size_t T>
class BTree
{
public:
    struct NodeElement
    {
        int mKeys[2 * T - 1];
        NodeElement* mChildren[2 * t];
        bool mIsLeaf = true;
        int mSize = 0;

        NodeElement()
        {
            memset(mKeys, 0, sizeof(int) * (2 * T - 1));
            memset(mChildren, nullptr, sizeof(NodeElement*) * (2 * T));
        }

        int GetKeySizeThreshold()const
        {
            return 2 * T - 1;
        }

        // ex. T = 2, M = 3, split [5,6,7]
        //        4 9               4 6 9
        //          \       =>       / \
        //         5 6 7            5   7
        void SplitChild(int childPos, NodeElement* child)
        {
            int midKey = child->mKeys[T - 1];
            // 创建一个新节点，并赋予child mid key右边的keys
            NodeElement* newElem = new NodeElement();
            newElem->mSize = T - 1;
            newElem->mIsLeaf = child->mIsLeaf;

            for(int i = 0; i < M - 1; i++) {
                newElem->mKeys[i] = child->mKeys[i + T];
            }
            // 如果不是叶子节点，还需赋值mid右边children指针
            if (!child->mIsLeaf)
            {
                for(int i = 0; i < M; i++) {
                    newElem->mChildren[i] = child->mChildren[i + T];
                }
            }

            // 重新设置child值，仅保留mid左边数据
            child->mSize = T - 1;
           
            // 将newElem和mid key添加到当前节点中
            for(int i = mSize; i >= childPos + 1; i--) {
                mChildren[i + 1] = mChildren[i];
            }
            for(int i = mSize - 1; i >= childPos; i--) {
                mKeys[i + 1] = mKeys[i];
            }

            mKeys[childPos] = midKey;
            mChildren[childPos + 1] = newElem;
            mSize++;
        }

        void InsertNotFull(int key)
        {
            int pos = mSize - 1;
            if (mIsLeaf)
            {
                // 如果是叶子节点，则直接在对应的位置插入数据
                assert(mSize < GetKeySizeThreshold());

                while(pos >= 0 && mKeys[pos] > key) 
                {
                    mKeys[pos + 1] = mKeys[pos];
                    pos--;
                }
                mKeys[pos + 1] = key;
                mSize++;
            }
            else
            {
                // 如果是internalNode,先找到合适的子节点位置
                // 如果子节点已经满，则先分割子节点，再插入
                while(pos >= 0 && mKeys[pos] > key)  {
                    pos--;
                }
                pos++;
                
                if (mChildren[pos]->mSize >= GetKeySizeThreshold())
                {
                    SplitChild(pos, mChildren[pos]);
                    // 子节点分割后，会有子节点的中间key值添加到当前节点中
                    // 所以需要重新检查下位置
                    if (mKeys[pos] < key) {
                        pos++;
                    }
                }
                mChildren[pos]->InsertNotFull(key);
            }
        }
    }
    NodeElement* mRoot = nullptr;

public:
    void Insert(int key)
    {
        if (mRoot == nullptr)
        {
            mRoot = new NodeElement();
            mRoot->mIsLeaf = true;
            mRoot->mKeys[0] = key;
            mRoot->mSize = 1;
        }
        else
        {
            if (mRoot->mSize < mRoot->GetKeySizeThreshold())
            {
                mRoot->InsertNotFull(key);
            }
            else
            {
                // 如果根节点已满，则分割根节点
                NodeElement* newElem = new NodeElement();
                newElem->mSize = 0;
                newElem->mIsLeaf = false;
                newElem->mChildren[0] = mRoot;
                newElem->SplitChild(0, mRoot);

                if (newElement->mKeys[0] < key){
                    newElement->mChildren[1]->InsertNotFull(key);
                }
                else {
                    newElement->mChildren[0]->InsertNotFull(key);
                }

                mRoot = newElem;
            }
        }
    }
}