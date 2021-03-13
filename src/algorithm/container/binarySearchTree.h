#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>

class BinarySearchTree
{
public:
    class TreeImpl;
    struct NodeElement
	{
		int mValue;
		NodeElement* mLeft = nullptr;
		NodeElement* mRight = nullptr;
		TreeImpl* mImpl = nullptr;
	};

    struct TreeImpl
	{
		NodeElement* mRoot = nullptr;
		size_t mSize = 0;
    };
    TreeImpl* mImpl = nullptr;

private:
    NodeElement* FindInorderSuccessor(NodeElement* node)
    {
        if (!node) {
            return nullptr;
        }

        NodeElement* successor = node;
        while (successor && successor->mLeft){
            successor = successor->mLeft;
        }     
        return successor == node ? nullptr : successor;
    }

    NodeElement* Find(int key)
    {
        if (!mImpl || !mImpl->mRoot) {
            return nullptr;
        }

        NodeElement* target = nullptr;
        NodeElement* node = mImpl->mRoot;
        while (node != nullptr)
        {
            if (node->mValue == key) 
            {
                target = node;
                break;
            }
            else if (key < node->mValue) {
                node = node->mLeft;
            }
            else {
                node = node->mRight;
            }
        }

        return target;
    }

    NodeElement* Insert(NodeElement* node, int key)
    {
        if (!mImpl) {
			mImpl = new TreeImpl();
		}

        auto it = Find(key);
        if (it) {
            return it;
        }

        NodeElement* parent = node;
        while (node != nullptr)
        {
            parent = node;
            if (key < node->mValue) {
                node = node->mLeft;
            }
            else {
                node = node->mRight;
            }
        }

        NodeElement* newElem = new NodeElement();
		newElem->mValue = key;
		newElem->mImpl = mImpl;
        
        if (parent) 
        {
            if (key < parent->mValue) {
                parent->mLeft = newElem;
            }
            else {
                parent->mRight = newElem;
            }
        }

        return newElem;;
    }

    void Insert(int key)
    {
        if (!mImpl) {
			mImpl = new TreeImpl();
		}
        auto node = Insert(mImpl->mRoot, key);
        if (node && !mImpl->mRoot) {
            mImpl->mRoot = node;
        }
    }

    NodeElement* Erase(NodeElement* node, int key)
    {
        if (node == nullptr) {
            return nullptr;
        }

        if (key < node->mValue) {
            node->mLeft = Erase(node->mLeft, key);
        }
        else if (key > node->mValue) {
            node->mRight = Erase(node->mRight, key);
        }
        else
        {
            if (node->mLeft == nullptr)
            {
                NodeElement* temp = node->mRight;
                delete node;
                return temp;
            }
            else if (node->mRight == nullptr)
            {
                NodeElement* temp = node->mLeft;
                delete node;
                return temp;                
            }
            else
            {
                NodeElement* successor = FindInorderSuccessor(node->mRight);
                node->mValue = successor->mValue;
                node->mRight = Erase(node->mRight, successor->mValue);
            }
        }
        return node;
    }

    NodeElement* Erase(int key)
    {
        if (!mImpl || !mImpl->mRoot) {
            return;
		}
        return Erase(mImpl->mRoot, key);
    }
};