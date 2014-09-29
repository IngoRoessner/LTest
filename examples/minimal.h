#ifndef MINIMAL_H_INCLUDED
#define MINIMAL_H_INCLUDED

#include "../src/LTest.h"

namespace minimal{

auto testName = ltest().addTest("testname", []{return true;});

int main() {
    //ltest().run(testName);
    ltest().run();
    return 0;
}

};

#endif // MINIMAL_H_INCLUDED
