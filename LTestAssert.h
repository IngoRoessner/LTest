#ifndef LTESTASSERT_H_INCLUDED
#define LTESTASSERT_H_INCLUDED

#include <exception>
#include <string>

using namespace std;

namespace LTAssert{
    class FalseAssert: public exception{
    private:
        string text;
    public:
        FalseAssert(string str):exception(), text(str){}
        string what(){
            return text;
        }
    };

    template<typename T>
    void Equal(T a, T b){
        if(!(a==b)){
            throw FalseAssert("not equal");
        }
    }

    template<typename T>
    void UnEqual(T a, T b){
        if(a==b){
            throw FalseAssert("is equal, should be unequal");
        }
    }

    template<typename T>
    void True(T b){
        if(!b){
            throw FalseAssert("bool = false, should be true");
        }
    }

    template<typename T>
    void False(T b){
        if(b){
            throw FalseAssert("bool = true, should be false");
        }
    }

}

#endif // LTESTASSERT_H_INCLUDED
