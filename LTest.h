#ifndef LTEST_H
#define LTEST_H

#include <iostream>
#include <functional>
#include <list>
#include <string>
#include <tuple>

using namespace std;

class LTest
{
    private:
        typedef list<tuple<string, function<bool ()>>> TestListType;
        TestListType testCases;
        list<string> ok;
        list<string> fail;
        list<tuple<string, string>> error;
        string prefix;
        LTest();
        static LTest& getInstanz();
    public:

        static void runTests();

        static void errorOut(ostream& os = cout);

        static void failOut(ostream& os = cout);

        static void okOut(ostream& os = cout);

        static void testOut(ostream& os = cout);

        static void run(ostream& os = cout);

        static void addTest(string testName, function<bool ()> test);

};



#endif // LTEST_H
