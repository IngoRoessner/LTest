#include "../src/LTest.h"
#include "chrono_lib_test.h"
#include "addTest.h"

#include <fstream>

int main(){
    LTest::run();
    ofstream file;
    file.open("bin/TEST-LTestTests.xml");
    LTest::run(file, Format::Xml);
}
