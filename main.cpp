#include "LTest.h"

using namespace std;

#if __cplusplus > 199711L
#include "LTest11.h"
void tests_1(){
    LTest::ignore("test 1");
    LT11::addTest("test 1", [](){return true;});
    LTest::ignore();
    LT11::addTest("test 2", [](){return true;});
    LT11::addTest("test 3", [](){return true;});
    LT11::addTest("test 4", [](){return true;});
    LT11::addTest("test 5", [](){LTAssert::True(false, "my message");});
    LT11::addTest("test 6", [](){LTAssert::True(false); return true;});
    LT11::addTest("test 7", [](){return true;});
    LT11::addTest("test 8", [](){return true;});

    //LTest::ignore(10);
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
#else
    bool testFooOld(){
        return true;
    }

    void tests_1(){
        LTest::addTest("old cpp", testFooOld);
    }
#endif // __cplusplus




int main()
{
    tests_1();
    LTest::run();
    return 0;
}
