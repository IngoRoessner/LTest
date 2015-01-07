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

#ifndef RESULTSET_H_INCLUDED
#define RESULTSET_H_INCLUDED

#include <thread>
#include "../src/LTest.h"
#include "time.h"

namespace resultset{

constexpr unsigned int DELAY = 200;

void sleep(){
    std::this_thread::sleep_for(std::chrono::milliseconds(DELAY + 1));
}

// lets prepare you testsuite and insert the tests
TestSuite myTests = {
    ltest().addTest("ok", [](){}),

    ltest().addTest("ok slow", [](){
        sleep();
    }),

    ltest().addTest("slow because of work", [](){
        std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now(), current_time;
        while(true){
            current_time = std::chrono::system_clock::now();
            double timeWaited = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - start).count() / 1e6;
            if(timeWaited > static_cast<double>(DELAY)){
                break;
            }
        }
    }),

    ltest().addTest("failed", [](){
        return false;
    }),

    ltest().addTest("failed slow", [](){
        sleep();
        LTAssert::Equal("a", "b");
    }),

    ltest().addTest("slow aborted test", [](){
        sleep();
        throw 1;
    }),
    ltest().ignoreNext(),
    ltest().addTest("Well I would be slow, but I'm ignored.", [](){
        sleep();
    })
};

/**
 * Example describing how to programmatically access the test results
 * *** Important note: ***
 * If you try to export your test results into another format instead (e.g. JSON, YAML),
 * have a look at OutputFormat/OutputFormat.h, OutputFormatBase<ResultSetType> or the example XmlOutput.h.
 */
int main() {
    TestResultSet result = ltest().run(); // lets run all our added tests and retrieve the TestResultSet (= Basically a list of TestResult objects)

    // how to filter on test state
    // you can use this type wrapping syntax to filter on Ok/Failed/Aborted/Ingnored tests (see TestResult with it's subtypes for details)
    ResultSet<TestResultFailed> failedTests(result);

    // or you can just use the predefined filter methods doing the same
    auto failedTests2 = result.getFails();
    LTAssert::Equal(failedTests,failedTests2);

    // now lets observe how we can even select further on our TestResultSet

    auto filterSlowTests = [](std::shared_ptr<TestResult> currentTestResult) -> bool {
        double executionTimeInSeconds = currentTestResult -> get_time_taken();
        double executionTimeInMillSec = executionTimeInSeconds * 1000;
        return  executionTimeInMillSec > DELAY;
    };

    TestResultSet slowTests = result.getSubSet(filterSlowTests); // retrieves all tests which took longer than DELAY seconds
    std::cout << slowTests.out() << std::endl;   // output the slow tests
    return 0;
}


};


#endif // RESULTSET_H_INCLUDED
