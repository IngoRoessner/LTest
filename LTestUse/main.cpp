#include "LTest.h"
#include "MuteStream.h"
#include "extern_testsuite_example.h"
#include "datatest.h"
#include <fstream>
using namespace std;

void foo1(){}

bool foo2(){return true;}

void tests_1(){
    LTest::addTest("void funkt pointer", foo1);
    LTest::addTest("bool funkt pointer", foo2);

    LTest::addTest("not unique exception", [](){
        LTAssert::ExpectException<logic_error>([](){
            LTest::addTest("bool funkt pointer", foo2);
        });
    });

    LTest::addTest("not unique exception2", [](){
        LTAssert::ExpectException<logic_error>("testname 'bool funkt pointer' is not unique", [](){
            LTest::addTest("bool funkt pointer", foo2);
        });
    });

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

void printResults(const TestResultSet& results) {
	for (auto& result : results) {
		cout << "*******************************" << endl;
		cout << "programmatic access to: " << result->get_testname() << endl;
		//cout << "state: " << result->get_state() << endl;
		try {
			cout << "content of cout, if any: " << result->get_output_mapping().at(&cout)<<endl;;
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
    LTest::run();
    cout<<endl<<endl;

    //you can run one TestSuite (TestSuite == list<string>)
    LTest::run(ExternTestSuiteExample);
    cout<<endl<<endl;

	TestResultSet results = LTest::runTests(ExternTestSuiteExample, true);
	printResults(results);
    results.out();
    cout<<endl<<endl;
    LTest::clearResultSet();
    ofstream test_file;
    test_file.open("bin/TEST-ExternTestSuiteExample.xml");
    LTest::run(ExternTestSuiteExample, false, test_file, Format::Xml);
    test_file.close();
    //you can ignore TestSuites
    LTest::ignore(ExternTestSuiteExample);
    LTest::run();

    return 0;
}
