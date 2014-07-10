#include "LTest.h"
#include "extern_testcase_example.h"

using namespace std;

void foo1(){}

bool foo2(){return true;}

void tests_1(){
    LTest::addTest("void funkt pointer", foo1);
    LTest::addTest("bool funkt pointer", foo2);
    LTest::ignore("test 1");
    LTest::addTest("test 1", [](){return true;});
    LTest::ignore();
    LTest::addTest("test 2", [](){return true;});
    LTest::addTest("test 3", [](){return true;});
    LTest::addTest("test 4", [](){return true;});
    LTest::addTest("test 5", [](){LTAssert::True(false, "my message");});
    LTest::addTest("test 6", [](){LTAssert::True(false); return true;});
    LTest::addTest("test 7", [](){return true;});
    LTest::addTest("test 8", [](){return true;});

    LTest::ignore(10);
    LTest::addTest("test ignored 1", [](){return true;});
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



int main()
{
    tests_1();
    LTest::run();

    cout<<endl<<endl;
    for(int i = 0; i<sizeof(ExternTestCaseExample) / sizeof(ExternTestCaseExample[0]); i++){
        LTest::runTest(ExternTestCaseExample[i]);
    }
    LTest::output();
    return 0;
}
