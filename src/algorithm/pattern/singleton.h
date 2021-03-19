#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>

// 饿汉式和懒汉式概念：https://blog.csdn.net/fuzhongmin05/article/details/71001857
// C++下多线程安全实现： https://segmentfault.com/a/1190000020244045
// 单例模式一般用于全局模块Manager实现

// 懒汉式多线程安全最佳实现： 局部静态变量
class Singleton
{
public:
    Singleton& GetInstance()
    {
        static Singleton singleton;
        return singleton;
    }
};

