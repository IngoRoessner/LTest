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
#include "FunctionPattern.h"

using namespace std;

const string EXCEPTION_MESSAGE = "Exception at fixture ";

template<typename T>
class ResultWrapper{
    T result;
    unsigned int count;

    void validateBool(function<bool(T)> validator, string message)
    {
        bool b = false;
        try{
            b = validator(result);
        }catch(LTAssert::FalseAssert a){
            message = a.what();
        }catch(...){
            message = "exception in validation";
        }
        LTAssert::True(b, "Fixture "+patch::to_string(count)+": "+message);
    }

    void validateVoid(function<void(T)> validator)
    {
        string message;
        bool b = false;
        try{
            validator(result);
            b = true;
        }catch(LTAssert::FalseAssert a){
            message = a.what();
        }catch(...){
            message = "exception in validation";
        }
        LTAssert::True(b, "Fixture "+patch::to_string(count)+": "+message);
    }

public:

    ResultWrapper(T t, unsigned int c): result(t), count(c){}

    void expect(T expectedValue, string message = "not expected value"){
    	LTAssert::Equal(expectedValue, result, "Fixture "+patch::to_string(count)+": "+message);
    }

    template<typename FunctType>
    FunctionPatternNot<T, void, FunctType, bool, T>
    expect(FunctType validator, string message = "validation fails")
    {
        validateBool(validator, message);
    }

    template<typename FunctType>
    FunctionPatternNot<T, void, FunctType, void, T>
    expect(FunctType validator)
    {
        validateVoid(validator);
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

    typedef function<ReturnType(ParameterTypes...)> FunctionType;
    FunctionType function_under_test;

    template <class... Types>
    ReturnType run(Types&&... args){
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

public:

    ParameterTest(FunctionType f){
        function_under_test = f;
        count = 0;
    }

    ResultWrapper<ReturnType> with(ParameterTypes... args){
        ++count;
        return ResultWrapper<ReturnType>(run(args...), count);
    }
};


template<typename... ParameterTypes>
class ParameterTest<void, ParameterTypes...>: public ParameterTestBase{

    typedef function<void(ParameterTypes...)> FunctionType;
    FunctionType function_under_test;

    template <class... Types>
    void run(Types&&... args){
        try{
            function_under_test(args...);
        }
        catch(LTAssert::FalseAssert e){
            string msg = "Fixture "+patch::to_string(count)+": "+e.what();
            throw LTAssert::FalseAssert(msg);
        }
        catch(...){
            throw runtime_error(EXCEPTION_MESSAGE+patch::to_string(count));
        }
    }

public:
    ParameterTest(FunctionType f){
        function_under_test = f;
        count = 0;
    }

    void with(ParameterTypes... args){
        ++count;
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
