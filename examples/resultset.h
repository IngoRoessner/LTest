#ifndef RESULTSET_H_INCLUDED
#define RESULTSET_H_INCLUDED

#include "../src/LTest.h"

namespace resultset{

int main() {
    TestResultSet result = LTest::run();
    ResultSet<TestResultFailed> failedTests(result);
    auto okTests = result.getOK();
    int i = 0;
    auto odd = result.getSubSet([&](shared_ptr<TestResult> element){return ++i%2 == 0;});
    auto oddFails = failedTests.getSubSet([&](shared_ptr<TestResultFailed> element){return ++i%2 == 0;});
    cout<<"###########Failed##################"<<endl;
    cout<<failedTests.out();
    cout<<"###########OK##################"<<endl;
    cout<<okTests.out();
    cout<<"###########odd##################"<<endl;
    cout<<odd.out();
    cout<<"############oddFails#################"<<endl;
    cout<<oddFails.out();
    return 0;
}

};


#endif // RESULTSET_H_INCLUDED
