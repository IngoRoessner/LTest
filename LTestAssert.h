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

    template<typename T>
    void Equal(T a, T b, string message){
        if(!(a==b)){
            throw FalseAssert(message);
        }
    }

    template<typename T>
    void UnEqual(T a, T b, string message){
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

}

#endif // LTESTASSERT_H_INCLUDED
