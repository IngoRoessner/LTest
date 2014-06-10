#ifndef LTEST_H
#define LTEST_H

#include <iostream>
#include <functional>
#include <list>
#include <map>
#include <string>
#include <tuple>
#include "LTestAssert.h"
#include "toStringPatch.h"

using namespace std;

class LTest
{
    private:
        typedef list<tuple<string, function<bool ()>>> TestListType;
        TestListType testCases;
        list<string> ok;
        list<string> fail;
        map<string, bool> ignores;
        list<string> actualIgnore;
        list<tuple<string, string>> error;
        list<tuple<string, string>> assert;
        string prefix;
        LTest();
        static LTest& getInstanz();
        unsigned int counter;
        unsigned int testNumber;
    public:

        static void runTests();

        static void errorOut(ostream& os = cout);

        static void assertOut(ostream& os = cout);

        static void failOut(ostream& os = cout);

        static void okOut(ostream& os = cout);

        static void ignoreOut(ostream& os = cout);

        static void countOut(ostream& os = cout);

        static void testOut(ostream& os = cout);

        static void run(ostream& os = cout);

        static void addTest(string testName, function<bool ()> test);

        static void ignore(string testName);

        static void ignore(unsigned int number = 1);

};



#endif // LTEST_H
