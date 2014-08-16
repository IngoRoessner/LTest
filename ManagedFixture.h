#ifndef GLOBALFIXTURE_H_INCLUDED
#define GLOBALFIXTURE_H_INCLUDED

#include <list>
#include <functional>

using namespace std;

class ManagedFixtureBase{
public:
    virtual void runAfter() = 0;
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
class ManagedFixture: public ManagedFixtureBase{
    T& fixture;
    bool changed;
    function<void(T&)> beforeFunction;
    function<void(T&)> afterFunction;
public:
    ManagedFixture(T& t): fixture(t), changed(false){
        ManagedFixtureList::getInstance().push_back(this);
    }

    ManagedFixture(const ManagedFixture& other): fixture(other.fixture), changed(other.changed),
        beforeFunction(other.beforeFunction), afterFunction(other.afterFunction)
    {
        ManagedFixtureList::getInstance().push_back(this);
    }

    ~ManagedFixture(){
        ManagedFixtureList::getInstance().remove(this);
    }

    T& operator()(){
        if(changed == false && beforeFunction){
            beforeFunction(fixture);
        }
        changed = true;
        return fixture;
    }

    ManagedFixture<T>& before(function<void(T&)> funct){
        beforeFunction = funct;
        return *this;
    }

    ManagedFixture<T>& after(function<void(T&)> funct){
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
