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
    void UnEqual(T1 a, T2 b, string message = "Assert::UnEqual -> Equal"){
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
    void ExpectException(string expectetWhat, function<void()> funct, string message = "expected exception was not thrown"){
        bool exceptionIsCatched = false;
        try{
            funct();
        }catch(ExceptionType e){
            if(e.what() == expectetWhat){
                exceptionIsCatched = true;
            }
        }
        True(exceptionIsCatched, message);
    }

}

#endif // LTESTASSERT_H_INCLUDED
