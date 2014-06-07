#include "LTest.h"

using namespace std;

void tests_1(){
    LTest::ignore();
    LTest::ignore("test 1");
    LTest::addTest("test 1", [](){return true;});
    LTest::addTest("test 2", [](){return true;});
    LTest::addTest("test 3", [](){return true;});
    LTest::addTest("test 4", [](){return true;});
    LTest::ignore(3);
    LTest::ignore();
    LTest::addTest("test 5", [](){return true;});
    LTest::addTest("test 6", [](){return true;});
    LTest::addTest("test 7", [](){return true;});
    LTest::addTest("test 8", [](){return true;});
}

int main()
{
    tests_1();
    LTest::run();
    return 0;
}
