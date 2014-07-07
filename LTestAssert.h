#ifndef LTESTASSERT_H_INCLUDED
#define LTESTASSERT_H_INCLUDED

#include <exception>
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
    void True(T b){
        if(!b){
            throw FalseAssert("bool = false, should be true");
        }
    }

    template<typename T>
    void True(T b, string message){
        if(!b){
            throw FalseAssert(message);
        }
    }

    template<typename T>
    void False(T b){
        if(b){
            throw FalseAssert("bool = true, should be false");
        }
    }

    template<typename T>
    void False(T b, string message){
        if(b){
            throw FalseAssert(message);
        }
    }

}

#endif // LTESTASSERT_H_INCLUDED
