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
#include "LTestThreadPool.h"

using namespace LTestSource;

using TestSuite = std::list<std::string>;

class LTest
{
    typedef unsigned int uint;
    typedef std::map<testname, std::function<bool ()>> testname_to_function_mapping;
    testname_to_function_mapping testCases;
    std::map<uint, testname> test_inserted_order;
    std::set<testname> ignores;
    std::set<uint> ignored_indexes;
    TestResultSet resultset;
    LTest();
    static LTest& getInstanz();
    uint counter;
    MuteStreamMap mutedStreams;

    static bool isIgnored(std::string testName);
    static bool isIgnored(uint testIndex);

    static std::shared_ptr<TestResult> runTest(const std::string& testName, std::function<bool ()> testFunction);

public:
    static void setStreamCaptureMode(std::ostream& os, CaptureMode mode)
    {
        getInstanz().mutedStreams.setCaptureMode(os, mode);
    }

    //adds a test function to the test list, execution via run(), runTests() or runTest()
    static void addTestFunction(std::string testName, std::function<bool ()> test);

    template<typename FunctType>
    static FunctionPattern<std::string, FunctType, bool, AnyType>
    addTest(std::string testName, FunctType test)
    {
        addTestFunction(testName, test);
        return testName;
    }

    template<typename FunctType>
    static FunctionPattern<std::string, FunctType, void, AnyType>
    addTest(std::string testName, FunctType test)
    {
        std::function<bool()> foo = [=]()
        {
            test();
            return true;
        };
        addTestFunction(testName, foo);
        return testName;
    }


    template<typename TestFuncType, typename ParamFuncType>
    static std::string addTest(std::string testName, TestFuncType testFunction, ParamFuncType parameterFunction)
    {
        std::function<bool()> foo = [=]()
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
    static TestResultSet run(std::ostream& os = std::cout, Format format = Format::Text);

    //runTest() & output()
    static TestResultSet run(std::string test, std::ostream& os = std::cout, Format format = Format::Text);

    static TestResultSet run(TestSuite testsuite, bool force = false, std::ostream& os = std::cout, Format format = Format::Text);

    static TestResultSet run(std::initializer_list<std::string> testsuite, bool force = false, std::ostream& os = std::cout, Format format = Format::Text);

    //loop all tests, execute only not ignored tests (you can get the result output via output())
    static TestResultSet runTests();

    //execute all tests with the given name by force (no ignores). (result output via output())
    static TestResultSet runTest(const std::string test);

    static TestResultSet runTests(const TestSuite testsuite, bool force = false);

    static TestResultSet runTests(const std::initializer_list<std::string> testsuite, bool force = false);

    static std::string getIgnoreLabel();

    //ignore the given test
    static std::string ignore(std::string testName);

    //ignores the next #number tests
    static std::string ignoreNext(unsigned int number = 1);

    static std::string ignore(TestSuite testsuite);

    static std::string ignore(std::initializer_list<std::string> testsuite);

    static TestResultSet getResultSet();

    static void clearResultSet();

};

#endif // LTEST_H
