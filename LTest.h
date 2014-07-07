#ifndef LTEST_H
#define LTEST_H

#include <iostream>
#include <functional>
#include <list>
#include <map>
#include <string>
#include <utility>
#include "LTestAssert.h"
#include "toStringPatch.h"


using namespace std;

class LTest
{
    private:
        //c++11??
        #if __cplusplus > 199711L
        typedef function<bool ()> funktionPointer;
        #else
        typedef bool(*funktionPointer)();
        #endif // __cplusplus

        typedef list<pair<string, funktionPointer> > TestListType;
        TestListType testCases;
        list<string> ok;
        list<string> fail;
        map<string, bool> ignores;
        list<string> actualIgnore;
        list<pair<string, string> > error;
        list<pair<string, string> > assert;
        string prefix;
        LTest();
        static LTest& getInstanz();
        unsigned int counter;
        unsigned int testNumber;

        static void runTest(const string& testName, funktionPointer& testFunction);

    public:
        //adds a test function to the test list, execution via run(), runTests() or runTest()
        static void addTest(string testName, funktionPointer test);

        //runTests() & output()
        static void run(ostream& os = cout);

        //runTest() & output()
        static void run(string test, ostream& os = cout);

        //loop all tests, execute only not ignored tests (result output via output())
        static void runTests();

        //execute all tests with the given name by force (no ignores). (result output via output())
        static void runTest(const string& test);

        //ignore the given test
        static void ignore(string testName);

        //ignores the next #number tests
        static void ignore(unsigned int number = 1);

        //run errorOut(), assertOut(), failOut(), okOut(), ignoreOut() and countOut()
        static void output(ostream& os = cout);

        static void errorOut(ostream& os = cout);

        static void assertOut(ostream& os = cout);

        static void failOut(ostream& os = cout);

        static void okOut(ostream& os = cout);

        static void ignoreOut(ostream& os = cout);

        static void countOut(ostream& os = cout);

};



#endif // LTEST_H
