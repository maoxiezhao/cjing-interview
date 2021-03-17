1.memcpy和memmove相同点
都是用于从src拷贝count个字节到dest。

2.memcpy和memmove区别
如果目标区域和源区域有重叠的话：
memcpy不能够确保源串所在重叠区域在拷贝之前被覆盖。
memmove能够保证源串在被覆盖之前将重叠区域的字节拷贝到目标区域中，复制后src内容会被更改，当目标区域与源区域没有重叠则和memcpy函数功能相同。

但当源内存和目标内存存在重叠时，memcpy会出现错误，而memmove能正确地实施拷贝，但这也增加了一点点开销。


memmove的处理措施：
（1）当源内存的首地址等于目标内存的首地址时，不进行任何拷贝
（2）当源内存的首地址大于目标内存的首地址时，实行正向拷贝
（3）当源内存的首地址小于目标内存的首地址时，实行反向拷贝


----------------------------------------------------------------------------
//实现代码如下：
 
void * __cdecl memcpy(void * dst,const void * src,size_t count){
    void * ret = dst;

    while (count--) {
    // 注意， memcpy函数没有处理dst和src区域是否重叠的问题
        *(char *)dst = *(char *)src;
        dst = (char *)dst + 1;
        src = (char *)src + 1;
    }

    return(ret);
}
  
void * __cdecl memmove(void * dst,const void * src,size_t count){
    void * ret = dst;
    if (dst <= src || (char *)dst >= ((char *)src + count)) {
        // 若dst和src区域没有重叠，则从起始处开始逐一拷贝
        while (count--){
            *(char *)dst = *(char *)src;
            dst = (char *)dst + 1;
            src = (char *)src + 1;
        }
    }
    else{
    // 若dst和src 区域交叉，则从尾部开始向起始位置拷贝，这样可以避免数据冲突
        dst = (char *)dst + count - 1;
        src = (char *)src + count - 1;

        while (count--){
            *(char *)dst = *(char *)src;
            dst = (char *)dst - 1;
            src = (char *)src - 1;
        }
    }

    return(ret);
}