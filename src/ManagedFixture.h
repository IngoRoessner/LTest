/*
 *     The MIT License (MIT)
 *
 *     Copyright (c) 2014 Ingo Rössner
 *                        Dan Häberlein
 *
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 */


#ifndef GLOBALFIXTURE_H_INCLUDED
#define GLOBALFIXTURE_H_INCLUDED

#include <list>
#include <functional>
#include <type_traits>
#include <map>
#include <thread>
#include <mutex>

namespace LTestSource{
    template<typename T>
    class FixtureSync{
        std::map<std::thread::id, bool> change;
        std::map<T* , std::mutex> fixturemutex;
        std::mutex changeMutex;

    public:
        static FixtureSync& getInstance(){
            static FixtureSync instance;
            return instance;
        }

        bool& changed(){
            changeMutex.lock();
            std::thread::id thID = std::this_thread::get_id();
            if(!change.count(thID)){
                change[thID] = false;
            }
            changeMutex.unlock();
            return change[thID];
        }

        void lock(T& fixture){
            fixturemutex[&fixture].lock();
        }

        void unlock(T& fixture){
            fixturemutex[&fixture].unlock();
        }
    };
}

class ManagedFixtureBase{
public:
    virtual void runAfter() = 0;
};


class ManagedFixtureList: public std::list<ManagedFixtureBase*>{
public:
    static ManagedFixtureList& getInstance(){
        static  ManagedFixtureList instance;
        return instance;
    }

    void runAfter(){
        for(ManagedFixtureBase* fixturePtr : *this){
            fixturePtr->runAfter();
        }
    }

    static void after(){
        getInstance().runAfter();
    }
};

template<typename T>
class ManagedRefFixture: public ManagedFixtureBase{
    T& fixture;
    std::function<void(T&)> beforeFunction;
    std::function<void(T&)> afterFunction;
    LTestSource::FixtureSync<T>& sync;
public:
    ManagedRefFixture(T& t): fixture(t), sync(LTestSource::FixtureSync<T>::getInstance()){
        ManagedFixtureList::getInstance().push_back(this);
    }

    ManagedRefFixture(const ManagedRefFixture& other):
        fixture(other.fixture), beforeFunction(other.beforeFunction),
        afterFunction(other.afterFunction),sync(LTestSource::FixtureSync<T>::getInstance())
    {
        ManagedFixtureList::getInstance().push_back(this);
    }

    ~ManagedRefFixture(){
        ManagedFixtureList::getInstance().remove(this);
    }

    T& operator()(){
        if(!sync.changed()){
            sync.lock(fixture);
        }
        if(sync.changed() == false && beforeFunction){
            beforeFunction(fixture);
        }
        sync.changed() = true;
        return fixture;
    }

    ManagedRefFixture<T>& before(std::function<void(T&)> funct){
        beforeFunction = funct;
        return *this;
    }

    ManagedRefFixture<T>& after(std::function<void(T&)> funct){
        afterFunction = funct;
        return *this;
    }

    void runAfter(){
        if(sync.changed() && afterFunction){
            afterFunction(fixture);
        }
        sync.changed() = false;
        sync.unlock(fixture);
    }
};

template<typename T>
class ManagedRValFixture: public ManagedFixtureBase{
    T fixture_r_value;
    T& fixture;
    std::function<void(T&)> beforeFunction;
    std::function<void(T&)> afterFunction;
    LTestSource::FixtureSync<T>& sync;
public:

    ManagedRValFixture(T&& t): fixture_r_value(t), fixture(fixture_r_value), sync(LTestSource::FixtureSync<T>::getInstance()){
        ManagedFixtureList::getInstance().push_back(this);
    }

    ManagedRValFixture(T t): fixture_r_value(t), fixture(fixture_r_value), sync(LTestSource::FixtureSync<T>::getInstance()){
        ManagedFixtureList::getInstance().push_back(this);
    }

    ManagedRValFixture(): fixture_r_value(), fixture(fixture_r_value), sync(LTestSource::FixtureSync<T>::getInstance()){
        ManagedFixtureList::getInstance().push_back(this);
    }

    ManagedRValFixture(const ManagedRValFixture& other): fixture_r_value(other.fixture_r_value),
        fixture(fixture_r_value),
        beforeFunction(other.beforeFunction),
        afterFunction(other.afterFunction),
        sync(other.sync)
    {
        ManagedFixtureList::getInstance().push_back(this);
    }

    ~ManagedRValFixture(){
        ManagedFixtureList::getInstance().remove(this);
    }

    T& operator()(){
        if(!sync.changed()){
            sync.lock(fixture);
        }
        if(sync.changed() == false && beforeFunction){
            beforeFunction(fixture);
        }
        sync.changed() = true;
        return fixture;
    }

    ManagedRValFixture<T>& before(std::function<void(T&)> funct){
        beforeFunction = funct;
        return *this;
    }

    ManagedRValFixture<T>& after(std::function<void(T&)> funct){
        afterFunction = funct;
        return *this;
    }

    void runAfter(){
        if(sync.changed() && afterFunction){
            afterFunction(fixture);
        }
        sync.changed() = false;
        sync.unlock(fixture);
    }
};



template <typename T>
using ManagedFixture = typename std::conditional< std::is_reference<T>::value,
                                                 ManagedRefFixture<typename std::remove_reference<T>::type>,
												 ManagedRValFixture<typename std::remove_reference<T>::type>
												>::type;


template<typename T>
ManagedFixture<T> manageFixture(T instance){
    return ManagedFixture<T>(instance);
}

template<typename T>
ManagedFixture<T> manageFixture(){
    return ManagedFixture<T>();
}

#endif // GLOBALFIXTURE_H_INCLUDED
