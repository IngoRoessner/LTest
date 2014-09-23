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
#include "AsyncTaskExecuter.h"

using namespace LTestSource;

using TestSuite = std::list<std::string>;

class LTest
{
    typedef unsigned int uint;
    typedef std::map<testname, std::function<bool ()>> testname_to_function_mapping;

    //STATE
    testname_to_function_mapping testCases;
    std::map<uint, testname> test_inserted_order;
    std::set<testname> ignores;
    std::set<uint> ignored_indexes;
    TestResultSet resultset;
    uint counter;
    MuteStreamMap mutedStreams;

    //OPTIONS
    std::ostream* outstream_;
    Format format_;
    bool force_;
    unsigned int async_;

    LTest();
    LTest(LTest& other);
    LTest& operator=(const LTest&) = delete;

    bool isIgnored(std::string testName);
    bool isIgnored(uint testIndex);
    std::shared_ptr<TestResult> runTest(const std::string& testName, std::function<bool ()> testFunction);

public:
    static LTest& getInstanz();

    void setStreamCaptureMode(std::ostream& os, CaptureMode mode)
    {
        mutedStreams.setCaptureMode(os, mode);
    }

    //adds a test function to the test list, execution via run(), runTests() or runTest()
    void addTestFunction(std::string testName, std::function<bool ()> test);

    template<typename FunctType>
    FunctionPattern<std::string, FunctType, bool, AnyType>
    addTest(std::string testName, FunctType test)
    {
        addTestFunction(testName, test);
        return testName;
    }

    template<typename FunctType>
    FunctionPattern<std::string, FunctType, void, AnyType>
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
    std::string addTest(std::string testName, TestFuncType testFunction, ParamFuncType parameterFunction)
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
    TestResultSet run();

    //runTest() & output()
    TestResultSet run(std::string test);

    TestResultSet run(TestSuite testsuite);

    TestResultSet run(std::initializer_list<std::string> testsuite);

    //loop all tests, execute only not ignored tests (you can get the result output via output())
    TestResultSet runTests();

    //execute all tests with the given name by force (no ignores). (result output via output())
    TestResultSet runTest(const std::string test);

    TestResultSet runTests(const TestSuite testsuite);

    TestResultSet runTests(const std::initializer_list<std::string> testsuite);

    std::string getIgnoreLabel();

    //ignore the given test
    std::string ignore(std::string testName);

    //ignores the next #number tests
    std::string ignoreNext(unsigned int number = 1);

    std::string ignore(TestSuite testsuite);

    std::string ignore(std::initializer_list<std::string> testsuite);

    TestResultSet getResultSet();

    void clearResultSet();

//OPTIONS
    LTest outstream(std::ostream& os);

    LTest format(Format f);

    LTest force();

    LTest async(unsigned int i);
};

extern LTest& ltest;

#endif // LTEST_H
