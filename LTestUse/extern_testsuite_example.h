#ifndef TESTCASE_H_INCLUDED
#define TESTCASE_H_INCLUDED

#include "LTest.h"
#include <string>
#include <list>

string str = "do your tests";
TestSuite ExternTestSuiteExample = {
    LTest::addTest("Suite testCase", [&](){cout<<"Suite testCase"<<str<<endl;}),
    LTest::addTest("Suite testCase2", [](){cout<<"Suite testCase2"<<endl;}),

    LTest::addTest("Suite fail cout", [](){cout<<"Suite testCase2"<<endl;return false;}),

    LTest::ignore(),
    LTest::addTest("Suite ignored 1", [](){cout<<"Suite ignored 1"<<endl;return false;}),

    LTest::ignore("Suite ignored 2"),
    LTest::addTest("Suite ignored 2", [](){cout<<"Suite ignored 2"<<endl;return false;}),

    LTest::ignore(2),
    LTest::addTest("Suite ignored 3", [](){cout<<"Suite ignored 3"<<endl;return false;}),
    LTest::addTest("Suite ignored 4", [](){cout<<"Suite ignored 4"<<endl;return false;}),

    LTest::addTest("Suite after ignores", [](){cout<<"Suite after ignores"<<endl;return false;}),
};


#endif // TESTCASE_H_INCLUDED
