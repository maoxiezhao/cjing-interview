#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>

// based on the very nice implementation of rb-trees by:
// https://web.archive.org/web/20120507164830/http://web.mit.edu/~emin/www/source_code/red_black_tree/index.html

// tutorial: https://www.programiz.com/dsa/deletion-from-a-red-black-tree
// Properties:
// Red/Black Property: Every node is colored, either red or black.
// Root Property: The root is black.
// Leaf Property: Every leaf (NIL) is black.
// Red Property: If a red node has children then, the children are always black.
// Depth Property: For each node, any simple path from this node to any of its descendant
//                 leaf has the same black-depth (the number of black nodes).

template <class T>
struct Comparator
{
    inline bool operator()(const T &p_a, const T &p_b) const { return (p_a < p_b); }
};

template <typename KeyT, typename ValueT, class C = Comparator<KeyT>>
class RBTree
{
private:
    enum Color
    {
        RED,
        BLACK,
    };

    struct RBTreeImpl;
    struct NodeElement
    {
        // base member
        Color mColor = BLACK;
        NodeElement *mLeft = nullptr;
        NodeElement *mRight = nullptr;
        KeyT mkey;
        ValueT mValue;
        RBTreeImpl &mImpl;

        // external member
        NodeElement *mParent = nullptr;
        NodeElement *mNext = nullptr;
        NodeElement *mPrev = nullptr;

        NodeElement(RBTreeImpl &impl) : mImpl(impl) {}

        /////////////////////////////////////////////////////
        // methods
        void SetColor(Color color)
        {
            mColor = color;
        }

        NodeElement *GetPredecessor() const
        {
            NodeElement *ret = nullptr;
            // if left is not null, find maximum from left child
            if (mLeft != nullptr)
            {
                ret = mLeft;
                while (ret->mRight != nullptr)
                {
                    ret = ret->mRight;
                }
            }
            else
            {
                // 否则获取作为左节点时的父节点
                while (ret == ret->mParent->mLeft)
                {
                    ret = ret->parent;
                }
                return ret->mParent == impl.mRoot ? nullptr : ret->mParent;
            }
            return ret;
        }

        NodeElement *GetSuccessor() const
        {
            NodeElement *ret = nullptr;
            // if right is not null, find successor from right child
            if (mRight != nullptr)
            {
                ret = mRight;
                while (ret->mLeft != nullptr)
                {
                    ret = ret->mLeft;
                }
            }
            else
            {
                // 否则获取作为左节点时的父节点
                while (ret == ret->mParent->mRight)
                {
                    ret = ret->parent;
                }
                return ret->mParent == impl.mRoot ? nullptr : ret->mParent;
            }
            return ret;
        }
    };
    struct RBTreeImpl
    {
        NodeElement *mRoot = nullptr;
        int mSize = 0;

        void CreateRoot()
        {
            mRoot = new NodeElement();
            mRoot->mColor = BLACK;
            mRoot->mParent = mRoot->mLeft = mRoot->mRight = nullptr;
        }
    };
    RBTreeImpl mImpl;

    //  [3]           5
    //  / \    =>    / \
    // 2   5        3   6
    //    / \      / \ 
    //   4   6    2   4
    void LeftRotate(NodeElement *node)
    {
        NodeElement *r = node->mRight;
        if (!r)
        {
            return;
        }
        node->mRight = r->mLeft;
        if (r->mLeft)
        {
            r->mLeft->mParent = node;
        }

        r->mParent = node->mParent;
        if (node == node->mParent->mLeft)
        {
            node->mParent->mLeft = r;
        }
        else
        {
            node->mParent->mRight = r;
        }

        r->mLeft = node;
        node->mParent = r;
    }

    //  [4]           2
    //  / \    =>      \
    // 2   5            4
    //  \              / \
    //   3            3   5
    void RightRotate(NodeElement *node)
    {
        NodeElement *left = node->mRight;
        if (!left)
        {
            return;
        }
        node->mLeft = left->mRight;
        if (left->mRight)
        {
            left->mRight->mParent = node;
        }

        left->mParent = node->mParent;
        if (node == node->mParent->mLeft)
        {
            node->mParent->mLeft = left;
        }
        else
        {
            node->mParent->mRight = left;
        }

        left->mRight = node;
        node->mParent = left;
    }

    void InsertFixRB(NodeElement *newNode)
    {
        NodeElement *node = newNode;
        NodeElement *parent = node->mParent;
        NodeElement *grandParent;

        while (parent->mColor == Color::RED)
        {
            grandParent = parent->mParent;
            if (parent == grandParent->mLeft)
            {
                // case 1: 如果parent节点和兄弟节点都是红色，此时grandParent节点必为黑色
                // 设置parent和parent的兄弟节点都为black, grandParent设置为Red, node = grandParent继续
                if (grandParent->mRight->mColor == Color::RED)
                {
                    parent->SetColor(Color::BLACK);
                    grandParent->mRight->SetColor(Color::BLACK);
                    grandParent->SetColor(Color::RED);

                    node = grandParent;
                    parent = node->mParent;
                }
                // case 2 : 如果兄弟节点为黑色， grandParent节点必为黑色
                // 此时以设置changeColor(parent, grandParent),然后rightRotate（让parent成为父节点，来保证RB属性）
                // 此时parent->mRight必须是black，如果node==parent->mRight，需要先leftRotate
                else
                {
                    if (node == parent->mRight)
                    {
                        LeftRotate(parent);
                        node = parent;
                        parent = node->mParent;
                    }
                    parent->SetColor(Color::BLACK);
                    grandParent->SetColor(Color::RED);
                    RightRotate(grandParent);
                }
            }
            // 和上述情况一致
            else
            {
                if (grandParent->mLeft->mColor == Color::RED)
                {
                    parent->SetColor(Color::BLACK);
                    grandParent->mRight->SetColor(Color::BLACK);
                    grandParent->SetColor(Color::RED);

                    node = grandParent;
                    parent = node->mParent;
                }
                else
                {
                    if (node == parent->mLeft)
                    {
                        RightRotate(parent);
                        node = parent;
                        parent = node->mParent;
                    }
                    parent->SetColor(Color::BLACK);
                    grandParent->SetColor(Color::RED);
                    LeftRotate(grandParent);
                }
            }
        }
        _set_color(_data._root->left, BLACK);
    }

    void EraseFixRB(NodeElement *node)
    {
        // emmmm, too complex, just see: https://www.programiz.com/dsa/deletion-from-a-red-black-tree
    }

public:
    void Swap(NodeElement *node1, NodeElement *node2)
    {
    }

    NodeElement *Insert(const KeyT &key, const ValueT &value)
    {
        // 1. insert the new node in a suitable pos like binarySearchTree
        // 2. set color of new node is red
        // 3. fix rbTree

        if (mImpl.mRoot == nullptr)
        {
            mImpl.CreateRoot();
        }

        // find pos
        NodeElement *parent = mImpl.mRoot;
        C less;
        NodeElement *node = mImpl.mRoot->mLeft;
        while (node != nullptr)
        {
            parent = node;
            if (less(key, node->mKey))
            {
                node = node->mLeft;
            }
            else if (less(node->mKey, key))
            {
                node = node->mRight;
            }
            else
            {
                node->mValue = value;
                return node;
            }
        }

        // create new nodeElement
        NodeElement *newNode = new NodeElement(mImpl);
        newNode->mParent = parent;
        newNode->mKey = key;
        newNode->mValue = value;

        if (parent == mImpl.mRoot || less(key, parent->mKey))
        {
            parent->mLeft = newNode;
        }
        else
        {
            parent->mRight = newNode;
        }

        // set successor and predecessor
        newNode->mNext = newNode->GetSuccessor();
        newNode->mPrev = newNode->GetPredecessor();
        if (newNode->mNext)
        {
            newNode->mNext->mPrev = newNode;
        }
        if (newNode->mPrev)
        {
            newNode->mPrev->mNext = newNode;
        }

        mImpl.mSize++;
        // fix rbTree
        InsertFixRB(newNode);
        return newNode;
    }

    void Erase(NodeElement *target)
    {
        // 找到一个子节点或者后继节点，和当前节点交换删除，同时根据情况修复RBTree
        if (mImpl.mRoot == nullptr || target == nullptr)
        {
            return;
        }

        // rp为Removed节点，如果当前节点非满节点，则rp=target，否则rp=target->mNext
        NodeElement *rp = target->mLeft == nullptr || target->mRight == nullptr ? target : target->mNext;
        NodeElement *node = rp->mLeft != nullptr ? rp->mRight : rp->mLeft;
        NodeElement *slibling = nullptr;
        if (rp == rp->mParent->mLeft)
        {
            rp->mParent->mLeft = node;
            slibling = rp->mParent->mRight;
        }
        else
        {
            rp->mParent->mRight = node;
            slibling = rp->mParent->mLeft;
        }

        // PS：color of nil is black
        if (node->mColor == Color::RED) // rp->color must is Black,此时删除rp，设置node为black
        {
            node->mParent = rp->mParent;
            node->SetColor(Color::BLACK);
        }
        else if (rp->mColor == Color::BLACK && rp->mParent != mImpl.mRoot)
        {
            EraseFixRB(slibling);
        }

        if (rp != target)
        {
            Swap(rp, target);
        }
        if (target->mNext)
        {
            target->mNext->mPrev = target->mPrev;
        }
        if (target->mPrev)
        {
            target->mPrev->mNext = target->mNext;
        }
        delete target;
        mImpl.mSize--;
    }
};