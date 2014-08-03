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
#include "function_traits.h"
#include "MuteStream.h"
#include "DataFunction.h"
#include <time.h>

using namespace std;

using TestSuite = list<string>;
typedef string testname;

enum TestState
{
    OK, FAILED, ABORTED, IGNORED
};

class TestResult
{
    TestState _state;
    double _time_taken;
    map<ostream*, string> _output_mapping;
    testname _tname;

public :

    TestResult(testname tname = "no_testname_given") : _state(TestState::IGNORED), _time_taken(0), _tname(tname) {}

    TestResult(TestState state, double time_taken, map<ostream*, string> output_mapping, testname tname)
        : _state(state), _time_taken(time_taken), _output_mapping(output_mapping), _tname(tname) {}

    TestState get_state() const
    {
        return _state;
    }

    double get_time_taken() const
    {
        return _time_taken;
    }

    map<ostream*, string> get_output_mapping() const
    {
        return _output_mapping;
    }

    string get_testname() const {
    	return _tname;
    }

};

class LTest
{
    typedef map<string, function<bool ()>> testname_to_function_mapping;
    testname_to_function_mapping testCases;
    list<testname> test_inserted_order;
    list<string> ok;
    list<string> fail;
    map<string, bool> ignores;
    list<string> actualIgnore;
    list<pair<string, string> > error;
    list<pair<string, string> > assert;
    LTest();
    static LTest& getInstanz();
    unsigned int counter;
    MuteStreamMap mutedStreams;
    DataFunctionBase* dataFunction;

    static bool isIgnored(string testName);

    static TestResult runTest(const string& testName, function<bool ()> testFunction);

public:
    static void setMuteMode(ostream& os, MuteMode mode)
    {
        getInstanz().mutedStreams.setMuteMode(os, mode);
    }

    //adds a test function to the test list, execution via run(), runTests() or runTest()
    static void addTestFunction(string testName, function<bool ()> test);

    template<typename FunctType>
    static typename FunctionTypeIs<FunctType, bool>::type addTest(string testName, FunctType test)
    {
        addTestFunction(testName, test);
        return testName;
    }

    template<typename FunctType>
    static typename FunctionTypeIs<FunctType, void>::type addTest(string testName, FunctType test)
    {
        function<bool()> foo = [&]()
        {
            test();
            return true;
        };
        addTestFunction(testName, foo);
        return testName;
    }

    template<typename FunctType>
    static typename FunctionTypeIs<FunctType, bool(*)()>::type addTest(string testName, FunctType test)
    {
        addTestFunction(testName, test);
        return testName;
    }

    template<typename FunctType>
    static typename FunctionTypeIs<FunctType, void(*)()>::type addTest(string testName, FunctType test)
    {
        function<bool()> foo = [=]()
        {
            test();
            return true;
        };
        addTestFunction(testName, foo);
        return testName;
    }


    template<typename Funct>
    static string addTest(string testName, Funct testFunction, function<void()> testDataFoo)
    {
        function<bool()> foo = [=]()
        {
            typedef typename DataFunctionTypeWrapper<Funct>::type DataFunctionType;
            auto datafunct = DataFunctionType(testFunction);
            getInstanz().dataFunction = &datafunct;
            testDataFoo();
            return true;
        };
        addTestFunction(testName, foo);
        return testName;
    }

    template <class... Types>
    static void fixture(Types&&... args)
    {
        if(getInstanz().dataFunction->isVoidReturn())
        {
            (dynamic_cast<DataFunction<void, Types...>*>(getInstanz().dataFunction))->run(args...);
        }
        else
        {
            (dynamic_cast<DataFunction<Types...>*>(getInstanz().dataFunction))->run(args...);
        }
    }

    //runTests() & output()
    static list<TestResult>run(ostream& os = cout);

    //runTest() & output()
    static TestResult run(string test, ostream& os = cout);

    static list<TestResult> run(TestSuite& testsuite, bool force = false, ostream& os = cout);

    //loop all tests, execute only not ignored tests (you can get the result output via output())
    static list<TestResult> runTests();

    //execute all tests with the given name by force (no ignores). (result output via output())
    static TestResult runTest(const string& test);

    static list<TestResult> runTests(const TestSuite& testsuite, bool force = false);

    static string getIgnoreLable();

    //ignore the given test
    static string ignore(string testName);

    //ignores the next #number tests
    static string ignoreNext(unsigned int number = 1);

    static string ignore(TestSuite& testsuite);

    static void clearOutput();

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
