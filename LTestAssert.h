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


#ifndef LTESTASSERT_H_INCLUDED
#define LTESTASSERT_H_INCLUDED

#include <string>

using namespace std;

namespace LTAssert{
    class FalseAssert{
    private:
        string text;
    public:
        FalseAssert(string str):text(str){}
        string what(){
            return text;
        }
    };

    template<typename T1, typename T2>
    void Equal(T1 a, T2 b, string message = "Assert::Equal -> not Equal"){
        if(!(a==b)){
            throw FalseAssert(message);
        }
    }

    template<typename T1, typename T2>
    void NotEqual(T1 a, T2 b, string message = "Assert::NotEqual -> Equal"){
        if(a==b){
            throw FalseAssert(message);
        }
    }


    template<typename T>
    void True(T b, string message = "bool = false, should be true"){
        if(!b){
            throw FalseAssert(message);
        }
    }


    template<typename T>
    void False(T b, string message = "bool = true, should be false"){
        if(b){
            throw FalseAssert(message);
        }
    }


    template<typename ExceptionType>
    void ExpectException(function<void()> funct, string message = "expected exception was not thrown"){
        bool exceptionIsCatched = false;
        try{
            funct();
        }catch(ExceptionType){
            exceptionIsCatched = true;
        }
        True(exceptionIsCatched, message);
    }

    template<typename ExceptionType>
    void ExpectException(string expectedWhat, function<void()> funct, string message = "expected exception was not thrown"){
        bool exceptionIsCatched = false;
        try{
            funct();
        }catch(ExceptionType e){
            if(e.what() == expectedWhat){
                exceptionIsCatched = true;
            }
        }
        True(exceptionIsCatched, message);
    }

}

#endif // LTESTASSERT_H_INCLUDED
