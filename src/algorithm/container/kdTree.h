#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>
#include <array>

// https://www.joinquant.com/view/community/detail/c2c41c79657cebf8cd871b44ce4f5d97
// 本质是一个多维的binary search tree
// TODO: 目前并不是一个balanced kd-tree

template <typename T, size_t N>
class KDTree
{
public:
    using PointT = std::array<F32, N>;

private:
    class NodeElement;
    static NodeElement *EMPTY_NODE = new NodeElement();
    struct NodeElement
    {
        NodeElement *left = nullptr;
        NodeElement *right = nullptr;
        T value;
        PointT splitPoint;

        PointT operator PointT()
        {
            return splitPoint;
        }
    };

    struct KDTreeImpl
    {
        NodeElement *root = nullptr;
    };
    KDTreeImpl mImpl;

public:
    NodeElement *GetNearest(const PointT &point)
    {
        int level = 0;
        float dist2 = Dist2(point, PointT(*mImpl.root));
        return GetNearestImpl(point, mImpl.root, mImpl.root, dist2, level);
    }

private:
    void SortPoints(int leftIndex, int rightIndex, std::vector<PointT> &points, int level)
    {
        auto begin = &points.at(leftIndex);
        auto end = &points.at(rightIndex);
        std::nth_element(begin, begin + (leftIndex + rightIndex) / 2,
                         end, [&](const PointT &p1, const PointT &p2)
                         { return p1.at(level) < p2.at(level); });
    }

    NodeElement *BuildTree(int leftIndex, int rightIndex, std::vector<PointT> &points, int level)
    {
        if (leftIndex == rightIndex)
            return EMPTY_NODE;

        int length = rightIndex - leftIndex + 1;
        if (length > 1)
            SortPoints();

        int middle = leftIndex + (length / 2);
        NodeElement *left = nullptr;
        if (middle > leftIndex)
        {
            left = BuildTree(leftIndex, middle, points, (level + 1) % N);
        }

        NodeElement *right = nullptr;
        if (middle < rightIndex)
        {
            right = BuildTree(middle + 1, rightIndex, points, (level + 1) % N);
        }

        return {
            left,
            right,
            T(),
            points[middle]};
    }

    NodeElement *GetNearestImpl(const PointT &point, NodeElement *branch, NodeElement *bestNode, float bestDist2, int level)
    {
        if (branch == nullptr)
            return EMPTY_NODE;

        // 1. 检测当前父节点是否是最近节点
        float dist2 = Dist2(point, PointT(branch));
        if (dist2 < bestDist2)
        {
            bestDist2 = dist2;
            bestNode = branch;
        }

        NodeElement *next = nullptr;
        NodeElement *other = nullptr;
        float dx = point.at(level) < PointT(*branch).at(level);
        if (dx < 0)
        {
            next = branch->left;
            other = branch->right;
        }
        else
        {
            next = branch->right;
            other = branch->left;
        }

        int nextLevel = (level + 1) % N;
        // 2. 检查目标子节点返回的节点是否是最小节点
        NodeElement *further = GetNearestImpl(point, next, bestNode, bestDist2, nextLevel);
        if (further != EMPTY_NODE)
        {
            float d = Dist2(PointT(further), point);
            if (d < bestDist2)
            {
                bestDist2 = d;
                bestNode = further;
            }
        }

        // 3. 检查距离分割线的距离，是否小于当前距离，如果小于则在另一个分支可能存在更接近的点
        if (dx * dx < bestDist2)
        {
            NodeElement *further = GetNearestImpl(point, other, bestNode, bestDist2, nextLevel);
            if (further != EMPTY_NODE)
            {
                float d = Dist2(PointT(further), point);
                if (d < bestDist2)
                {
                    bestDist2 = d;
                    bestNode = further;
                }
            }
            return bestNode;
        }
    }

    float Dist(const PointT &p1, const PointT &p2)
    {
        float dist = 0.0f;
        for (size_t i = 0; i < N; i++)
            dist += fabs(p1.at(i) - p2.at(i));
        return dist;
    }

    float Dist2(const PointT &p1, const PointT &p2)
    {
        float dist2 = 0.0f;
        for (size_t i = 0; i < N; i++)
        {
            float d = fabs(p1.at(i) - p2.at(i));
            dist2 += d * d;
        }
        return dist2;
    }
};