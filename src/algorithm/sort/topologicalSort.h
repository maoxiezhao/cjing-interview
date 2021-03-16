#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <queue>
#include <assert.h>

struct Edge
{
    int u;
    int v;
};

std::vector<int> TopologicalSort(std::vector<int>& v, std::vector<Edge> e)
{
    // 构建入度表
    std::vector<int> inDegree(v.size(), 0);
    for(const auto& edge : e) {
        inDegree[edge.v]++;
    }

    // 添加所有入度为0的点
    std::queue<int> queue;
    for(int i = 0; i < inDegree.size(); i++) 
    {
        if (inDegree[i] == 0) {
            queue.push(i);
        }
    }

    std::vector<int> answer;
    while(!queue.empty())
    {
        int index = queue.front();
        queue.pop();
        answer.push_back(index);

        for(const auto& edge : e) 
        {
            if (edge.u == index)
            {
                inDegree[edge.v]--;
                if (inDegree[edge.v] == 0) {
                    queue.push(edge.v);
                }
            }
        }
    }

    // 说明存在环，无法拓扑排序
    if (answer.size() != v.size()) {
        assert(false);
    }

    return answer;
}