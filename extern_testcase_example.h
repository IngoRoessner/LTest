#ifndef TESTCASE_H_INCLUDED
#define TESTCASE_H_INCLUDED

#include "LTest.h"
#include <string>

string str = "hallo";
string ExternTestCaseExample[] = {
    LTest::addTest("testCase", [&](){cout<<"testCase"<<str<<endl;}),
    LTest::addTest("testCase2", [](){cout<<"testCase2"<<endl;})
};


#endif // TESTCASE_H_INCLUDED
