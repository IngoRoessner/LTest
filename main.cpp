#include "LTest.h"

using namespace std;

void tests_1(){
    LTest::addTest("test 1", [](){return true;});
    LTest::setPrefix("test2prefix");
    LTest::addTest("test 2", [](){return false;});
}

int main()
{
    tests_1();
    LTest::run();
    return 0;
}
