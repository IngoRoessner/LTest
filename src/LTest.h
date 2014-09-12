/*
 *     The MIT License (MIT)
 *
 *     Copyright (c) 2014 Ingo Rössner
 *                        Dan Häberlein
 *
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 */


#ifndef LTEST_H
#define LTEST_H

#include <iostream>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <algorithm>
#include <iterator>
#include <future>
#include "LTestAssert.h"
#include "toStringPatch.h"
#include "MuteStream.h"
#include "ParameterTest.h"
#include "TestResult.h"
#include "OutputFormat/OutputFormat.h"
#include <time.h>
#include "LTestMisuseException.h"
#include "ManagedFixture.h"
#include "FunctionPattern.h"

using namespace std;

using TestSuite = list<string>;

class LTest
{
    typedef unsigned int uint;
    typedef map<testname, function<bool ()>> testname_to_function_mapping;
    testname_to_function_mapping testCases;
    map<uint, testname> test_inserted_order;
    set<testname> ignores;
    set<uint> ignored_indexes;
    TestResultSet resultset;
    LTest();
    static LTest& getInstanz();
    uint counter;
    MuteStreamMap mutedStreams;

    static bool isIgnored(string testName);
    static bool isIgnored(uint testIndex);

    static shared_ptr<TestResult> runTest(const string& testName, function<bool ()> testFunction);

public:
    static void setVerboseMode(ostream& os, VerboseMode mode)
    {
        getInstanz().mutedStreams.setVerboseMode(os, mode);
    }

    //adds a test function to the test list, execution via run(), runTests() or runTest()
    static void addTestFunction(string testName, function<bool ()> test);

    template<typename FunctType>
    static FunctionPattern<string, FunctType, bool, functionpattern::AnyType>
    addTest(string testName, FunctType test)
    {
        addTestFunction(testName, test);
        return testName;
    }

    template<typename FunctType>
    static FunctionPattern<string, FunctType, void, functionpattern::AnyType>
    addTest(string testName, FunctType test)
    {
        function<bool()> foo = [=]()
        {
            test();
            return true;
        };
        addTestFunction(testName, foo);
        return testName;
    }


    template<typename TestFuncType, typename ParamFuncType>
    static string addTest(string testName, TestFuncType testFunction, ParamFuncType parameterFunction)
    {
        function<bool()> foo = [=]()
        {
            typedef typename ParameterTestType<TestFuncType>::type TestType;
            auto tempParamTest = TestType(testFunction);
            parameterFunction(tempParamTest);
            return true;
        };
        addTestFunction(testName, foo);
        return testName;
    }

    //runTests() & output()
    static TestResultSet run(ostream& os = cout, Format format = Format::Text);

    //runTest() & output()
    static shared_ptr<TestResult> run(string test, ostream& os = cout, Format format = Format::Text);

    static TestResultSet run(TestSuite testsuite, bool force = false, ostream& os = cout, Format format = Format::Text);

    static TestResultSet run(initializer_list<string> testsuite, bool force = false, ostream& os = cout, Format format = Format::Text);

    //loop all tests, execute only not ignored tests (you can get the result output via output())
    static TestResultSet runTests();

    //execute all tests with the given name by force (no ignores). (result output via output())
    static shared_ptr<TestResult> runTest(const string test);

    static TestResultSet runTests(const TestSuite testsuite, bool force = false);

    static TestResultSet runTests(const initializer_list<string> testsuite, bool force = false);

    static string getIgnoreLabel();

    //ignore the given test
    static string ignore(string testName);

    //ignores the next #number tests
    static string ignoreNext(unsigned int number = 1);

    static string ignore(TestSuite testsuite);

    static string ignore(initializer_list<string> testsuite);

    static TestResultSet getResultSet();

    static void clearResultSet();

};

#endif // LTEST_H
