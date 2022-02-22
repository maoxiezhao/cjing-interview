
// Small alloc strategy:
// MAX_SMALL_SIZE is 64
// According to size divided into 4 free lists: 8 16 32 64
// 小内存范围为 (0, 64]
// 基本策略就是将内存分为4裆位
    // (0, 8]   => 0; 
    // (8, 16]  => 1; 
    // (16, 32] => 2; 
    // (32, 64] => 3; 
// 每档都会创建以MemPage为Node的List，MemPage分为Header和实际Data块

// 分配：
// 当实际分配内存时，会GetFreeListIndex获取ListIndex，得到对应的MemPage
// 根据MemPageHeader.firstFree得到可用的mem

// 释放：
// 首先需要得到mem属于哪个MemPage,见GetMemPage,因为mem必定属于MemPage的内存块中
// 且MemPage按照PAGE_SIZE(4096)分配，则根据 (UIntPtr)ptr & ~(PAGE_SIZE - 1)必定可以得到MemPage
// 的起始指针
// 如果MemPage原来是Full且必定移除于链表外，则重新添加会链表
// 同时令Freelist = U32((U8*)mem - memPage->data)

// 重分配
// 如果新分配的大小属于同一裆位 (ListIndex相同)
// 则直接返回原指针，否则则重新分配一个小内存（或者大内存），memcpy拷贝到新的内存后，FreeSmall

#include <windows.h>
#include <math.h>

using U8 = unsigned short;
using U32 = unsigned int;
using UIntPtr = U64;

static constexpr U32 SMALL_ALLOC_MAX_SIZE = 64;
static constexpr U32 PAGE_SIZE = 4096;
static constexpr size_t MAX_PAGE_COUNT = 16384;

// Find first one bit (MSB to LSB)
static inline uint32_t LeadingZeroes(uint32_t x)
{
	unsigned long result;
	if (_BitScanReverse(&result, x))
		return 31 - result;
	else
		return 32;
}

void* MemReserve(size_t size)
{
    return VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_TOP_DOWN, PAGE_READWRITE);
}

void MemCommit(void* ptr, size_t size)
{
    VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
}

struct MemPage
{
    struct Header
    {
        MemPage* next;
        U32 firstFree;
        U32 itemSize;
    };
    Header header;
    U8 data[PAGE_SIZE - sizeof(Header)];
};

struct SmallAllocator
{
    MemPage* freeList[4];
    U32 pageCount = 0;
    U8* pageBuffer = nullptr;
}

static U32 GetFreeListIndex(size_t size)
{
    // According to size divided into 4 free lists: 8 16 32 64
    // (0, 8]   => 0; 
    // (8, 16]  => 1; 
    // (16, 32] => 2; 
    // (32, 64] => 3; 
    ASSERT(size > 0);
    ASSERT(size <= SMALL_ALLOC_MAX_SIZE);
    return 31 - LeadingZeroes(((U32)size - 1) >> 2); // 从小端开始找到第一个不是0的位置
}

static void InitMemPage(MemPage* page, int itemSize)
{
    MemCommit(page, PAGE_SIZE);
    page = new (page) MemPage();
    page->header.itemSize = itemSize;
    page->header.next = nullptr;
    page->header.firstFree = 0;

    // 默认情况下每个itemSize块第一个U32位都指向了下一个可用的位置
    for (int i = 0; i < (sizeof(page->data) / itemSize); i++)
        *(U32*)(&page->data[i * itemSize]) = i * itemSize + itemSize;
}

static void* AllocSmall(SmallAllocator& allocator, size_t size)
{
    U32 freeListIndex = GetFreeListIndex(size);
    if (allocator.pageBuffer == nullptr)
        allocator.pageBuffer = (U8*)MemReserve(MAX_PAGE_COUNT * PAGE_SIZE);

    MemPage* page = allocator.freeList[freeListIndex];
    if (page == nullptr)
    {
        // Allocate a new memPage
        if (allocator.pageCount == MAX_PAGE_COUNT)
            return nullptr;

        page = (MemPage*)(allocator.pageBuffer + PAGE_SIZE * allocator.pageCount);
        InitMemPage(page, 8 << freeListIndex);
        allocator.freeList[freeListIndex] = page;
        allocator.pageCount++;
    }

    U8* mem = &page->data[page->header.firstFree];
    page->header.firstFree = *(U32*)mem;  // 默认情况下每个itemSize块第一个U32位都指向了下一个可用的位置，见InitMemPage

    // 检测当前MemPage是否用完
    if (page->header.firstFree + page->header.itemSize > sizeof(page->data)) //  PAGE_SIZE - sizeof(Header)
    {
        // 令freeList指向pageList下一个MemPage
        if (allocator.freeList[freeListIndex] == page)
            allocator.freeList[freeListIndex] = page->header.next;

        // 将自己从List（可用PageList）移除（在Free后会重新添加到List）
        page->header.next = nullptr;
    }

    return static_cast<void*>(mem);
}

static MemPage* GetMemPage(void* ptr)
{
    return reinterpret_cast<MemPage*>((UIntPtr)ptr & ~(PAGE_SIZE - 1));
}

static void FreeSmall(SmallAllocator& allocator, void* ptr)
{
    if (ptr == nullptr)
        return;
    
    MemPage* memPage = GetMemPage(ptr);
    assert(memPage != nullptr);

    // 如果释放的是一个已经分配满的MemPage，说明已经从List中移除，则需要重新添加到
    // FreeList[index]链表中
    if (page->header.firstFree + page->header.itemSize > sizeof(page->data)) //  PAGE_SIZE - sizeof(Header)
    {
        assert(page->header.next == nullptr);
        U32 freeListIndex = GetFreeListIndex(size);
        page->header->next = allocator.freeList[freeListIndex];
        allocator.freeList[freeListIndex] = page;
    }

    *(U32*)ptr = page->header.firstFree;    // 令每个itemSize块第一个U32位都指向了下一个可用的位置
    page->header.firstFree = U32((U8*)ptr - page->data);
}

static void* Realloc(SmallAllocator& allocator, void* ptr, size_t newSize)
{  
    MemPage* memPage = GetMemPage(ptr);
    assert(memPage != nullptr);
    if (newSize <= SMALL_ALLOC_MAX_SIZE) // newSize <= 64
    {   
        // 如果FreeListIndex相同，则直接返回Ptr
        U32 freeListIndex = GetFreeListIndex(newSize);
        newPage = allocator.freeList[freeListIndex];
        if (newPage != nullptr && newPage->header.itemSize == memPage->header.itemSize)
            return ptr;
    }

    void* newMem = newSize <= SMALL_ALLOC_MAX_SIZE ? AllocSmall(alocator, newSize) : malloc(newSize);
    memcpy(newMem, ptr, std::min((size_t)page->header.itemSize, size));
    FreeSmall(allocator, ptr);
    return newMem;
}   