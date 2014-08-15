#ifndef GLOBALFIXTURE_H_INCLUDED
#define GLOBALFIXTURE_H_INCLUDED

#include <list>
#include <functional>

using namespace std;

class GlobalFixtureBase{
public:
    virtual void runAfter() = 0;
};


class GlobalFixtureList: public list<GlobalFixtureBase*>{
public:
    static GlobalFixtureList& getInstance(){
        static  GlobalFixtureList instance;
        return instance;
    }

    void runAfter(){
        for(GlobalFixtureBase* globalFixturePtr : *this){
            globalFixturePtr->runAfter();
        }
    }

    static void after(){
        getInstance().runAfter();
    }
};

template<typename T>
class GlobalFixture: public GlobalFixtureBase{
    T& fixture;
    bool changed;
    function<void(T&)> beforeFunction;
    function<void(T&)> afterFunction;
public:
    GlobalFixture(T& t): fixture(t), changed(false){
        GlobalFixtureList::getInstance().push_back(this);
    }

    GlobalFixture(const GlobalFixture& other): fixture(other.fixture), changed(other.changed),
        beforeFunction(other.beforeFunction), afterFunction(other.afterFunction)
    {
        GlobalFixtureList::getInstance().push_back(this);
    }

    ~GlobalFixture(){
        GlobalFixtureList::getInstance().remove(this);
    }

    T& operator()(){
        if(changed == false && beforeFunction){
            beforeFunction(fixture);
        }
        changed = true;
        return fixture;
    }

    GlobalFixture<T>& before(function<void(T&)> funct){
        beforeFunction = funct;
        return *this;
    }

    GlobalFixture<T>& after(function<void(T&)> funct){
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


#endif // GLOBALFIXTURE_H_INCLUDED
