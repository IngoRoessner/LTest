#ifndef DATAFUNCTION_H_INCLUDED
#define DATAFUNCTION_H_INCLUDED

#include "LTestAssert.h"
#include "sstream"
#include <type_traits>

using namespace std;


class DataFunctionBase{
public:
    unsigned int count;
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
        count = 0;
    }

    template <typename T, class... Types>
    void run(T ret, Types&&... args){
        ++count;
        ReturnType result;
        stringstream sstm;
        string msg;
        try{
            result = foo(args...);
        }catch(...){
            msg = "Exception at fixture ";
            sstm << msg << count;
            throw LTAssert::FalseAssert(sstm.str());
        }
        msg = "Fail at fixture ";
        sstm << msg << count;
        LTAssert::Equal(result, ret, sstm.str());
    }
};

template<typename... ParameterTypes>
class DataFunction<void, ParameterTypes...>: public DataFunctionBase{
public:
    typedef void(*FooType)(ParameterTypes...);
    FooType foo;

    DataFunction(FooType f){
        foo = f;
        count = 0;
    }

    template <class... Types>
    void run(Types&&... args){
        ++count;
        try{
            foo(args...);
        }catch(...){
            string msg = "Exception at fixture ";
            stringstream sstm;
            sstm << msg << count;
            throw LTAssert::FalseAssert(sstm.str());
        }
    }

    bool isVoidReturn(){
        return true;
    }
};


#endif // DATAFUNCTION_H_INCLUDED
