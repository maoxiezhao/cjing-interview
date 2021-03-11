#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>

template<typename KeyT, typename ValueT, typename Hasher>
class HashTable
{
private:
    Hasher  mHasher;
    KeyT*   mKeys	   = nullptr;
    ValueT* mValues    = nullptr;
    size_t* mHashTable = nullptr;

    size_t  mCapacity  = 0;
    size_t  mCapacityThreshold = 0;
    size_t  mSize = 0;
    size_t  mMask = 0;  // mCapacity - 1

private:
    size_t GetPosByHash(size_t hash)const
    {
        return hash & mMask;
    }

    // 获取hash的期望位置和curPos的距离
    size_t ProbeDistanceHash(size_t hash, size_t index)
    {
        return (index - GetPosByHash(hash) + mCapacity) & mMask;
    }

    ValueT* InsertImpl(size_t hash, KeyT&& key, const ValueT& value)
    {
        size_t pos = GetPosByHash(hash);
        size_t dist = 0;
        ValueT* ret = nullptr;
        while (true)
        {
            // 如果指定位置为空，则直接在该位置创建key和value
            if (mHashTable[pos] == 0)
            {
                new (&mKeys[pos])  KeyT(std::move(key));
                new (&mValues[pos]) ValueT(value);
                mHashTable[pos] = hash;

                if (ret == nullptr) {
                    ret = &mValues[pos];
                }
                return ret;
            }
            // 如果位置已被占用（冲突），使用线性开放式寻址寻找下一个可用位置
            else
            {
                // 获取当前已存在的对象的期望hashPos和pos的距离，如果这个距离小于dist，
                // 交换已存在对象和elem,之后则寻找已存在对象的下一个可用位置.
                // 目的是使得所有elem的位置偏移尽可能小，减少冲突次数
                size_t curElemProbeDist = ProbeDistanceHash(mHashTable[pos], pos);
                if (curElemProbeDist < dist)
                {
                    std::swap(hash, mHashTable[pos]);
                    std::swap(key, mKeys[pos]);
                    // std::swap(value, mValues[pos]
                    ValueT temp = mValues[pos];
                    mValues[pos] = value;
                    value = temp;

                    mValues[pos] = value;
                    dist = curElemProbeDist;

                    if (ret == nullptr) {
                        ret = &mValues[pos];
                    }
                }
                pos = (pos + 1) & mMask;
                dist++;
            }
        }
        return ret;
    }

    int GetIndexByKey(const KeyT& key)const
    {
        size_t hash = HashKey(key);
        size_t pos = GetPosByHash(hash);
        size_t dist = 0;
        while (true)
        {
            if (dist >= mCapacity) {
                return -1;
            }
            if (mHashTable[pos] == 0) {
                return -1;
            }
            else if (mHashTable[pos] == hash && mKeys[pos] == key) {
                return pos;
            }
            
            // 如果冲突，使用线性开放式寻址寻找下一个位置
            pos = (pos + 1) & mMask;
            dist++;
        }
        return 0;
    }


public:
    size_t HashKey(const KeyT& key)const
    {
			long hash = mHasher((long)0, key);
			hash &= 0x7fffffff;
			hash |= hash == 0;
			return (size_t)hash;
    }

    ValueT* insert(KeyT key, const ValueT& value)
    {
        if (auto ret = find(key))
        {
            *ret = value;
            return ret;
        }

        if ((++mSize) >= mCapacityThreshold) {
            Grow();
        }
        return InsertImpl(HashKey(key), std::move(key), std::move(value));
    }

    ValueT* find(const KeyT& key)
    {
        int pos = GetIndexByKey(key);
        return pos != -1 ? &mValues[pos] : nullptr;
    }
};