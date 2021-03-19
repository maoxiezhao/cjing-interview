#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <assert.h>

// https://blog.csdn.net/u012611878/article/details/72859047

// 观察者模式也可以称之为通知模式，例如事件通知、Callback回调都是观察者模式的应用

// simple impl
class Object;
class Observer
{
public:
    Observer();
    virtual Observer();

    void Callback(Object& obj) = 0;
}

class Object
{
public:
    void AddObserver(Observer* observer)
    void RemoveObserver(Observer* observer);
    void Notify()
    {
        for(auto observer : mObservers) {
            observer->Callback(*this);
        }
    }
private:
    std::vector<Observer*> mObservers;
}