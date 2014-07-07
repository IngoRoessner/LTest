#ifndef LTEST11_H_INCLUDED
#define LTEST11_H_INCLUDED

#include "LTest.h"
#include "function_traits.h"

namespace LT11{
    template<typename FunctType>
    static typename RetIs<FunctType, bool>::type addTest(string testName, FunctType test){
        function<bool()> foo = test;
        LTest::addTest(testName, foo);
    }

    template<typename FunctType>
    static typename RetIs<FunctType, void>::type addTest(string testName, FunctType test){
        function<bool()> foo = [&](){test(); return true;};
        LTest::addTest(testName, foo);
    }

    template<typename FunctType>
    static typename RetIs<FunctType, bool(*)()>::type addTest(string testName, FunctType test){
        function<bool()> foo = [&](){return test();};
        LTest::addTest(testName, foo);
    }

    template<typename FunctType>
    static typename RetIs<FunctType, void(*)()>::type addTest(string testName, FunctType test){
        function<bool()> foo = [&](){test(); return true;};
        LTest::addTest(testName, foo);
    }
}



#endif // LTEST11_H_INCLUDED
