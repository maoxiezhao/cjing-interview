## Floyd判圈算法
https://blog.csdn.net/u012534831/article/details/74231581  

基本思路就是快指针移动速度比慢指针快（next.next)，如果链表存在环，则快慢指针必然在链表结束前汇合

当两者相遇之后，固定一个指针，让另一个指针行走一圈，使用count计数，如果两个指针相等（即相遇），则count即为环的长度
-----
```
struct Node
{
    Node* next = nullptr;
}
Node* FindBeginLoop(Node* root)
{
    Node* slowPtr = root;
    Node* fastPtr = root;
    
    bool haveLoop = false;
    while(slowPtr.next && fastPtr.next && fastPtr.next.next)
    {
        slowPtr = slowPtr.next;
        fastPtr = fastPtr.next;
        if (slowPtr == fastPtr)
        {
            haveLoop = true;
            break;
        }
    }

    if(loopExists)
    {
        slowPtr=head;
        while(slowPtr!=fastPtr){
            slowPtr=slowPtr.getNext();
            fastPtr=fastPtr.getNext();
        }
        return slowPtr;
    } 

    return nullptr;
}

```