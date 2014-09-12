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
#include <mutex>

using namespace std;

class ManagedFixtureBase{
public:
    virtual void runAfter() = 0;

    mutex fixtureMutex;
};


class ManagedFixtureList: public list<ManagedFixtureBase*>{
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
    bool changed;
    function<void(T&)> beforeFunction;
    function<void(T&)> afterFunction;
public:
    ManagedRefFixture(T& t): fixture(t), changed(false){
        ManagedFixtureList::getInstance().push_back(this);
    }

    ManagedRefFixture(const ManagedRefFixture& other):
        fixture(other.fixture), changed(other.changed), beforeFunction(other.beforeFunction),
        afterFunction(other.afterFunction)
    {
        ManagedFixtureList::getInstance().push_back(this);
    }

    ~ManagedRefFixture(){
        ManagedFixtureList::getInstance().remove(this);
    }

    T& operator()(){
        if(changed == false){
            fixtureMutex.lock();
        }
        if(changed == false && beforeFunction){
            beforeFunction(fixture);
        }
        changed = true;
        return fixture;
    }

    ManagedRefFixture<T>& before(function<void(T&)> funct){
        beforeFunction = funct;
        return *this;
    }

    ManagedRefFixture<T>& after(function<void(T&)> funct){
        afterFunction = funct;
        return *this;
    }

    void runAfter(){
        if(changed){
            fixtureMutex.unlock();
        }
        if(changed && afterFunction){
            afterFunction(fixture);
        }
        changed = false;
    }
};

template<typename T>
class ManagedRValFixture: public ManagedFixtureBase{
    T fixture_r_value;
    T& fixture;
    bool changed;
    function<void(T&)> beforeFunction;
    function<void(T&)> afterFunction;
public:

    ManagedRValFixture(T&& t): fixture_r_value(t), fixture(fixture_r_value), changed(false){
        ManagedFixtureList::getInstance().push_back(this);
    }

    ManagedRValFixture(T t): fixture_r_value(t), fixture(fixture_r_value), changed(false){
        ManagedFixtureList::getInstance().push_back(this);
    }

    ManagedRValFixture(): fixture_r_value(), fixture(fixture_r_value), changed(false){
        ManagedFixtureList::getInstance().push_back(this);
    }

    ManagedRValFixture(const ManagedRValFixture& other): fixture_r_value(other.fixture_r_value),
        fixture(fixture_r_value),
        changed(other.changed), beforeFunction(other.beforeFunction),
        afterFunction(other.afterFunction)
    {
        ManagedFixtureList::getInstance().push_back(this);
    }

    ~ManagedRValFixture(){
        ManagedFixtureList::getInstance().remove(this);
    }

    T& operator()(){
        if(changed == false && beforeFunction){
            beforeFunction(fixture);
        }
        changed = true;
        return fixture;
    }

    ManagedRValFixture<T>& before(function<void(T&)> funct){
        beforeFunction = funct;
        return *this;
    }

    ManagedRValFixture<T>& after(function<void(T&)> funct){
        afterFunction = funct;
        return *this;
    }

    void runAfter(){
        if(changed && afterFunction){
            afterFunction(fixture);
        }
        changed = false;
    }
};



template <typename T>
using ManagedFixture = typename conditional< is_reference<T>::value,
                                                 ManagedRefFixture<typename remove_reference<T>::type>,
												 ManagedRValFixture<typename remove_reference<T>::type>
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
