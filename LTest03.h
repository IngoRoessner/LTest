#ifndef LTEST03_H_INCLUDED
#define LTEST03_H_INCLUDED


#include <string>

using namespace std;
template<typename LT, typename FunctType>
void boolLambdaAdd(string testName, FunctType test){
    throw "there should be no lambda in c++03";
}

template<typename LT, typename FunctType>
void voidLambdaAdd(string testName, FunctType test){
    throw "there should be no lambda in c++03";
}

template<typename LT, typename FunctType>
void boolFunctionAdd(string testName, FunctType test){
    LT::addTestFunction(testName, test);
}

template<typename LT, typename FunctType>
void voidFunctionAdd(string testName, FunctType test){
    //TODO
}

#endif // LTEST03_H_INCLUDED
