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


#ifndef ARGUMENTS_H_INCLUDED
#define ARGUMENTS_H_INCLUDED

#include "DataFunction.h"
#include "function_traits.h"
#include <tuple>
#include "LTestMisuseException.h"

using namespace std;


template<typename... Types>
class Arguments{
    DataFunctionBase* dataFunction;
    tuple<Types...> storedArgs;
    bool dataFuncReturnsVoid;
    bool executed;

    void checkAndThrowWhenVoidFunction(string&& errorMessage){
    	if(dataFuncReturnsVoid){
    		throw ExpectAtVoid(errorMessage);
    	}
    }

public:

    Arguments(DataFunctionBase* datafunc, Types... args):dataFunction(datafunc), storedArgs(args...) {
        dataFuncReturnsVoid = datafunc->isVoidReturn();
        if(dataFuncReturnsVoid){
            executed = true;
            (dynamic_cast<DataFunction<void, Types...>*>(dataFunction))->run(args...);
        }else{
            executed = false;
        }
    }

    ~Arguments(){
        if(!executed){
            throw MissingExpect("fixture is not executed. use expect()");
        }
    }

    template<typename Ret>
    void expect(Ret expectedValue){
    	checkAndThrowWhenVoidFunction("void function can't expect anything");
        executed = true;
        (dynamic_cast<DataFunction<Ret, Types...>*>(dataFunction))->runWithTuple(expectedValue, storedArgs);
    }

    template<typename Funct>
    void validate(Funct validator){
        typedef typename GetFunctParamType<Funct>::type Ret;
        checkAndThrowWhenVoidFunction("void function can't validate anything");
        executed = true;
        (dynamic_cast<DataFunction<Ret, Types...>*>(dataFunction))->validate(validator, storedArgs);
    }

    template<typename T>
    void anything(){
    	checkAndThrowWhenVoidFunction("void function get nothing not anything");
        executed = true;
        (dynamic_cast<DataFunction<T, Types...>*>(dataFunction))->validate([](T t){return true;}, storedArgs);
    }
};

#endif // ARGUMENTS_H_INCLUDED
