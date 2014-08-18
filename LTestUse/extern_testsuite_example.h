#ifndef TESTCASE_H_INCLUDED
#define TESTCASE_H_INCLUDED

#include "LTest.h"
#include <string>
#include <list>

#define EXTERNTESTSUITE ExternTestSuiteExample

string str = "do your tests";
TestSuite EXTERNTESTSUITE = {
    LTest::addTest("Suite testCase", [&](){cout<<"Suite testCase"<<str<<endl;}),
    LTest::addTest("Suite testCase2", [](){cout<<"Suite testCase2"<<endl;}),

    LTest::addTest("Suite fail cout", [](){cout<<"Suite testCase2"<<endl;return false;}),

    LTest::ignoreNext(),
    LTest::addTest("Suite ignored 1", [](){cout<<"Suite ignored 1"<<endl;return false;}),

    LTest::ignore("Suite ignored 2"),
    LTest::addTest("Suite ignored 2", [](){cout<<"Suite ignored 2"<<endl;return false;}),

    LTest::ignoreNext(2),
    LTest::addTest("Suite ignored 3", [](){cout<<"Suite ignored 3"<<endl;return false;}),
    LTest::addTest("Suite ignored 4", [](){cout<<"Suite ignored 4"<<endl;return false;}),

    LTest::addTest("Suite after ignores", [](){cout<<"Suite after ignores"<<endl;return false;}),

    LTest::addTest("Suite with cout and cerr", [](){cout<<"This goes to std out and will be available programmatically"<<endl; cerr << "and this to cerr!"; return true;}),

    LTest::addTest("throw ", [](){throw string("nope");}),
};


#endif // TESTCASE_H_INCLUDED
