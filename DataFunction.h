#ifndef DATAFUNCTION_H_INCLUDED
#define DATAFUNCTION_H_INCLUDED

#include "LTestAssert.h"
#include "sstream"

using namespace std;

template<class T>
string argsToString(T arg){
    string result;
    stringstream ss;
    ss<<arg;
    result = ss.str();
    return result;
}

template<class T, class... Types>
string argsToString(T arg, Types&&... args){
    string result;
    stringstream ss;
    ss<<arg<<", ";
    result = ss.str();
    result = result + argsToString(args...);
    return result;
}

class DataFunctionBase{
public:
    virtual bool isVoidReturn(){
        return false;
    }

};

template<typename ReturnType, typename... ParameterTypes>
class DataFunction: public DataFunctionBase{
public:
    typedef ReturnType(*FooType)(ParameterTypes...);
    FooType foo;
    DataFunction(FooType f){
        foo = f;
    }

    template <typename T, class... Types>
    void run(T ret, Types&&... args){
        ReturnType result = foo(args...);
        LTAssert::Equal(result, ret, "Fail at fixture("+argsToString(args...)+") -> "+argsToString(ret)+"!="+argsToString(result));
    }
};

template<typename... ParameterTypes>
class DataFunction<void, ParameterTypes...>: public DataFunctionBase{
public:
    typedef void(*FooType)(ParameterTypes...);
    FooType foo;
    DataFunction(FooType f){
        foo = f;
    }

    template <class... Types>
    void run(Types&&... args){
        foo(args...);
    }

    bool isVoidReturn(){
        return true;
    }
};


#endif // DATAFUNCTION_H_INCLUDED
