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
    typedef function<ReturnType(ParameterTypes...)> FooType;
    FooType foo;

    DataFunction(FooType f){
        foo = f;
        count = 0;
    }

    template <typename T, class... Types>
    void run(T ret, Types&&... args){
        ++count;
        typename conditional<is_reference<ReturnType>::value, typename remove_reference<ReturnType>::type, ReturnType>::type result;
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
    typedef function<void(ParameterTypes...)> FooType;
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
            string msg = "unknown exception at fixture ";
            stringstream sstm;
            sstm << msg << count;
            throw LTAssert::FalseAssert(sstm.str());
        }
    }

    bool isVoidReturn(){
        return true;
    }
};

template<typename Functor>
struct DataFunctionTypeWrapper{
    typedef typename DataFunctionTypeWrapper<decltype(&Functor::operator())>::type type;
};


template <typename ClassType, typename ReturnType, typename... ParameterTypes>
struct DataFunctionTypeWrapper<ReturnType(ClassType::*)(ParameterTypes...) const>
{
    typedef DataFunction<ReturnType, ParameterTypes...> type;
};

template <typename ReturnType, typename... ParameterTypes>
struct DataFunctionTypeWrapper<ReturnType(*)(ParameterTypes...)>
{
    typedef DataFunction<ReturnType, ParameterTypes...> type;
};


#endif // DATAFUNCTION_H_INCLUDED
