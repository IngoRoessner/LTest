#include "../src/LTest.h"
#include "chrono_lib_test.h"
#include "addTest.h"

#include <fstream>

using namespace std;

int main(){
    ltest().run();
    ofstream file;
    file.open("bin/TEST-LTestTests.xml");
    ltest().format(Format::Xml).outstream(file).run();
    file.close();
}
