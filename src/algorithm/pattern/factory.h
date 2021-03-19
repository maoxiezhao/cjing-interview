#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>

// C++下三种工厂实现：https://blog.csdn.net/silangquan/article/details/20492293
// Factory一般用于资源或者对象（多派生）实例的创建
// 类似于resourceFactory实现
struct ResourceType
{
    int mHashValue;
};

class Resource
{
};

class IFactory
{
public:
    virtual Resource* CreateResource() = 0;
    virtual void Destroy(Resource* res) = 0;
};

class ShaderFactory : public IFactory
{
public:
    Resource* CreateResource();
    void Destroy(Resource* res);
};

class TextureFactory : public IFactory
{
public:
    Resource* CreateResource();
    void Destroy(Resource* res);
};

class ResourceManager
{
public:
    void RegisterFactory(const ResourceType& type, IFactory* factory);
    IFactory* GetFactory(const ResourceType& type);
    Resource* CreateResource(const ResourceType& type)
    {
        IFactory* factory = GetFactory(type);
        if (!factory) {
            return nullptr;
        }
        return factory->CreateResource();
    }
};