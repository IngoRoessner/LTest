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
    LTest::addTest("ok", [](){}),

    LTest::addTest("ok slow", [](){
        sleep();
    }),

    LTest::addTest("slow because of work", [](){
        std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now(), current_time;
        while(true){
            current_time = std::chrono::system_clock::now();
            double timeWaited = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - start).count() / 1e6;
            if(timeWaited > static_cast<double>(DELAY)){
                break;
            }
        }
    }),

    LTest::addTest("failed", [](){
        return false;
    }),

    LTest::addTest("failed slow", [](){
        sleep();
        LTAssert::Equal("a", "b");
    }),

    LTest::addTest("slow aborted test", [](){
        sleep();
        throw 1;
    }),
    LTest::ignoreNext(),
    LTest::addTest("Well I would be slow, but I'm ignored.", [](){
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
    TestResultSet result = LTest::run(); // lets run all our added tests and retrieve the TestResultSet (= Basically a list of TestResult objects)

    // how to filter on test state
    // you can use this type wrapping syntax to filter on Ok/Failed/Aborted/Ingnored tests (see TestResult with it's subtypes for details)
    ResultSet<TestResultFailed> failedTests(result);

    // or you can just use the predefined filter methods doing the same
    auto failedTests2 = result.getFails();
    LTAssert::Equal(failedTests,failedTests2);

    // now lets observe how we can even select further on our TestResultSet

    auto filterSlowTests = [](shared_ptr<TestResult> currentTestResult) -> bool {
        double executionTimeInSeconds = currentTestResult -> get_time_taken();
        double executionTimeInMillSec = executionTimeInSeconds * 1000;
        return  executionTimeInMillSec > DELAY;
    };

    TestResultSet slowTests = result.getSubSet(filterSlowTests); // retrieves all tests which took longer than DELAY seconds
    std::cout << slowTests.out() << endl;   // output the slow tests
    return 0;
}

};


#endif // RESULTSET_H_INCLUDED
