#include "LTest.h"
#include "MuteStream.h"
#include "extern_testsuite_example.h"
#include "datatest.h"

using namespace std;

void foo1(){}

bool foo2(){return true;}

void tests_1(){
    LTest::addTest("void funkt pointer", foo1);
    LTest::addTest("bool funkt pointer", foo2);
    LTest::ignore("test 1");
    LTest::addTest("test 1", [](){return true;});
    LTest::ignoreNext();
    LTest::addTest("test 2", [](){return true;});
    LTest::addTest("test 3", [](){return true;});
    LTest::addTest("test 4", [](){return true;});
    LTest::addTest("test 5", [](){LTAssert::True(false, "my message");});
    LTest::addTest("test 6", [](){LTAssert::True(false); return true;});
    LTest::addTest("test 7", [](){return true;});
    LTest::addTest("test 8", [](){return true;});

    LTest::ignoreNext(10);
    LTest::addTest("test ignored 1", [](){cout<<"###"<<endl;return true;});
    LTest::addTest("test ignored 2", [](){return true;});
    LTest::addTest("test ignored 3", [](){return true;});
    LTest::addTest("test ignored 4", [](){return true;});
    LTest::addTest("test ignored 5", [](){return true;});
    LTest::addTest("test ignored 6", [](){return true;});
    LTest::addTest("test ignored 7", [](){return true;});
    LTest::addTest("test ignored 8", [](){return true;});
    LTest::addTest("test ignored 9", [](){return true;});
    LTest::addTest("test ignored 10", [](){return true;});
}

void printResults(const std::list<TestResult>& results) {
	for (auto& result : results) {
		cerr << result.get_testname() << endl;
		cerr << result.get_state() << endl;
		cerr << result.get_time_taken() << endl;
		try {
			cerr << result.get_output_mapping().at(&cout);
		} catch (...) {

		}
	}
}

int main()
{
    //you can mute test streams (cout on fail as default mute)
    //LTest::setMuteMode(cout, MuteMode::NONE);
    //LTest::setMuteMode(cout, MuteMode::EVERYTHING);
    //LTest::setMuteMode(cout, MuteMode::FAIL);
    //default LTest::setMuteMode(cout, MuteMode::FAIL);

    //you can define your tests in a function (tests_1())
    //or define in a TestSuite like in datatest.h
    tests_1();

    //you can run all
    std::list<TestResult> results = LTest::run();
    printResults(results);
    cout<<endl<<endl;

    //you can run one TestSuite (TestSuite == list<string>)
    LTest::run(ExternTestSuiteExample);
    cout<<endl<<endl;
	results = LTest::runTests(ExternTestSuiteExample, true);
	printResults(results);
    //you can run without output and do the output later
    LTest::output();
    cout<<endl<<endl;

    /*
    //you can chose where the results where streamed
    LTest::run(cout);

    LTest::runTest();
    LTest::output(cout);

    //you can stream result fragments
    LTest::errorOut(cout)
    */

    //you can ignore TestSuites
    LTest::ignore(ExternTestSuiteExample);
    LTest::run();

    return 0;
}
