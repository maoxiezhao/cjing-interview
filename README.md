# cjing-interview
收集一些面试相关的资料，对一些算法和实现做一个存档，部分待补全，持续更新中...  

  - [Basic algorithm](#basic-algorithm)
    - [Sort](#sort)
    - [Data Structures](#data-structures)
    - [Pattern](#pattern)
    - [Specific Algorithm](#specific-algorithm)
  - [Math](#math)
  - [DataBase](#database)
  - [Net](#net)
  - [MultiThread](#multithread)
  - [AI](#ai)
  - [Rendering](#rendering)
  - [Animation](#animation)
  - [Game](#game)
  - [Tips](#tips)
  - [Related links](#related-links)
****
## Basic algorithm
### Sort
QuickSort <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/sort/quickSort.h">[here]</a>  
MergeSort <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/sort/mergeSort.h">[here]</a>  
topologicalSort <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/sort/topologicalSort.h">[here]</a>  
heapSort <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/sort/heapSort.h">[here]</a> 

### Data Structures
List <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/container/list.h">[here]</a>  
HashTable <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/container/hashTable.h">[here]</a>  
Queue <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/container/queue.h">[here]</a>  
Stack <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/container/stack.h">[here]</a>  
binaryHeap <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/container/binaryHeap.h">[here]</a>  
binarySearchTree <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/container/binarySearchTree.h">[here]</a>  
AVL Tree <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/container/avlTree.h">[here]</a>  
B-Tree <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/container/bTree.h.h">[here]</a>  
RedBlack Tree <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/container/rbTree.h">[here]</a>  
Graph <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/container/graph.h">[here]</a>  
SegmentTree <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/container/segmentTree.h">[here]</a>  
KDTreee <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/container/kdTree.h">[here]</a>  

### Pattern
Singleton <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/pattern/singleton.h">[here]</a>  
Factory <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/pattern/factory.h">[here]</a>  
Observer <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/pattern/observer.h">[here]</a>  

### Specific Algorithm
摩尔投票算法：(找出众数) <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/other/摩尔投票算法.md">[here]</a>  
Floyd 判圈算法：(链表判圈) <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/other/Floyd判圈算法.md">[here]</a>  
单调栈算法：<a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/other/单调栈算法.md">[here]</a>  

****
## Math
弹簧阻尼系统 <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/math/spring.h">[here]</a>  

****
## DataBase
数据库面试总结 <a href ="https://blog.csdn.net/lxin_liu/article/details/89328058">[here]</a>  
SQL语句练习 <a href ="https://www.nowcoder.com/ta/sql?from=baidusql&bd_vid=11675627489460984601">[here]</a>  
常用SQL语句 <a href ="https://blog.csdn.net/Ace_2/article/details/85002791">[here]</a>  
common_sql <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/db/common_sqls.sql">[here]</a>  
LSM Tree解析  <a href ="https://hzhu212.github.io/posts/2d7c5edb/">[here]</a>  

****
## Net
TCP三次握手四次挥手解读 <a href ="https://mp.weixin.qq.com/s/tH8RFmjrveOmgLvk9hmrkw">[here]</a>   
基于ASIO的简易C/S架构  
ASIO浅析（ICOP和Epoll） <a href ="https://zhuanlan.zhihu.com/p/55503053">[here]</a>   

****
## MultiThread
基于Fiber的JobSystem实现 <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/docs/Fiber_jobsystem.docx">[here]</a>  
C++内存模型 <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/other/C++内存模型.md">[here]</a>  
一种无锁队列的实现 <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/algorithm/container/mpmc_bounded_queue.h">[here]</a>  

****
## AI 
行为树单节点数据多Task式的行为树实现

****
## Rendering
基于Vulkan的RenderGraph实现  
Forward+的pipeline实现

****
## Animation
Animation retarget  
GPU skinning <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/animation/gpuSkinning.md">[here]</a>  

****
## Game
简单的ECS系统实现  
简单的局部规避实现（非RVO算法）

****
## Tips 
memmove和memcpy <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/tips/memmove_memcpy.md">[here]</a>   
C++左值右值 <a href ="https://blog.csdn.net/xuwqiang1994/article/details/79924310">[here]</a>  
C++可变模板参数及其遍历 <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/tips/可变模板参数及其遍历.md">[here]</a>   
红点系统的树形结构实现 <a href ="https://zhuanlan.zhihu.com/p/231815818">[here]</a>  
std::remove实现原理 <a href = "https://www.zhihu.com/question/407495201/answer/1346022594">[here]</a>  
C++ CRTP介绍（模板类派生） <a href = "https://zhuanlan.zhihu.com/p/136258767">[here]</a>  
条件变量和信号量的区别 <a href ="https://github.com/maoxiezhao/cjing-interview/blob/main/src/tips/条件变量和信号量的区别.md">[here]</a>   

****
## Related links
基础算法和数据结构：DSA <a href ="https://www.programiz.com/dsa/stack">[here]</a>  
可视化算法：Visual algorithm <a href ="https://visualgo.net/zh">[here]</a>  
C++面试：Huihut interview <a href ="https://github.com/huihut/interview#algorithm">[here]</a>  
BTree实现：B-Tree <a href ="https://www.cnblogs.com/lianzhilei/p/11250589.html">[here]</a>  
C++对象模型 <a href ="https://www.cnblogs.com/QG-whz/p/4909359.html">[here]</a>  
线段树: SegmentTree <a href ="https://blog.csdn.net/zearot/article/details/48299459">[here]</a>  
C++面试总结（多方面） <a href ="https://www.cnblogs.com/QG-whz/p/4909359.html">[here]</a>  
C#面试总结（基础）<a href ="https://zhuanlan.zhihu.com/p/56522099">[here]</a>  
*线段树: SegmentTree <a href ="https://blog.csdn.net/lxin_liu/category_8859795.html">[here]</a>  