#ifndef LEAP_YEAR_H_INCLUDED
#define LEAP_YEAR_H_INCLUDED

#include "../src/LTest.h"
#include <limits>

namespace leap_year{

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

    //to time internsive
    LTest::ignoreNext(),
    LTest::addTest("test is not negative: ", [](){
        int min_int = std::numeric_limits<int>::min()/20; //divided by 20 to reduce time
        for(int i = 0 ; i > min_int; i--){
            LTAssert::False(is_leap_year(i));
        }
    })
};

TestSuite suite2 = {
    LTest::addTest("test whole leap year function", is_leap_year, [](auto test){
        test.with(2014).expect(false);
        test.with(1900).expect(false);
        test.with(2000).expect(true);
        test.with(2004).expect(true);
    })
};

int main() {
    LTest::run(suite1, true); //force to run the time intensive but ignored "test is not negative: " test
    LTest::run(suite2);
    LTest::run();
    return 0;
}

};
#endif // LEAP_YEAR_H_INCLUDED
