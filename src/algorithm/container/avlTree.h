#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>

// https://www.programiz.com/dsa/avl-tree

class AVLTree
{
private:
    struct NodeElement
	{
		int mValue = 0;
        int mHeight = 1;
		NodeElement* mLeft = nullptr;
		NodeElement* mRight = nullptr;
	};

    inline int GetHeight(NodeElement* node)const  {
        return node != nullptr ? node->mHeight : 0;
    }

    //   4            2
    //  / \    =>      \
    // 2   5            4
    //  \              / \
    //   3            3   5
    NodeElement* RightRotate(NodeElement* node)
    {
        NodeElement* left = node->mLeft;
        if (!left) {
            return node;
        }

        NodeElement* rightChild = left->mRight;
        left->mRight = node;
        node->mLeft = rightChild;
        node->mHeight = std::max(GetHeight(node->mLeft), GetHeight(node->mRight)) + 1;
        left->mHeight = std::max(GetHeight(left->mLeft), GetHeight(left->mRight)) + 1;
    
        return left;
    }

    //   3            5
    //  / \    =>    / \
    // 2   5        3   6
    //    / \      / \ 
    //   4   6    2   4
    NodeElement* LeftRotate(NodeElement* node)
    {
        NodeElement* right = node->mRight;
        if (!right) {
            return node;
        }

        NodeElement* leftChild = right->mLeft;
        right->mLeft = node;
        node->mRight = leftChild;
        node->mHeight = std::max(GetHeight(node->mLeft), GetHeight(node->mRight)) + 1;
        right->mHeight = std::max(GetHeight(right->mLeft), GetHeight(right->mRight)) + 1;
    
        return right;
    }

    int GetBalanceFactor(NodeElement* node)
    {
        if (!node) {
            return 0;
        }
        return GetHeight(node->mLeft) - GetHeight(node->mRight);
    }

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

    NodeElement* Insert(NodeElement* node, int key)
    {
        if (node == nullptr) {
            return new NodeElement();
        }

        if (key < node->mValue) {
            node->mLeft = Insert(node->mLeft, key);
        }
        else if (key > node->mValue) {
            node->mRight = Insert(node->mRight, key);
        }
        else {
            return node;
        }

        // 1. update the balance factor
        node->mHeight = std::max(GetHeight(node->mLeft), GetHeight(node->mRight)) + 1;
        int balanceFactor = GetBalanceFactor(node);
        
        // 2 .balance the tree
        if (balanceFactor > 1)
        {
            if (key < node->mLeft->mValue) {
                return RightRotate(node);
            }
            else if (key > node->mLeft->mValue) 
            {
                node->mLeft = LeftRotate(node->mLeft);
                return RightRotate(node);
            }
        }
        else if (balanceFactor < -1)
        {
            if (key > node->mRight->mValue) {
                return LeftRotate(node);
            }
            else if (key < node->mLeft->mValue) 
            {
                node->mRight = RightRotate(node->mRight);
                return LeftRotate(node);
            }
        }
        return node;
    }

    NodeElement* Delete(NodeElement* root, int key)
    {
        if (root == nullptr) {
            return nullptr;
        }

        if (key < root->mValue) {
            root->mLeft = Delete(root->mLeft, key);
        }
        else if (key > root->mValue) {
            root->mRight = Delete(root->mRight, key);
        }
        else 
        {
            // 1. case 1: only one child
            if (!root->mLeft || !root->mRight) 
            {
                NodeElement* temp = root->mLeft ? root->mLeft : root->mRight;
                if (!temp) 
                {
                    *root = *temp;
                    delete temp;
                }
            }
            else
            {
                // case 2: two children
                NodeElement* temp = FindInorderSuccessor(root->mRight);
                if (temp)
                {
                    // swap root and successor, and then delete successor
                    root->mValue = temp->mValue;
                    root->mRight = Delete(root->mRight, temp->mValue);
                }
            }
        }

        if (root == nullptr) {
            return nullptr;
        }

        // 1. update the balance factor
        root->mHeight = std::max(GetHeight(root->mLeft), GetHeight(root->mRight)) + 1;
        int balanceFactor = GetBalanceFactor(root);
        
        // 2 .balance the tree
        if (balanceFactor > 1)
        {
            // 新添加的节点在左节点左边，直接右旋
            if (GetBalanceFactor(root->mLeft) >= 0) {
                return RightRotate(root);
            }
            else 
            // 需要先对左子节点左旋再右旋
            {
                root->mLeft = LeftRotate(root->mLeft);
                return RightRotate(root);
            }
        }
        else if (balanceFactor < -1)
        {
            // 新添加的节点在右节点右边，直接左旋
            if (GetBalanceFactor(root->mRight) <= 0) {
                return LeftRotate(root);
            }
            else 
            // 需要先对右子节点右旋再左旋
            {
                root->mRight = RightRotate(root->mLeft);
                return LeftRotate(root);
            }
        }
        return root;
    }
};