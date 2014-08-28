#ifndef MINIMAL_H_INCLUDED
#define MINIMAL_H_INCLUDED

#include "../src/LTest.h"

namespace minimal{

auto testName = LTest::addTest("testname", []{return true;});

int main() {
    //LTest::run(testName);
    LTest::run();
    return 0;
}

};

#endif // MINIMAL_H_INCLUDED
