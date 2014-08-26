/*
 *     The MIT License (MIT)
 *
 *     Copyright (c) 2014 Ingo Rössner
 *                        Dan Häberlein
 *
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 */


#ifndef PARAMETERTEST_H_INCLUDED
#define PARAMETERTEST_H_INCLUDED

#include "LTestAssert.h"
#include "sstream"
#include <type_traits>
#include <exception>
#include <string>
#include "function_traits.h"

using namespace std;

const string EXCEPTION_MESSAGE = "Exception at fixture ";

template<typename T>
class ResultWrapper{
    T result;
    unsigned int count;

public:

    ResultWrapper(T t, unsigned int c): result(t), count(c){}

    void expect(T expectedValue, string massage = "dont get expected value"){
    	LTAssert::Equal(expectedValue, result, "Fixture "+patch::to_string(count)+": "+massage);
    }

    void expect(function<bool(T)> validator, string massage = "validation fails"){
    	LTAssert::True(validator(result), "Fixture "+patch::to_string(count)+": "+massage);
    }

    T getResult(){
        return result;
    }
};

class ParameterTestBase{
public:
    unsigned int count;
};

template<typename ReturnType, typename... ParameterTypes>
class ParameterTest: public ParameterTestBase{
public:
    typedef function<ReturnType(ParameterTypes...)> FunctionWithReturnAndParameterType;
    FunctionWithReturnAndParameterType function_under_test;

    ParameterTest(FunctionWithReturnAndParameterType f){
        function_under_test = f;
        count = 0;
    }

    template <class... Types>
    ReturnType run(Types&&... args){
        ++count;
        try{
            return function_under_test(args...);
        }
        catch(LTAssert::FalseAssert e){
            string msg = "Fixture "+patch::to_string(count)+": "+e.what();
            throw LTAssert::FalseAssert(msg);
        }
        catch(...){
            throw runtime_error(EXCEPTION_MESSAGE + patch::to_string(count));
        }
    }

    ResultWrapper<ReturnType> with(ParameterTypes... args){
        return ResultWrapper<ReturnType>(run(args...), count);
    }
};


template<typename... ParameterTypes>
class ParameterTest<void, ParameterTypes...>: public ParameterTestBase{
public:
    typedef function<void(ParameterTypes...)> FooType;
    FooType foo;

    ParameterTest(FooType f){
        foo = f;
        count = 0;
    }

    template <class... Types>
    void run(Types&&... args){
        ++count;
        try{
            foo(args...);
        }
        catch(LTAssert::FalseAssert e){
            string msg = "Fixture "+patch::to_string(count)+": "+e.what();
            throw LTAssert::FalseAssert(msg);
        }
        catch(...){
            throw runtime_error(EXCEPTION_MESSAGE+patch::to_string(count));
        }
    }

    void with(ParameterTypes... args){
        run(args...);
    }
};

template<typename Functor>
struct ParameterTestType{
    typedef typename ParameterTestType<decltype(&Functor::operator())>::type type;
};


template <typename ClassType, typename ReturnType, typename... ParameterTypes>
struct ParameterTestType<ReturnType(ClassType::*)(ParameterTypes...) const>
{
    typedef ParameterTest<ReturnType, ParameterTypes...> type;
};

template <typename ReturnType, typename... ParameterTypes>
struct ParameterTestType<ReturnType(*)(ParameterTypes...)>
{
    typedef ParameterTest<ReturnType, ParameterTypes...> type;
};


#endif // PARAMETERTEST_H_INCLUDED
