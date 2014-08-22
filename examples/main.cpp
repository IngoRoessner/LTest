#include "LTest.h"
#include <limits>
#include "LTestAssert.h"

//#define LEAP_YEAR_EXAMPLE

#ifdef LEAP_YEAR_EXAMPLE

bool is_leap_year(int year){
    return year > 0 and ((year % 4 == 0 and year % 100 != 0) or year % 400 == 0);
}

TestSuite suite1 = {
    LTest::addTest("test is leap year 2014 : ", [](){
        return !is_leap_year(2014);
    }),
    LTest::addTest("test is leap year 1900 : ", [](){
        return !is_leap_year(1900);
    }),
    LTest::addTest("test is leap year 2000 : ", [](){
        return is_leap_year(2000);
    }),
    LTest::addTest("test is not negative: ", [](){
        int min_int = std::numeric_limits<int>::min();
        for(int i = 0 ; i > min_int; i--){
            LTAssert::False(is_leap_year(i));
        }
    })
};

TestSuite suite2 = {
    LTest::addTest("test whole leap year function", is_leap_year, []{
        LTest::arguments(2014).expect(false);
        LTest::arguments(1900).expect(false);
        LTest::arguments(2000).expect(true);
        LTest::arguments(2004).expect(true);
    })
};

int main() {
    LTest::run(suite1);
    LTest::run(suite2);
    LTest::run();
    return 0;
}

#elif MINIMAL_EXAMPLE

auto testName = LTest::addTest("testname", []{return true;});

int main() {
    LTest::run(testName);
    return 0;
}

#endif
